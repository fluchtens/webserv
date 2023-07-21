/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 17:59:23 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/21 12:52:29 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(Parser &config, Server &server, std::vector<Location> &location) : _config(config), _server(server), _location(location)
{
	this->_isAlive = true;
	this->_contentLenght = 0;
	this->_sizeBody = 0;
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
	_socket = 0;
}

Client::Client(const Client &srcs) :
    _config(srcs._config),
	_server(srcs._server),
	_location(srcs._location)
{ *this = srcs; }

Client::~Client(void)
{
	return ;
}

Client &Client::operator=(const Client &srcs)
{
	if (this != &srcs)
	{
		_socket = srcs._socket;
		_csin = srcs._csin;
		_crecSize = srcs._crecSize;
		_lastGetTime = srcs._lastGetTime;
		_isAlive = srcs._isAlive;
		_requestStr << srcs._requestStr.str();
		_contentLenght = srcs._contentLenght;
		_sizeBody = srcs._sizeBody;
		_requestPars = srcs._requestPars;
		_httpVersion = srcs._httpVersion;
		_uri = srcs._uri;
		_method = srcs._method;
		_headers = srcs._headers;
		_bodyReq << srcs._bodyReq.str();
		_cookie = srcs._cookie;
		_query = srcs._query;
		_filePath = srcs._filePath;
		_bodyRep = srcs._bodyRep;
		_response = srcs._response;
		_sizeSend = srcs._sizeSend;
		_sizeRep = srcs._sizeRep;
		_config = srcs._config;
		_server = srcs._server;
		_location = srcs._location;
	}
	return (*this);
}
