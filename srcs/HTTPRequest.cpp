/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 10:28:19 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/21 18:48:45 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"

/* ************************************************************************** */
/*                               Canonical form                               */
/* ************************************************************************** */

HTTPRequest::HTTPRequest(Client &client) : _client(client)
{
	this->parseRequest();
}

HTTPRequest::HTTPRequest(const HTTPRequest &rhs) : _client(rhs._client)
{
	*this = rhs;
}

HTTPRequest &HTTPRequest::operator=(const HTTPRequest &rhs)
{
	(void) rhs;
	return (*this);
}

HTTPRequest::~HTTPRequest(void)
{
	return ;
}

/* ************************************************************************** */
/*                          Private Member functions                          */
/* ************************************************************************** */

void HTTPRequest::parseRequest(void)
{
	std::stringstream requestStream(this->_client._requestStr.str());

	this->parseRequestLine(requestStream);
	this->parseRequestHeader(requestStream);
}

void HTTPRequest::parseRequestLine(std::stringstream &requestStream)
{
	std::string line;

	if (std::getline(requestStream, line))
	{
		std::stringstream ss(line);
		std::string method;

		ss >> method;
		ss >> this->_client._uri;
		ss >> this->_client._httpVersion;

		if (method == "GET") {
			this->_client._method = GET;
		}
		else if (method == "POST") {
			this->_client._method = POST;
		}
		else if (method == "DELETE") {
			this->_client._method = DELETE;
		}
		else {
			this->_client._method = UNKNOWN;
		}

		std::string::size_type pos = this->_client._uri.find("?");
		if (pos != std::string::npos) {
			this->_client._query = this->_client._uri.substr(pos);
			this->_client._uri.erase(pos);
		}
	}
}

void HTTPRequest::parseRequestHeader(std::stringstream &requestStream)
{
	std::string line;

	while (std::getline(requestStream, line) && line.length() != 1) {
		std::size_t separatorPos = line.find(':');

		if (separatorPos != std::string::npos) {
			std::string headerName = line.substr(0, separatorPos);
			std::string headerValue = line.substr(separatorPos + 2);
			if (headerName == "Content-Length") {
				this->_client._contentLenght = std::atoi(headerValue.c_str());
			}
			else if (headerName == "Cookie") {
				this->_client._cookie = headerValue;
			}
			this->_client._headers.insert(std::make_pair(headerName, headerValue));
		}
	}

	if (this->_client._method != POST) {
		return ;
	} else {
		std::size_t bodyPos = this->_client._requestStr.str().find("\r\n\r\n");
		this->_client._bodyReq << this->_client._requestStr.str().substr(bodyPos + 4);
		this->_client._bodySize = this->_client._bodyReq.str().length();
		std::cout << _client._bodySize << std::endl;
	}
}
