/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 08:33:19 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/25 20:13:53 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "Utils.hpp"

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
	}
	return (*this);
}

Connection::~Connection(void)
{
	// std::cout << "Connection destructor called" << std::endl;
	this->closeClientSockets();
}

/* ************************************************************************** */
/*                          Public Member functions                           */
/* ************************************************************************** */

void Connection::handlePOST(Client& client)
{
	// Vérification si l'URI correspond à une configuration de location
	Location *location = findLocationForUri(client._uri, client._location);
	if (!location || !location->isMethodAllowed("POST"))
	{
		std::cerr << "\033[0;31mError : 405 Method Not Allowed from client:\033[0m " << client._socketFd << std::endl;
		sendHttpErrorResponse(client, 405);
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
			sendHttpErrorResponse(client, 400);
			return;
		}

		//Recuperation du filename
		std::string 			filePartHeader = "filename=\"";
		std::string::size_type	filePartHeaderPos = client._requestStr.str().find(filePartHeader);
		if (filePartHeaderPos == std::string::npos)
		{
			sendHttpErrorResponse(client, 400);
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
			sendHttpErrorResponse(client, 400);
			return;
		}
		startPos += contentTypeHeader.length();
		endPos = client._requestStr.str().find("\r\n", startPos);
		std::string contentType = client._requestStr.str().substr(startPos, endPos - startPos);
		// for (std::map<std::string, std::string>::iterator it = _mimeTypes.begin(); (*it).second == contentType; it++)
		// {
		// 	if (it == _mimeTypes.end())
		// 	{
		// 		sendHttpErrorResponse(client, 400);
		// 		return;
		// 	}
		// }

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
			sendHttpErrorResponse(client, 400);
			return;
		}
		std::string fileData = body.substr(startPos, endPos - startPos);

		std::fstream outFile;
		outFile.open(location->getRoot() + "/" + filename, std::ios::binary | std::ios::out);
		if (outFile.is_open())
		{
			outFile << fileData;
			client._bodyResp = "Successfully created file";
			createHttpResponse(client, 201, "text/html");
			sendHttpResponse(client);
		}
		else 
		{
			sendHttpErrorResponse(client, 400);
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
		sendHttpErrorResponse(client, 405);
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
		client._bodyResp = fileName + " deleted";
		createHttpResponse(client, 200, "text/html");
		sendHttpResponse(client);
	}
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
		sendHttpErrorResponse(client, 500);
		return;
	}

	pid_t pid = fork();
	if (pid < 0)
	{
		sendHttpErrorResponse(client, 500);
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
			sendHttpErrorResponse(client, 500);
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
			client._bodyResp += buffer;
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
			sendHttpErrorResponse(client, 500);
	}
}

/* ************************************************************************** */
/*                               Initialization                               */
/* ************************************************************************** */

void Connection::initConnection(void)
{
	FD_ZERO(&this->_setReads);
	FD_ZERO(&this->_setWrite);
	FD_ZERO(&this->_setErrors);

	for (std::vector<Server *>::iterator it = this->_servers.begin(); it != this->_servers.end(); it++) {
		this->addToFdSet((*it)->getSocket(), this->_setReads);
	}

	for (std::vector<Client>::iterator it = this->_client.begin(); it != this->_client.end(); it++) {
		if (!((*it)._requestPars)) {
			this->addToFdSet(it->_socketFd, this->_setReads);
		} else {
			this->addToFdSet(it->_socketFd, this->_setWrite);
		}
		this->addToFdSet(it->_socketFd, this->_setErrors);
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

void Connection::start(void)
{
	this->initConnection();
	this->acceptSockets();
	this->traitement();
}

/* ************************************************************************** */
/*                                  Requests                                  */
/* ************************************************************************** */

bool Connection::receiveClientRequest(Client &client)
{
	int maxReadBytes = 1024;
	char buffer[maxReadBytes];
	int readBytes = recv(client._socketFd, buffer, maxReadBytes, 0);
	if (readBytes <= 0) {
		if (readBytes == -1) {
			printError("recv() failed");
			sendHttpErrorResponse(client, 500);
		}
		client._isAlive = false;
		return (false);
	}

	if (readBytes > maxReadBytes) {
		printError("Request size exceeds the limit");
		sendHttpErrorResponse(client, 413);
		return (false);
	}

	if (client._contentLenght == 0 && client._requestStr.str().empty() == 0) {
		client._requestStr.str(std::string());
		client._bodyReq.str(std::string());
	}

	std::cout << CLR_BLUEB << buffer << CLR_RESET << std::endl;
	client._requestStr.write(buffer, readBytes);
	if (client._contentLenght == 0) {
		parseHttpRequest(client);
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
			ret = this->getRequest(client);
			break;
		case POST:
			handlePOST(client);
			break;
		case DELETE:
			handleDELETE(client);
			break;
		default:
			printError("Method not allowed");
			sendHttpErrorResponse(client, 405);
			break;
	}
	return (ret);
}

bool Connection::getRequest(Client& client)
{
	if (hanglGetLocation(client)) {
		return (true);
	}
	
	if (client._uri.length() >= 100) {
		printHttpError("Request-URI Too Long", 414);
		sendHttpErrorResponse(client, 414);
		return (true);
	}

	if (client._respSize == 0) {
		client._filePath = this->getFilePath(client);
		std::ifstream file(client._filePath);
		if (!file.is_open()) {
			printHttpError("Not Found", 404);
			sendHttpErrorResponse(client, 404);
		} else {
			std::string content, line;
			while (std::getline(file, line)) {
				content += line;
				if (!file.eof()) {
					content += "\n";
				}
			}
			client._bodyResp = content;
			client._respSize = 0;
			file.close();
			createHttpResponse(client, 200, this->_mimeTypes.getType(client._filePath));
			sendHttpResponse(client);
		}
	}
	return (true);
}

Location *Connection::findLocationForUri(const std::string& uri, const std::vector<Location>& locations)
{
	for (size_t i = 0; i < locations.size(); i++) {
		if (uri.find(locations[i].getUrl()) == 0) {
			return (const_cast<Location*>(&locations[i]));
		}
	}
	return (nullptr);
}

Location *findLocation(Client &client)
{
	for (size_t i = 0; i < client._location.size(); i++) {
		if (client._location[i].getUrl() == client._uri) {
			return &(client._location[i]);
		}
	}
	return (nullptr);
}

bool Connection::hanglGetLocation(Client &client)
{
	Location *location = findLocation(client);
	if (!location) {
		return (false);
	}

	if (!location->getCgiPath().empty()) {
		executeCGI(client, location);
		return (true);
	}

	if (!location->isMethodAllowed("GET")) {
		printHttpError("GET Method Not Allowed", 405);
		sendHttpErrorResponse(client, 405);
		return (true);
	}

	if (location->getAutoIndex()) {
		startAutoIndex(client, location->getPath());
		return (true);
	}

	std::string filePath = getFilePath(client, location);
	std::ifstream file(filePath);
	if (!file.is_open()) {
		printHttpError("Not Found", 404);
		sendHttpErrorResponse(client, 404);
	} else {
		if (location->getReturn().empty()) {
			std::string content, line;
			while (std::getline(file, line)) {
				content += line;
				if (!file.eof()) {
					content += "\n";
				}
			}
			client._bodyResp = content;
			createHttpResponse(client, 200, this->_mimeTypes.getType(filePath));
			sendHttpResponse(client);
		} else {
			createHttpRedirResponse(client, location);
			sendHttpResponse(client);
		}
	}
	file.close();
	return (true);
}

/* ************************************************************************** */
/*                                    Utils                                   */
/* ************************************************************************** */

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
	return (false);
}

void Connection::closeClientSockets()
{
	for (std::vector<Client>::iterator it = this->_client.begin(); it != this->_client.end(); it++) {
		std::cout << "> Closing client sockets" << std::endl;
		close(it->_socketFd);
	}
}

std::string Connection::getFilePath(Client &client)
{
	std::string root = client._config.getRoot();
	std::string path = root + client._uri;
	if (path.back() == '/') {
		path += client._config.getIndex();
	}
	return (path);
}

std::string Connection::getFilePath(Client &client, Location *location)
{
	std::string root = location->getRoot();
	std::string path = client._server.getRoot() + root;
	if (root != "/") {
		path += "/";
	}
	path += location->getIndex();
	return (path);
}
