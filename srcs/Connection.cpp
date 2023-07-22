/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 08:33:19 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/22 15:20:33 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "utils.hpp"
#include <stdexcept>
#include <iostream>
#include <algorithm>

/* ************************************************************************** */
/*                           Constructor Destructor                           */
/* ************************************************************************** */

Connection::Connection(void)
{
	// std::cout << "Connection default constructor called" << std::endl;
	return ;
}

Connection::Connection(std::vector<Server*>& servers) : _servers(servers), _highestFd(-1)
{
	// std::cout << "Connection constructor called" << std::endl;
	this->_highestFd = 0;
	this->_timeout.tv_sec = 3;
	this->_timeout.tv_usec = 0;
	this->initMime();
}

Connection::Connection(const Connection &rhs)
{
	// std::cout << "Connection copy constructor called" << std::endl;
	*this = rhs;
}

Connection &Connection::operator=(const Connection &rhs)
{
	// std::cout << "Connection copy assignment operator called" << std::endl;
	if (this != &rhs) {
		this->_servers = rhs._servers;
		this->_client = rhs._client;
		this->_setReads = rhs._setReads;
		this->_setWrite = rhs._setWrite;
		this->_setErrors = rhs._setErrors;
		this->_highestFd = rhs._highestFd;
		this->_timeout = rhs._timeout;
		this->_mimeTypes = rhs._mimeTypes;
	}
	return (*this);
}

Connection::~Connection(void)
{
	// std::cout << "Connection destructor called" << std::endl;
	this->closeClientSockets();
}

/* ************************************************************************** */
/*                          Private Member functions                          */
/* ************************************************************************** */

void Connection::initMime(void)
{
	this->_mimeTypes.insert(std::make_pair(".html", "text/html"));
	this->_mimeTypes.insert(std::make_pair(".css", "text/css"));
	this->_mimeTypes.insert(std::make_pair(".js", "application/javascript"));
	this->_mimeTypes.insert(std::make_pair(".pdf", "application/pdf"));
	this->_mimeTypes.insert(std::make_pair(".odt", "application/vnd.oasis.opendocument.text"));
	this->_mimeTypes.insert(std::make_pair(".rtf", "application/rtf"));
	this->_mimeTypes.insert(std::make_pair(".jpg", "image/jpeg"));
	this->_mimeTypes.insert(std::make_pair(".webp", "image/webp"));
	this->_mimeTypes.insert(std::make_pair(".png", "image/png"));
	this->_mimeTypes.insert(std::make_pair(".gif", "image/gif"));
	this->_mimeTypes.insert(std::make_pair(".mp4", "video/mp4"));
	this->_mimeTypes.insert(std::make_pair(".ico", "image/vnd.microsoft.icon"));
}

void Connection::addToFdSet(int fd, fd_set &fds)
{
	FD_SET(fd, &fds);
	if (fd > this->_highestFd)
		this->_highestFd = fd;
}

void Connection::checkFdStatus(void)
{
	int selectReady = select(this->_highestFd + 1, &this->_setReads, &this->_setWrite, &this->_setErrors, &this->_timeout);
	if (selectReady == -1) {
		throw (std::runtime_error("select() failed"));
	}
	else if (selectReady == 0) {
		// printError("select() timeout");
		for (std::vector<Client>::iterator it = this->_client.begin(); it != this->_client.end(); it++) {
			(*it)._isAlive = false;
		}
	}
}

bool Connection::isAlive(Client &client, bool isAlive)
{
	if (isAlive) {
		return (true);
	}

	FD_CLR(client._socketFd, &this->_setReads);
	FD_CLR(client._socketFd, &this->_setWrite);
	FD_CLR(client._socketFd, &this->_setErrors);

	shutdown(client._socketFd, SHUT_RDWR);
	close(client._socketFd);

	// std::cout << "> Client disconnected on socket " << client._socketFd << std::endl;;
	return (false);
}

void Connection::closeClientSockets()
{
	for (std::vector<Client>::iterator it = this->_client.begin(); it != this->_client.end(); it++) {
		std::cout << "> Closing client sockets" << std::endl;
		close(it->_socketFd);
	}
}

bool Connection::receiveClientRequest(Client &client)
{
	int maxReadBytes = 1024;
	char buffer[maxReadBytes];
	int readBytes = recv(client._socketFd, buffer, maxReadBytes, 0);
	if (readBytes <= 0) {
		if (readBytes == -1) {
			printError("recv() failed");
			sendErrorResponse(client, 500);
		}
		client._isAlive = false;
		return (false);
	}

	if (readBytes > maxReadBytes) {
		printError("Request size exceeds the limit");
		sendErrorResponse(client, 413);
		return (false);
	}

	if (client._contentLenght == 0 && client._requestStr.str().empty() == 0) {
		client._requestStr.str(std::string());
		client._bodyReq.str(std::string());
	}

	std::cout << CLR_BLUEB << buffer << CLR_RESET << std::endl;
	client._requestStr.write(buffer, readBytes);
	if (client._contentLenght == 0) {
		HTTPRequest httpRequest(client);
		if (client._method != POST) {
			return (true);
		}
	} else {
		client._bodyReq.write(buffer, readBytes);
		client._bodySize += readBytes;
	}
	
   	if (client._bodySize < client._contentLenght) {
		return (false);
	}
   	return (true);
}

bool Connection::handleReponse(Client &client)
{
	bool ret = true;
	switch (client._method)
	{
		case GET:
			ret = handleGET(client);
			break;
		case POST:
			handlePOST(client);
			break;
		case DELETE:
			handleDELETE(client);
			break;
		default:
			printError("Method not allowed");
			sendErrorResponse(client, 405);
			break;
	}
	return (ret);
}

/* ************************************************************************** */
/*                          Public Member functions                           */
/* ************************************************************************** */

void Connection::initConnection(void)
{
	FD_ZERO(&this->_setReads);
	FD_ZERO(&this->_setWrite);
	FD_ZERO(&this->_setErrors);

	for (std::vector<Server *>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++) {
		addToFdSet((*it)->getSocket(), this->_setReads);
	}

	for (std::vector<Client>::iterator it = this->_client.begin(); it != this->_client.end(); it++) {
		if (!((*it)._requestPars)) {
			addToFdSet(it->_socketFd, this->_setReads);
		} else {
			addToFdSet(it->_socketFd, this->_setWrite);
		}
		addToFdSet(it->_socketFd, this->_setErrors);
	}
	this->checkFdStatus();
}

void Connection::acceptSockets(void)
{
	for (std::vector<Server *>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++)
	{
		if (FD_ISSET((*it)->getSocket(), &this->_setReads))
		{
			Client newClient((*it)->getConfig(), (*it)->getServer(), (*it)->getLocation());
			std::memset(&newClient._socketAddress, 0, sizeof(newClient._socketAddress));
			newClient._socketAddrLen = sizeof(newClient._socketAddress);
			newClient._socketFd = accept((*it)->getSocket(), (sockaddr*)&newClient._socketAddress, &newClient._socketAddrLen);
			if (newClient._socketFd == -1) {
				throw (std::runtime_error("accept() failed"));
			}
			if (fcntl(newClient._socketFd, F_SETFL, O_NONBLOCK) < 0) {
				throw (std::runtime_error("fcntl() failed"));
			}
			// std::cout << "> New connection on socket " << newClient._socketFd << " on port " << (*it)->getPort() << std::endl;
			this->_client.push_back(newClient);
		}
	}
}

void Connection::traitement(void)
{
	for (std::vector<Client>::iterator it = this->_client.begin(); it < this->_client.end(); it++)
	{
		if (FD_ISSET(it->_socketFd, &this->_setErrors)) {
			it->_isAlive = false;
		}

		if (FD_ISSET(it->_socketFd, &this->_setReads)) {
			if (this->receiveClientRequest(*it)) {
				it->_requestPars = true;
				FD_SET(it->_socketFd, &this->_setWrite);
			}
		}

		if (FD_ISSET(it->_socketFd, &this->_setWrite)) {
			if (this->handleReponse(*it)) {
				it->_isAlive = false;
			}
		}

		if (!isAlive(*it, it->_isAlive)) {
			it = this->_client.erase(it);
		}
	}
}

bool Connection::handleGET(Client& client)
{
	if (hanglGetLocation(client)) {
		return (true);
	}
	
	if (client._uri.length() >= 100) {
		printHttpError("Request-URI Too Long", 414);
		sendErrorResponse(client, 414);
		return (true);
	}

	if (client._sizeRep == 0) {
		client._filePath = getFilePath(client);
		std::ifstream file(client._filePath, std::ios::in | std::ios::binary);
		if (!file.is_open()) {
			printHttpError("Not Found", 404);
			sendErrorResponse(client, 404);
		} else {
			std::string content, line;
			while (std::getline(file, line)) {
				content += line;
				if (!file.eof()) {
					content += "\n";
				}
			}
			client._bodyRep = content;
			client._sizeRep = 0;
			client._sizeSend = 0;
			file.close();
			createHttpResponse(client, 200, getMimeType(client._filePath));
		}
	}

	sendHttpResponse(client);
	if (client._sizeSend < client._sizeRep)
		return (false);
	return (true);
}

bool Connection::hanglGetLocation(Client &client)
{
	//Location and GET
	Location *location = findLocationForUri(client._uri, client._location);
	if (location)
	{
		if (!(location->getCgiPath().empty()))
		{
			executeCGI(client, location);
			return (true);
		}

		
		if (!(location->isMethodAllowed("GET")))
		{
			std::cerr << "\033[0;31mError : 405 Method Not Allowed from client:\033[0m " << client._socketFd << std::endl;
			sendErrorResponse(client, 405);
			return (true);
		}

		if (location->getAutoIndex())
		{
			std::cout << location->getPath();
			startAutoIndex(client, location->getPath());
			return (true);
		}
		std::string filePath = getFilePath(client, location);
		std::cout << "path : " << filePath << std::endl;
		std::ifstream file(filePath, std::ios::in | std::ios::binary);
		
		if (file.is_open() && location->getReturn().empty())
		{
			std::string line;
			while (getline(file, line))
    		{
				client._bodyRep.append(line + "\n");
    		    line.clear();
    		}
			createHttpResponse(client, 200, getMimeType(filePath));
			sendHttpResponse(client);
		}
		else if (!(location->getReturn().empty()))
		{
			client._response.append("HTTP/1.1 301 Moved Permanently\r\n");
			client._response.append("Location: " + location->getReturn() + "\r\n\r\n");
			client._sizeRep = client._response.size();
			sendHttpResponse(client);
		}
		else
		{
			std::cerr << "\033[0;31mError : 404 Not Found from client:\033[0m " << client._socketFd << std::endl;
			sendErrorResponse(client, 404);
		}
		
		file.close();
		return (true);
	}
	return (false);
}

void Connection::handlePOST(Client& client)
{
	// Vérification si l'URI correspond à une configuration de location
	Location *location = findLocationForUri(client._uri, client._location);
	if (!location || !location->isMethodAllowed("POST"))
	{
		std::cerr << "\033[0;31mError : 405 Method Not Allowed from client:\033[0m " << client._socketFd << std::endl;
		sendErrorResponse(client, 405);
		return;
	}
	
	// Si pas de cgiPath, on upload les données
	if (location->getCgiPath().empty())
	{
		//Recuperaton du Boundary
		std::string boundaryHeader = "Content-Type";
		std::string boundary = client._headers.find(boundaryHeader)->second.erase(0, std::strlen("multipart/form-data; boundary="));
		if (boundary.empty())
		{
			sendErrorResponse(client, 400);
			return;
		}

		//Recuperation du filename
		std::string 			filePartHeader = "filename=\"";
		std::string::size_type	filePartHeaderPos = client._requestStr.str().find(filePartHeader);
		if (filePartHeaderPos == std::string::npos)
		{
			sendErrorResponse(client, 400);
			return;
		}
		std::string::size_type startPos = filePartHeaderPos + filePartHeader.length();
		std::string::size_type endPos = client._requestStr.str().find("\"", startPos);
		std::string filename = client._requestStr.str().substr(startPos, endPos - startPos);
	
		//Check du content-type
		std::string contentTypeHeader = "Content-Type: ";
		startPos = client._requestStr.str().find(contentTypeHeader, endPos);
		if (startPos == std::string::npos)
		{
			sendErrorResponse(client, 400);
			return;
		}
		startPos += contentTypeHeader.length();
		endPos = client._requestStr.str().find("\r\n", startPos);
		std::string contentType = client._requestStr.str().substr(startPos, endPos - startPos);
		for (std::map<std::string, std::string>::iterator it = _mimeTypes.begin(); (*it).second == contentType; it++)
		{
			if (it == _mimeTypes.end())
			{
				sendErrorResponse(client, 400);
				return;
			}
		}

		// Trouver le début des données du fichier
		std::string body(client._bodyReq.str());
		startPos = body.find("\r\n\r\n");
		startPos += 4;

		// Trouver la fin des données du fichier
		boundary.replace(boundary.length(), 1, "--");
		endPos = body.find(boundary, startPos);
		endPos -= 4;
		if (endPos == std::string::npos)
		{
			sendErrorResponse(client, 400);
			return;
		}
		std::string fileData = body.substr(startPos, endPos - startPos);

		std::fstream outFile;
		outFile.open(location->getRoot() + "/" + filename, std::ios::binary | std::ios::out);
		if (outFile.is_open())
		{
			outFile << fileData;
			client._bodyRep = "Successfully created file";
			createHttpResponse(client, 201, "text/html");
			sendHttpResponse(client);
		}
		else 
		{
			sendErrorResponse(client, 400);
			return;
		}
		outFile.close();
	}
	else {
		executeCGI(client, location);
	}
}

void Connection::handleDELETE(Client& client)
{
	// Vérification si l'URI correspond à une configuration de location
	std::cout << "uri: " << client._uri << std::endl;
	Location *location = findLocationForUri(client._uri, client._location);
	if (!location || !location->isMethodAllowed("DELETE"))
	{
		std::cerr << "\033[0;31mError : 405 Method Not Allowed from client:\033[0m " << client._socketFd << std::endl;
		sendErrorResponse(client, 405);
		return;
	}

	//On recupere le nom du fichier dans la rquete
	std::istringstream	requestStream(client._requestStr.str());
	std::string			fileName;
	std::string			line;
	while (std::getline(requestStream, line))
	{
		size_t separator = line.find("\"file\":");
		if (separator != std::string::npos)
			fileName = line.substr(separator + 8, line.size() - 11);
	}
	
	//On supprime le fichier
	if (!(std::remove((location->getRoot() + "/" + fileName).c_str())))
	{
		client._bodyRep = fileName + " deleted";
		createHttpResponse(client, 200, "text/html");
		sendHttpResponse(client);
	}
}

std::string Connection::getMimeType(const std::string& filePath)
{
   	std::string fileExtension = filePath.substr(filePath.find_last_of('.'));
   	if (_mimeTypes.find(fileExtension) != _mimeTypes.end())
		return _mimeTypes[fileExtension];
	else
		return "application/octet-stream"; // Type MIME par défaut ?? ou "text/plain"
}

std::string Connection::getFilePath(const Client &client)
{
	std::string basePath = client._config.getRoot();
	std::string filePath = basePath + client._uri;
	if (filePath.back() == '/')
		filePath += client._config.getIndex();
	return filePath;
}

std::string Connection::getFilePath(const Client &client,const Location *location)
{
	std::string basePath = location->getRoot();
	std::string filePath = client._server.getRoot() + basePath + "/" + location->getIndex();
	return filePath;
}

Location *Connection::findLocationForUri(const std::string& uri, const std::vector<Location>& locations)
{
	for (size_t i = 0; i < locations.size(); i++)
	{
		if (uri.find(locations[i].getUrl()) == 0)
			return (const_cast<Location*>(&locations[i]));
	}
	return (nullptr);
}

void Connection::executeCGI(Client &client, Location *location)
{
	Cgi cgi(client, location);
	char **argv = cgi.arg(client);
	char **env = cgi.getenv();
	//char **argv = cgi.arg(client);

	int cgiInput[2];  // Pipe envoyer les données POST au script CGI
	int cgiOutput[2]; // Pipe pour lire la sortie du script CGI

	if (pipe(cgiInput) < 0 || pipe(cgiOutput) < 0)
	{
		sendErrorResponse(client, 500);
		return;
	}

	pid_t pid = fork();
	if (pid < 0)
	{
		sendErrorResponse(client, 500);
		return;
	}
	if (pid == 0)
	{
		signal(SIGPIPE, signalHandler);
		close(cgiInput[1]);
		close(cgiOutput[0]);

		dup2(cgiInput[0], STDIN_FILENO);
		dup2(cgiOutput[1], STDOUT_FILENO);

		if (execve(argv[0], argv, env) == -1)
		{
			sendErrorResponse(client, 500);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		close(cgiInput[0]);
		close(cgiOutput[1]);

		write(cgiInput[1], client._requestStr.str().c_str(), client._requestStr.str().length());
		close(cgiInput[1]);

		// Lire la sortie du script CGI depuis le pipe de sortie
		char buffer[4096];
		ssize_t bytesRead;
		while ((bytesRead = read(cgiOutput[0], buffer, sizeof(buffer) - 1)) > 0)
		{
			buffer[bytesRead] = '\0';
			client._bodyRep += buffer;
		}
		close(cgiOutput[0]);

		int status;
		waitpid(pid, &status, 0);

  
		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		{
			createHttpResponse(client, 200, "text/html");
			sendHttpResponse(client);
		}
		else
			sendErrorResponse(client, 500);
	}
}
