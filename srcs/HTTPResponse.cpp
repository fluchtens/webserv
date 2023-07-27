/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 11:42:21 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/27 12:53:20 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include "Utils.hpp"

/* ************************************************************************** */
/*                           Constructor Destructor                           */
/* ************************************************************************** */

HttpResponse::HttpResponse(void)
{
	// std::cout << "HttpResponse default constructor called" << std::endl;
	this->initHttpCodes();
}

HttpResponse::~HttpResponse(void)
{
	// std::cout << "HttpResponse destructor called" << std::endl;
}

/* ************************************************************************** */
/*                             Creating Responses                             */
/* ************************************************************************** */

void HttpResponse::create(Client &client, int statusCode, const std::string &contentType)
{
	std::string response, statusMessage;
	statusMessage = this->getHttpMessage(statusCode);

	response = "HTTP/1.1 " + std::to_string(statusCode) + " " + statusMessage + "\r\n";
	if (client._cookie.empty()) {
		response += "Set-Cookie: delicieux_cookie=choco\r\n";
	}
	response += "Content-Type: " + contentType + "\r\n";
	response += "Content-Length: " + std::to_string(client._bodyResp.size()) + "\r\n";
	response += "\r\n";
	response += client._bodyResp;
	client._respSize = response.size();
	client._response = response;
}

void HttpResponse::createRedirection(Client &client, Location *location)
{
	std::string response = "HTTP/1.1 301 Moved Permanently\r\n";
	response += "Location: " + location->getReturn() + "\r\n";
	response += "\r\n";
	client._respSize = response.size();
	client._response = response;
}

void HttpResponse::createAutoIndex(Client &client, std::string path)
{
	DIR *dir = opendir(path.c_str());
	if (!dir) {
		printHttpError("Not Found", 404);
		this->sendError(client, 404);
		return ;
	}

	std::string relativePath = path;
	std::string root = client._server.getRoot();
	size_t pos = relativePath.find(root);
	if (pos == 0) {
        relativePath.erase(0, root.length());
		if (path[0] == '/') {
            path.erase(0, 1);
        }
	}

	std::string htmlContent = "<!DOCTYPE html>\r\n";
	htmlContent += "<html lang=\"en\">\r\n";
	htmlContent += "<head>\r\n";
	htmlContent += "\t<meta charset=\"UTF-8\">\r\n";
	htmlContent += "\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\r\n";
	htmlContent += "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n";
	htmlContent += "\t<title>Index of " + path + "</title>\r\n";
	htmlContent += "</head>\r\n";
	htmlContent += "<body>\r\n";
	htmlContent += "\t<h1>Index of " + path + "</h1>\r\n";
	htmlContent += "\t<ul>\r\n";

	struct dirent *ent;
	while ((ent = readdir(dir))) {
		std::string name = std::string(ent->d_name);
		htmlContent += "\t<li><a href=\"" + relativePath + "/" + name + "\">" + name + "</a></li>\r\n";
	}

	htmlContent += "\t</ul>\r\n";
	htmlContent += "</body>\r\n";
	htmlContent += "</html>";

	closedir(dir);
	client._bodyResp = htmlContent;
	this->create(client, 200, "text/html");
	this->sendResponse(client);
}

/* ************************************************************************** */
/*                             Sending Responses                             */
/* ************************************************************************** */

void HttpResponse::sendResponse(Client &client)
{
	int err = 0;
	socklen_t errLen = sizeof(err);
	if (getsockopt(client._socketFd, SOL_SOCKET, SO_SNDBUF, &err, &errLen) == -1) {
		printError("getsockopt() failed");
		this->sendError(client, 500);
		client._isAlive = false;
		return ;
	}

	ssize_t sentBytes = send(client._socketFd, client._response.c_str(), client._respSize, 0);
	if (sentBytes == -1) {
		printError("send() failed");
		client._isAlive = false;
	}
	else if (sentBytes == 0) {
		printError("send() no data sent");
		client._isAlive = false;
	}
}

void HttpResponse::sendError(Client &client, int errorCode)
{
	std::string errorMessage, response, htmlContent;
	std::string errorPagePath, errorPageContent;

	errorMessage = this->getHttpMessage(errorCode);

	htmlContent += "<!DOCTYPE html>\r\n";
	htmlContent += "<html lang=\"en\">\r\n";
	htmlContent += "<head>\r\n";
	htmlContent += "\t<meta charset=\"UTF-8\">\r\n";
	htmlContent += "\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\r\n";
	htmlContent += "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n";
	htmlContent += "\t<title>" + std::to_string(errorCode) + " " + errorMessage + "</title>\r\n";
	htmlContent += "</head>\r\n";
	htmlContent += "<body>\r\n";

	errorPagePath = client._config.getErrorPage(errorCode);
	if (errorPagePath.empty()) {
		htmlContent += "\t<center><h1>" + std::to_string(errorCode) + " " + errorMessage + "</h1></center>\r\n";
	} else {
		std::ifstream file(client._config.getRoot() + errorPagePath);
		if (!file.is_open()) {
			htmlContent += "\t<center><h1>" + std::to_string(errorCode) + " " + errorMessage + "</h1></center>\r\n";
		} else {
			std::string line;
			while (std::getline(file, line)) {
				errorPageContent += line;
				if (!file.eof()) {
					errorPageContent += "\n";
				}
			}
			file.close();
		}
	}
	htmlContent += "\t<hr><center>webserv</center>\r\n";
	htmlContent += "</body>\r\n";
	htmlContent += "</html>";

	response = "HTTP/1.1 " + std::to_string(errorCode) + " " + errorMessage + "\r\n";
	response += "Content-Type: text/html\r\n";
	if (errorPagePath.empty()) {
		response += "Content-Length: " + std::to_string(htmlContent.length()) + "\r\n\r\n";
		response += htmlContent;
	} else {
		response += "Content-Length: " + std::to_string(errorPageContent.length()) + "\r\n\r\n";
		response += errorPageContent;
	}

	if (send(client._socketFd, response.c_str(), response.length(), 0) == -1) {
		printError("send() failed");
		client._isAlive = false;
	}
}

/* ************************************************************************** */
/*                                    Utils                                   */
/* ************************************************************************** */

void HttpResponse::initHttpCodes(void)
{
	this->_httpCodes.insert(std::make_pair(200, "OK"));
	this->_httpCodes.insert(std::make_pair(201, "Created"));
	this->_httpCodes.insert(std::make_pair(202, "Accepted"));
	this->_httpCodes.insert(std::make_pair(203, "Non-Authoritative Information"));
	this->_httpCodes.insert(std::make_pair(204, "No Content"));
	this->_httpCodes.insert(std::make_pair(205, "Reset Content"));
	this->_httpCodes.insert(std::make_pair(206, "Partial Content"));
	this->_httpCodes.insert(std::make_pair(207, "Multi-Status"));
	this->_httpCodes.insert(std::make_pair(208, "Already Reported"));
	this->_httpCodes.insert(std::make_pair(226, "IM Used"));
	this->_httpCodes.insert(std::make_pair(400, "Bad Request"));
	this->_httpCodes.insert(std::make_pair(401, "Unauthorized"));
	this->_httpCodes.insert(std::make_pair(402, "Payment Required"));
	this->_httpCodes.insert(std::make_pair(403, "Forbidden"));
	this->_httpCodes.insert(std::make_pair(404, "Not Found"));
	this->_httpCodes.insert(std::make_pair(405, "Method Not Allowed"));
	this->_httpCodes.insert(std::make_pair(406, "Not Acceptable"));
	this->_httpCodes.insert(std::make_pair(407, "Proxy Authentication Required"));
	this->_httpCodes.insert(std::make_pair(408, "Request Timeout"));
	this->_httpCodes.insert(std::make_pair(409, "Conflict"));
	this->_httpCodes.insert(std::make_pair(410, "Gone"));
	this->_httpCodes.insert(std::make_pair(411, "Length Required"));
	this->_httpCodes.insert(std::make_pair(412, "Precondition Failed"));
	this->_httpCodes.insert(std::make_pair(413, "Payload Too Large"));
	this->_httpCodes.insert(std::make_pair(414, "URI Too Long"));
	this->_httpCodes.insert(std::make_pair(415, "Unsupported Media Type"));
	this->_httpCodes.insert(std::make_pair(416, "Range Not Satisfiable"));
	this->_httpCodes.insert(std::make_pair(417, "Expectation Failed"));
	this->_httpCodes.insert(std::make_pair(418, "I'm a teapot"));
	this->_httpCodes.insert(std::make_pair(421, "Misdirected Request"));
	this->_httpCodes.insert(std::make_pair(422, "Unprocessable Entity"));
	this->_httpCodes.insert(std::make_pair(423, "Locked"));
	this->_httpCodes.insert(std::make_pair(424, "Failed Dependency"));
	this->_httpCodes.insert(std::make_pair(425, "Too Early"));
	this->_httpCodes.insert(std::make_pair(426, "Upgrade Required"));
	this->_httpCodes.insert(std::make_pair(428, "Precondition Required"));
	this->_httpCodes.insert(std::make_pair(429, "Too Many Requests"));
	this->_httpCodes.insert(std::make_pair(431, "Request Header Fields Too Large"));
	this->_httpCodes.insert(std::make_pair(451, "Unavailable For Legal Reasons"));
	this->_httpCodes.insert(std::make_pair(500, "Internal Server Error"));
	this->_httpCodes.insert(std::make_pair(501, "Not Implemented"));
	this->_httpCodes.insert(std::make_pair(502, "Bad Gateway"));
	this->_httpCodes.insert(std::make_pair(503, "Service Unavailable"));
	this->_httpCodes.insert(std::make_pair(504, "Gateway Timeout"));
	this->_httpCodes.insert(std::make_pair(505, "HTTP Version Not Supported"));
	this->_httpCodes.insert(std::make_pair(506, "Variant Also Negotiates"));
	this->_httpCodes.insert(std::make_pair(507, "Insufficient Storage"));
	this->_httpCodes.insert(std::make_pair(508, "Loop Detected"));
	this->_httpCodes.insert(std::make_pair(510, "Not Extended"));
	this->_httpCodes.insert(std::make_pair(511, "Network Authentication Required"));
}

std::string HttpResponse::getHttpMessage(int errorCode) const
{
	std::map<int, std::string>::const_iterator it = this->_httpCodes.find(errorCode);
	if (it != this->_httpCodes.end()) {
		return (it->second);
	}
	return ("");
}
