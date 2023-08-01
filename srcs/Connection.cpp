/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 08:33:19 by fluchten          #+#    #+#             */
/*   Updated: 2023/08/01 14:30:19 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Connection.hpp"
#include "Utils.hpp"

/* ************************************************************************** */
/*                           Constructor Destructor                           */
/* ************************************************************************** */

Connection::Connection(std::vector<Server*>& servers) : _servers(servers)
{
	// std::cout << "Connection constructor called" << std::endl;
	this->_highestFd = 0;
	this->_timeout.tv_sec = 3;
	this->_timeout.tv_usec = 0;
}

Connection::~Connection(void)
{
	// std::cout << "Connection destructor called" << std::endl;
	this->closeClientSockets();
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
			if (this->parseClientRequest(*it)) {
				it->_requestPars = true;
				FD_SET(it->_socketFd, &this->_setWrite);
			}
		}

		if (FD_ISSET(it->_socketFd, &this->_setWrite)) {
			if (this->handleReponse(*it)) {
				it->_isAlive = false;
			}
		}

		if (!this->isAlive(*it, it->_isAlive)) {
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

bool Connection::parseClientRequest(Client &client)
{
	int maxReadBytes = 1024;
	char buffer[maxReadBytes];
	int readBytes = recv(client._socketFd, buffer, maxReadBytes, 0);
	if (readBytes <= 0) {
		if (readBytes == -1) {
			printError("recv() failed");
			this->_httpResponse.sendError(client, 500);
		}
		client._isAlive = false;
		return (false);
	}

	if (readBytes > maxReadBytes) {
		printError("Request size exceeds the limit");
		this->_httpResponse.sendError(client, 413);
		return (false);
	}

	if (client._contentLenght == 0 && client._requestStr.str().empty() == 0) {
		client._requestStr.str(std::string());
		client._bodyReq.str(std::string());
	}

	// std::cout << CLR_BLUEB << buffer << CLR_RESET << std::endl;
	client._requestStr.write(buffer, readBytes);
	if (client._contentLenght == 0) {
		this->_httpRequest.parse(client);
		if (!client._validHost) {
			printHttpError("Forbidden", 403);
			this->_httpResponse.sendError(client, 403);
			return (false);
		}
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

/* ************************************************************************** */
/*                                  Responses                                 */
/* ************************************************************************** */

bool Connection::handleReponse(Client &client)
{
	bool ret = true;
	switch (client._method)
	{
		case GET:
			ret = this->getRequest(client);
			break;
		case POST:
			this->postRequest(client);
			break;
		case DELETE:
			this->deleteRequest(client);
			break;
		default:
			printError("Method not allowed");
			this->_httpResponse.sendError(client, 405);
			break;
	}
	return (ret);
}

bool Connection::getRequest(Client& client)
{
	if (this->getRequestLocation(client)) {
		return (true);
	}
	
	if (client._uri.length() >= 100) {
		printHttpError("Request-URI Too Long", 414);
		this->_httpResponse.sendError(client, 414);
		return (true);
	}

	if (client._respSize == 0) {
		client._filePath = this->getIndexPath(client);

		DIR *dir = opendir(client._filePath.c_str());
		if (dir) {
			struct dirent *ent;
			while ((ent = readdir(dir))) {
				if (ent->d_type == DT_DIR) {
					printHttpError("Forbidden", 403);
					this->_httpResponse.sendError(client, 403);
					return (true);
				}
			}
		}

		std::ifstream file(client._filePath);
		if (!file.is_open()) {
			printHttpError("Not Found", 404);
			this->_httpResponse.sendError(client, 404);
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
			this->_httpResponse.create(client, 200, this->_mimeTypes.getType(client._filePath));
			this->_httpResponse.sendResponse(client);
		}
	}
	return (true);
}

bool Connection::getRequestLocation(Client &client)
{
	Location *location = this->getLocation(client);
	if (!location) {
		return (false);
	}

	if (!location->isMethodAllowed("GET")) {
		printHttpError("GET Method Not Allowed", 405);
		this->_httpResponse.sendError(client, 405);
		return (true);
	}

	if (!location->getCgiPath().empty()) {
		this->executeCGI(client, location);
		return (true);
	}

	std::string filePath = this->getIndexPath(client, location);
	std::ifstream file(filePath);
	if (!file.is_open()) {
		if (location->getAutoIndex()) {
			this->_httpResponse.createAutoIndex(client, this->getAbsolutePath(client, location));
			return (true);
		}
		printHttpError("Forbidden", 403);
		this->_httpResponse.sendError(client, 403);
		return (true);
	}

	if (location->getReturn().empty()) {
		std::string content, line;
		while (std::getline(file, line)) {
			content += line;
			if (!file.eof()) {
				content += "\n";
			}
		}
		client._bodyResp = content;
		this->_httpResponse.create(client, 200, this->_mimeTypes.getType(filePath));
		this->_httpResponse.sendResponse(client);
	} else {
		this->_httpResponse.createRedirection(client, location);
		this->_httpResponse.sendResponse(client);
	}
	file.close();
	return (true);
}

void Connection::postRequest(Client& client)
{
	Location *location = this->getLocation(client);
	if (!location) {
		printHttpError("POST Method Not Allowed", 405);
		this->_httpResponse.sendError(client, 405);
		return ;
	}

	if (!location->isMethodAllowed("POST")) {
		printHttpError("POST Method Not Allowed", 405);
		this->_httpResponse.sendError(client, 405);
		return ;
	}

	if (!location->getCgiPath().empty()) {
		this->executeCGI(client, location);
		return ;
	}

	std::string request = client._requestStr.str();
	std::string body = client._bodyReq.str();

	std::string contentTypeHeader = client._headers["Content-Type"];
	size_t boundaryPos = contentTypeHeader.find("boundary=");
	if (boundaryPos == std::string::npos) {
		printHttpError("Bad Request", 400);
		this->_httpResponse.sendError(client, 400);
		return ;
	}
	boundaryPos += 9;
	std::string boundaryValue = contentTypeHeader.substr(boundaryPos);

	std::size_t fileStartPos, fileEndPos;
	fileStartPos = client._requestStr.str().find("filename=\"");
	if (fileStartPos == std::string::npos) {
		printHttpError("Bad Request", 400);
		this->_httpResponse.sendError(client, 400);
		return ;
	}
	fileStartPos += 10;
	fileEndPos = client._requestStr.str().find("\"", fileStartPos);
	std::string fileName = client._requestStr.str().substr(fileStartPos, fileEndPos - fileStartPos);

	std::size_t fileTypeStartPos, fileTypEndPos;
	fileTypeStartPos = client._requestStr.str().find("Content-Type: ", fileEndPos);
	if (fileTypeStartPos == std::string::npos) {
		printHttpError("Bad Request", 400);
		this->_httpResponse.sendError(client, 400);
		return ;
	}
	fileTypeStartPos += 14;
	fileTypEndPos = client._requestStr.str().find("\r\n", fileTypeStartPos);
	std::string contentType = client._requestStr.str().substr(fileTypeStartPos, fileTypEndPos - fileTypeStartPos);

	std::size_t bodyStartPos, bodyEndPos;
	bodyStartPos = body.find("\r\n\r\n");
	bodyStartPos += 4;

	bodyEndPos = body.find(boundaryValue + "--", bodyStartPos);
	bodyEndPos -= 4;
	if (bodyEndPos == std::string::npos) {
		printHttpError("Bad Request", 400);
		this->_httpResponse.sendError(client, 400);
		return ;
	}

	std::string filePath = this->getAbsolutePath(client, location) + "/" + fileName;
	std::string fileData = body.substr(bodyStartPos, bodyEndPos - bodyStartPos);
	std::ofstream outFile(filePath, std::ios::binary);
	if (!outFile.is_open()) {
		printHttpError("Bad Request", 400);
		this->_httpResponse.sendError(client, 400);
		return ;
	}
	outFile << fileData;
	client._bodyResp = "Successfully created file";
	this->_httpResponse.create(client, 201, "text/html");
	this->_httpResponse.sendResponse(client);
	outFile.close();
}

void Connection::deleteRequest(Client& client)
{
	Location *location = this->getLocation(client);
	if (!location) {
		printHttpError("DELETE Method Not Allowed", 405);
		this->_httpResponse.sendError(client, 405);
		return ;
	}

	if (!location->isMethodAllowed("DELETE")) {
		printHttpError("DELETE Method Not Allowed", 405);
		this->_httpResponse.sendError(client, 405);
		return ;
	}

	std::stringstream ss(client._requestStr.str());
	std::string line, fileName;
	while (std::getline(ss, line)) {
		if (line.find("{\"filename\":") != std::string::npos) {
			fileName = line.substr(13, line.length() - 13 - 2);
		}
	}

	std::string filePath = this->getAbsolutePath(client, location) + "/" + fileName;
	if (!std::remove(filePath.c_str())) {
		client._bodyResp = fileName + " successfully deleted.";
		this->_httpResponse.create(client, 200, "text/html");
		this->_httpResponse.sendResponse(client);
	}
}

/* ************************************************************************** */
/*                                     CGI                                    */
/* ************************************************************************** */

void Connection::executeCGI(Client &client, Location *location)
{
	Cgi cgi(client, location);

	char **av = cgi.getArgs();
	char **env = cgi.getEnv();

	int cgiInput[2];  // Pipe envoyer les données POST au script CGI
	int cgiOutput[2]; // Pipe pour lire la sortie du script CGI

	if (pipe(cgiInput) < 0 || pipe(cgiOutput) < 0)
	{
		this->_httpResponse.sendError(client, 500);
		return;
	}

	pid_t pid = fork();
	if (pid < 0)
	{
		this->_httpResponse.sendError(client, 500);
		return;
	}
	if (pid == 0)
	{
		signal(SIGPIPE, signalHandler);
		close(cgiInput[1]);
		close(cgiOutput[0]);

		dup2(cgiInput[0], STDIN_FILENO);
		dup2(cgiOutput[1], STDOUT_FILENO);

		if (execve(av[0], av, env) == -1)
		{
			this->_httpResponse.sendError(client, 500);
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
			this->_httpResponse.create(client, 200, "text/html");
			this->_httpResponse.sendResponse(client);
		}
		else
			this->_httpResponse.sendError(client, 500);
	}
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

std::string Connection::getIndexPath(Client &client)
{
	std::string serverRoot = client._config.getRoot();
	std::string index = serverRoot + client._uri;
	if (client._uri == "/") {
		index += client._config.getIndex();
	}
	return (index);
}

std::string Connection::getIndexPath(Client &client, Location *location)
{
	std::string serverRoot = client._config.getRoot();
	std::string locationRoot = location->getRoot();
	std::string index = serverRoot + locationRoot;
	if (locationRoot != "/") {
		index += "/";
	}
	if (!location->getIndex().empty()) {
		index += location->getIndex();
	} else {
		index += client._config.getIndex();
	}
	return (index);
}

std::string Connection::getAbsolutePath(Client &client, Location *location)
{
	std::string serverRoot = client._config.getRoot();
	std::string locationRoot = location->getRoot();
	std::string absolutePath = serverRoot + locationRoot;
	if (absolutePath.back() == '/') {
		absolutePath.pop_back();
	}
	return (absolutePath);
}


Location *Connection::getLocation(Client &client)
{
	for (size_t i = 0; i < client._location.size(); i++) {
		if (client._location[i].getUrl() == client._uri) {
			return &(client._location[i]);
		}
	}
	return (nullptr);
}
