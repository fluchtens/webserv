/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 11:42:21 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/25 15:45:08 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTP.hpp"
#include "Utils.hpp"

void createHttpResponse(Client &client, int statusCode, const std::string &contentType)
{
	std::string response, statusMessage;

	switch (statusCode)
	{
		case 200:
			statusMessage = "OK";
			break;
		case 201:
			statusMessage = "Created";
			break;
		case 204:
			statusMessage = "No Content";
			break;
		default:
			statusMessage = "Unknown Status";
			break ;
	}

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

void createHttpRedirResponse(Client &client, Location *location)
{
	std::string response = "HTTP/1.1 301 Moved Permanently\r\n";
	response += "Location: " + location->getReturn() + "\r\n";
	response += "\r\n";
	client._respSize = response.size();
	client._response = response;
	std::cout << "resp: " << client._response << std::endl;
}

void sendHttpResponse(Client &client)
{
	int err = 0;
	socklen_t errLen = sizeof(err);
	if (getsockopt(client._socketFd, SOL_SOCKET, SO_SNDBUF, &err, &errLen) == -1) {
		printError("getsockopt() failed");
		sendHttpErrorResponse(client, 500);
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

static const std::string getErrorMessage(int &errorCode)
{
	std::string errorMessage;

	switch (errorCode)
	{
		case 400:
			errorMessage = "Bad Request";
			break;
		case 404:
			errorMessage = "Not Found";
			break;
		case 405:
			errorMessage = "Method Not Allowed";
			break;
		case 413:
			errorMessage = "Payload Too Large";
			break;
		case 414:
			errorMessage = "URI Too Long";
			break;
		case 500:
			errorMessage = "Internal Server Error";
			break;
		default:
			errorMessage = "Unknown Error";
			break ;
	}
	return (errorMessage);
}

void sendHttpErrorResponse(Client &client, int errorCode)
{
	std::string errorMessage, response, htmlContent;
	std::string errorPagePath, errorPageContent;

	errorMessage = getErrorMessage(errorCode);

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
		htmlContent += "\t<h1>Error " + std::to_string(errorCode) + ": " + errorMessage + "</h1>\r\n";
	} else {
		std::ifstream file(client._config.getRoot() + errorPagePath);
		if (!file.is_open()) {
			htmlContent += "\t<h1>Error " + std::to_string(errorCode) + ": " + errorMessage + "</h1>\r\n";
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
