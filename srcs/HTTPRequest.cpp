/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 10:28:19 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/21 13:05:34 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"

/* ************************************************************************** */
/*                               Canonical form                               */
/* ************************************************************************** */

HTTPRequest::HTTPRequest(void)
{
	return ;
}

HTTPRequest::HTTPRequest(Client &client)
{
	parseRequest(client);
}

HTTPRequest::HTTPRequest(const HTTPRequest &rhs)
{
	*this = rhs;
}

HTTPRequest &HTTPRequest::operator=(const HTTPRequest &rhs)
{
	(void)rhs;
	return (*this);
}

HTTPRequest::~HTTPRequest(void)
{
	return ;
}

/* ************************************************************************** */
/*                          Public Member functions                           */
/* ************************************************************************** */

void HTTPRequest::parseRequest(Client &client)
{
	std::stringstream requestStream(client._requestStr.str());
	std::string line;

	this->parseRequestLine(client, requestStream);

	while (std::getline(requestStream, line) && line.length() != 1)
	{
		std::cout << "line: " << line << std::endl;
		size_t separator = line.find(": ");
		if (separator != std::string::npos)
		{
			std::string headerName = line.substr(0, separator);
			std::string headerValue = line.substr(separator + 2, line.size() - separator - 2 - 1);
			if (headerName == "Content-Length")
				client._contentLenght = std::atoi(headerValue.c_str());
			client._headers[headerName] = headerValue;
			if (headerName == "Cookie")
				client._cookie = headerValue.c_str();
		}
	}
	if (client._method != POST)
	{
		requestStream.clear();
		line.clear();
		return;
	}
	size_t posBody = client._requestStr.str().find("\r\n\r\n");
	client._bodyReq << client._requestStr.str().substr(posBody + 4);
	client._sizeBody = client._bodyReq.str().size();
}

/* ************************************************************************** */
/*                          Private Member functions                          */
/* ************************************************************************** */

// line: Host: localhost:4242
// line: User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:109.0) Gecko/20100101 Firefox/110.0
// line: Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
// line: Accept-Language: en-US,en;q=0.5
// line: Accept-Encoding: gzip, deflate, br
// line: Connection: keep-alive
// line: Cookie: delicieux_cookie=choco
// line: Upgrade-Insecure-Requests: 1
// line: Sec-Fetch-Dest: document
// line: Sec-Fetch-Mode: navigate
// line: Sec-Fetch-Site: none
// line: Sec-Fetch-User: ?1

void HTTPRequest::parseRequestLine(Client &client, std::stringstream &requestStream)
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

// void parseRequestHeader(Client &client, std::stringstream &requestStream)
// {
// 	std::string line;
// }
