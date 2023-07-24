/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 17:59:23 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/24 17:39:32 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/* ************************************************************************** */
/*                               Canonical form                               */
/* ************************************************************************** */

Client::Client(Parser &config, Server &server, std::vector<Location> &location) : _config(config), _server(server), _location(location)
{
	// Client life
	this->_isAlive = true;
	// Client socket
	this->_socketFd = 0;
	// std::memset(&this->_socketAddress, 0, sizeof(this->_socketAddress));
	this->_socketAddrLen = 0;
	// Client request
	this->_requestStr.str("");
	this->_requestPars = false;
	this->_method = UNKNOWN;
	this->_uri = "";
	this->_query = "";
	this->_httpVersion = "";
	this->_headers.clear();
	this->_contentLenght = 0;
	this->_cookie = "";
	this->_bodyReq.str("");
	this->_bodySize = 0;

	this->_filePath = "";
	this->_bodyRep = "";
	this->_response = "";
	this->_sizeRep = 0;
	this->_autoIndex.clear();
}

Client::Client(const Client &rhs) : _config(rhs._config), _server(rhs._server), _location(rhs._location)
{
	*this = rhs;
}

Client &Client::operator=(const Client &rhs)
{
	if (this != &rhs) {
		// Classes
		this->_config = rhs._config;
		this->_server = rhs._server;
		this->_location = rhs._location;
		// Client life
		this->_isAlive = rhs._isAlive;
		// Client socket
		this->_socketFd = rhs._socketFd;
		this->_socketAddress = rhs._socketAddress;
		this->_socketAddrLen = rhs._socketAddrLen;
		// Client request
		this->_requestStr << rhs._requestStr.str();
		this->_requestPars = rhs._requestPars;
		this->_method = rhs._method;
		this->_uri = rhs._uri;
		this->_query = rhs._query;
		this->_httpVersion = rhs._httpVersion;
		this->_headers = rhs._headers;
		this->_contentLenght = rhs._contentLenght;
		this->_cookie = rhs._cookie;
		this->_bodyReq << rhs._bodyReq.str();
		this->_bodySize = rhs._bodySize;

		this->_filePath = rhs._filePath;
		this->_bodyRep = rhs._bodyRep;
		this->_response = rhs._response;
		this->_sizeRep = rhs._sizeRep;
		this->_autoIndex = rhs._autoIndex;
	}
	return (*this);
}

Client::~Client(void)
{
	return ;
}

/* ************************************************************************** */
/*                             Operators Overloads                            */
/* ************************************************************************** */

/* std::ostream &operator<<(std::ostream &o, const Client &rhs)
{
	// o << CLR_YELLOWB << "> Location: " << CLR_RESET << std::endl;
	// o << CLR_YELLOW;
	// o << "	- url: " << rhs.getUrl() << std::endl;
	// // o << "	- allow: " << rhs.getAllow() << std::endl;
	// o << "	- root: " << rhs.getRoot() << std::endl;
	// o << "	- index: " << rhs.getIndex() << std::endl;
	// o << "	- path: " << rhs.getPath() << std::endl;
	// o << "	- autoindex: " << rhs.getAutoIndex() << std::endl;
	// o << "	- return: " << rhs.getReturn() << std::endl;
	// o << "	- cgi_script: " << rhs.getCgiScript() << std::endl;
	// o << "	- cgi_path: " << rhs.getCgiPath() << std::endl;
	// o << CLR_RESET;
	return (o);
} */
