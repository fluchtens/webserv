/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 10:28:19 by fluchten          #+#    #+#             */
/*   Updated: 2023/08/08 10:37:29 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"
#include "Utils.hpp"

/* ************************************************************************** */
/*                           Constructor Destructor                           */
/* ************************************************************************** */

HttpRequest::HttpRequest(void)
{
	// std::cout << "HttpRequest default constructor called" << std::endl;
}

HttpRequest::~HttpRequest(void)
{
	// std::cout << "HttpRequest destructor called" << std::endl;
}

/* ************************************************************************** */
/*                           Parsing Client Request                           */
/* ************************************************************************** */

void HttpRequest::parseRequestLine(Client &client, std::stringstream &requestStream)
{
	std::string line;

	if (std::getline(requestStream, line))
	{
		std::stringstream ss(line);
		std::string method;

		ss >> method;
		ss >> client._uri;
		ss >> client._httpVersion;

		if (method == "GET") {
			client._method = GET;
		}
		else if (method == "POST") {
			client._method = POST;
		}
		else if (method == "DELETE") {
			client._method = DELETE;
		}
		else {
			client._method = UNKNOWN;
		}

		std::string::size_type pos = client._uri.find("?");
		if (pos != std::string::npos) {
			client._query = client._uri.substr(pos);
			client._uri.erase(pos);
		}
	}
}

void HttpRequest::parseRequestHeader(Client &client, std::stringstream &requestStream)
{
	std::string line;

	while (std::getline(requestStream, line) && line.length() != 1) {
		std::size_t separatorPos = line.find(':');
		if (separatorPos != std::string::npos) {
			std::string headerName = line.substr(0, separatorPos);
			std::string headerValue = line.substr(separatorPos + 2);
			headerValue = line.substr(separatorPos + 2, headerValue.length() - 1);
			if (headerName == "Host") {
				this->checkIsValidHost(client, headerValue);
			}
			if (headerName == "Content-Length") {
				client._contentLenght = std::atoi(headerValue.c_str());
			}
			client._headers.insert(std::make_pair(headerName, headerValue));
		}
	}

	if (client._method != POST) {
		return ;
	} else {
		std::size_t bodyPos = client._requestStr.str().find("\r\n\r\n");
		client._bodyReq << client._requestStr.str().substr(bodyPos + 4);
		client._bodySize = client._bodyReq.str().length();
	}
}

void HttpRequest::parse(Client &client)
{
	std::stringstream requestStream(client._requestStr.str());

	this->parseRequestLine(client, requestStream);
	this->parseRequestHeader(client, requestStream);

	std::ofstream logfile("http_request_logs.txt", std::ios::app);
	if (!logfile.is_open()) {
		printError("HttpRequest log file creation failed");
	}
	logfile << client._requestStr.str();
    logfile.close();
}

/* ************************************************************************** */
/*                                    Utils                                   */
/* ************************************************************************** */

void HttpRequest::checkIsValidHost(Client &client, std::string headerValue)
{
	std::size_t separatorPos = headerValue.find(':');
	if (separatorPos != std::string::npos) {
		std::string host = headerValue.substr(0, separatorPos);
		std::string portStr = headerValue.substr(separatorPos + 1);
		if (host.empty() || portStr.empty()) {
			client._validHost = false;
			return ;
		}
		if (host != "localhost" && host != client._config.getHost() && host != client._config.getServerName()) {
			client._validHost = false;
			return ;
		}
		for (size_t i = 0; i < portStr.length(); i++) {
			if (!std::isdigit(portStr[i])) {
				client._validHost = false;
				return ;
			}
		}
		std::stringstream ss(portStr);
		int port;
		ss >> port;
		if (ss.fail() || port != client._config.getPort()) {
			client._validHost = false;
			return ;
		}
		return ;
	}
	if (headerValue != client._config.getServerName()) {
		client._validHost = false;
	}
}