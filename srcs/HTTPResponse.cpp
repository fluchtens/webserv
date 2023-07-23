/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 11:42:21 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/23 13:28:33 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

void createHttpResponse(Client &client, int statusCode, const std::string &contentType)
{
    std::string response;
    std::string statusMessage;

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
    }

    response.append("HTTP/1.1 " + std::to_string(statusCode) + " " + statusMessage + "\r\n");
    if(client._cookie.empty())
		response.append("Set-Cookie: delicieux_cookie=choco\r\n");
    response.append("Content-Type: " + contentType + "\r\n");
    response.append("Content-Length: " + std::to_string(client._bodyRep.size()) + "\r\n");
    response.append("Connection: Closed\r\n");
    response.append("\r\n");
    response.append(client._bodyRep);

    client._sizeRep = response.size();

    //On enregistre l'ensemble de la reponse dans le client pour l'envoyer en plusieurs fois.
    client._response = response;
}

void sendHttpResponse(Client &client)
{
    int optval = 0;
    socklen_t  optlen = sizeof(optval);
    if(getsockopt(client._socketFd, SOL_SOCKET, SO_SNDBUF, &optval, &optlen) == -1 || optval <= 0)
    {
        std::cerr << "Error : 500 sending data to client getsockopt(): " << client._socketFd << std::endl;
        sendErrorResponse(client, 500);
        client._isAlive = false;
        return;
    }
    
    ssize_t remainingSize = client._sizeRep - client._sizeSend;
    
    const char* bodyData = client._response.data() + client._sizeSend;
    ssize_t bodySize = std::min(remainingSize, static_cast<ssize_t>(optval));
    std::string response(bodyData, bodySize);

    ssize_t sentBytes = send(client._socketFd, response.data(), response.size(), 0);
    client._sizeSend += sentBytes;
    if (sentBytes == 0)
    {
        std::cerr << "\033[31mErreur send90 n'a pas envoye de donnee client : \033[0m" << client._socketFd << std::endl;
        client._isAlive = false;
    }
    else if (sentBytes == -1)
    {
        std::cerr << "\033[31mErreur de send() client : \00[0m" << client._socketFd << std::endl;
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

void sendErrorResponse(Client &client, int code)
{
    std::string errorMessage, response, htmlContent;
    std::string errorPagePath, errorPageContent;

    errorMessage = getErrorMessage(code);

    htmlContent += "<!DOCTYPE html>\r\n";
    htmlContent += "<html lang=\"en\">\r\n";
    htmlContent += "<head>\r\n";
    htmlContent += "\t<meta charset=\"UTF-8\">\r\n";
    htmlContent += "\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\r\n";
    htmlContent += "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n";
    htmlContent += "\t<title>" + std::to_string(code) + " " + errorMessage + "</title>\r\n";
    htmlContent += "</head>\r\n";
    htmlContent += "<body>\r\n";

    errorPagePath = client._config.getErrorPage(code);
    if (errorPagePath.empty()) {
        htmlContent += "\t<h1>Error " + std::to_string(code) + ": " + errorMessage + "</h1>\r\n";
    } else {
        std::ifstream file(client._config.getRoot() + errorPagePath);
        if (!file.is_open()) {
            htmlContent += "\t<h1>Error " + std::to_string(code) + ": " + errorMessage + "</h1>\r\n";
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

    response = "HTTP/1.1 " + std::to_string(code) + " " + errorMessage + "\r\n";
    response += "Content-Type: text/html\r\n";
    if (errorPagePath.empty()) {
        response += "Content-Length: " + std::to_string(htmlContent.length()) + "\r\n\r\n";
        response += htmlContent;
    } else {
        response += "Content-Length: " + std::to_string(errorPageContent.length()) + "\r\n\r\n";
        response += errorPageContent;
    }

    if (send(client._socketFd, response.c_str(), response.length(), 0) == -1) {
        std::cerr << "\033[31Error : sendErrorResponse() lors de l'envoi de la réponse d'erreur\033[0m" << std::endl;
        client._isAlive = false;
    }
}
