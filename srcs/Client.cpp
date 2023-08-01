/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 17:59:23 by fluchten          #+#    #+#             */
/*   Updated: 2023/08/01 10:07:02 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/* ************************************************************************** */
/*                               Canonical form                               */
/* ************************************************************************** */

Client::Client(Parser &config, Server &server, std::vector<Location> &location) : _config(config), _server(server), _location(location)
{
	// std::cout << "Client constructor called" << std::endl;
	// Life
	this->_isAlive = true;
	// Socket
	this->_socketFd = 0;
	// std::memset(&this->_socketAddress, 0, sizeof(this->_socketAddress));
	this->_socketAddrLen = 0;
	// Request
	this->_requestStr.str("");
	this->_requestPars = false;
	this->_method = UNKNOWN;
	this->_uri = "";
	this->_query = "";
	this->_httpVersion = "";
	this->_headers.clear();
	this->_validHost = true;
	this->_contentLenght = 0;
	this->_cookie = "";
	this->_bodyReq.str("");
	this->_bodySize = 0;
	// Response
	this->_filePath = "";
	this->_bodyResp = "";
	this->_response = "";
	this->_respSize = 0;
}

Client::Client(const Client &rhs) : _config(rhs._config), _server(rhs._server), _location(rhs._location)
{
	// std::cout << "Client copy constructor called" << std::endl;
	*this = rhs;
}

Client &Client::operator=(const Client &rhs)
{
	// std::cout << "Client copy assignment operator called" << std::endl;
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
		this->_validHost = rhs._validHost;
		this->_contentLenght = rhs._contentLenght;
		this->_cookie = rhs._cookie;
		this->_bodyReq << rhs._bodyReq.str();
		this->_bodySize = rhs._bodySize;
		// Response
		this->_filePath = rhs._filePath;
		this->_bodyResp = rhs._bodyResp;
		this->_response = rhs._response;
		this->_respSize = rhs._respSize;
	}
	return (*this);
}

Client::~Client(void)
{
	// std::cout << "Client destructor called" << std::endl;
}
