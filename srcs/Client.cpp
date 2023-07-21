/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 17:59:23 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/21 19:05:57 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/* ************************************************************************** */
/*                               Canonical form                               */
/* ************************************************************************** */

Client::Client(Parser &config, Server &server, std::vector<Location> &location) : _config(config), _server(server), _location(location)
{
	this->_isAlive = true;
	this->_contentLenght = 0;
	this->_bodySize = 0;
	this->_requestPars = false;
	this->_httpVersion = "";
	this->_uri = "";
	this->_method = UNKNOWN;
	this->_bodyReq.str("");
	this->_cookie = "";
	this->_query = "";
	this->_filePath = "";
	this->_bodyRep = "";
	this->_response = "";
	this->_sizeSend = 0;
	this->_sizeRep = 0;
	_requestStr.str(std::string());
	_socketFd = 0;
}

Client::Client(const Client &rhs) : _config(rhs._config), _server(rhs._server), _location(rhs._location)
{
	*this = rhs;
}

Client &Client::operator=(const Client &rhs)
{
	if (this != &rhs) {
		_socketFd = rhs._socketFd;
		_socketAddress = rhs._socketAddress;
		_socketAddrLen = rhs._socketAddrLen;
		_isAlive = rhs._isAlive;
		_requestStr << rhs._requestStr.str();
		_contentLenght = rhs._contentLenght;
		_bodySize = rhs._bodySize;
		_requestPars = rhs._requestPars;
		_httpVersion = rhs._httpVersion;
		_uri = rhs._uri;
		_method = rhs._method;
		_headers = rhs._headers;
		_bodyReq << rhs._bodyReq.str();
		_cookie = rhs._cookie;
		_query = rhs._query;
		_filePath = rhs._filePath;
		_bodyRep = rhs._bodyRep;
		_response = rhs._response;
		_sizeSend = rhs._sizeSend;
		_sizeRep = rhs._sizeRep;
		_config = rhs._config;
		_server = rhs._server;
		_location = rhs._location;
	}
	return (*this);
}

Client::~Client(void)
{
	return ;
}