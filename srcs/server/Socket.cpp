/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 10:40:34 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/25 14:41:33 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

/* ************************************************************************** */
/*                           Constructor Destructor                           */
/* ************************************************************************** */

Socket::Socket(Parser *cfg)
{
	this->_cfg = cfg;
	this->_createSocket();
	this->_bindSocket();
	this->_listenConnection();
}

Socket::~Socket(void)
{
	close(this->_serverFd);
}

/* ************************************************************************** */
/*                          Private Member functions                          */
/* ************************************************************************** */

void Socket::_createSocket(void)
{
	if ((this->_serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		throw (std::runtime_error("socket"));
	}
}

void Socket::_bindSocket(void)
{
	std::memset(static_cast<void *>(&this->_address), 0, sizeof(this->_address));

	this->_address.sin_family = AF_INET;
	// this->_address.sin_addr.s_addr = htonl(INADDR_ANY);
	// this->_address.sin_addr.s_addr = htonl(this->_cfg->getHost());
	// this->_address.sin_port = htons(80);
	this->_address.sin_port = htons(this->_cfg->getPort());

	if (bind(this->_serverFd, (struct sockaddr *)&this->_address, sizeof(this->_address)) < 0) { 
		throw (std::runtime_error("bind"));
	}
}

void Socket::_listenConnection(void)
{
	if (listen(this->_serverFd, SOMAXCONN) < 0) {
		throw (std::runtime_error("listen"));
	}
}

/* ************************************************************************** */
/*                          Public Member functions                           */
/* ************************************************************************** */

void Socket::acceptConnection(void)
{
	int addrlen = sizeof(this->_address);

	if ((this->_newSocketFd = accept(this->_serverFd, (struct sockaddr *)&this->_address, (socklen_t*)&addrlen))<0) {
		throw (std::runtime_error("accept"));
	}
}

/* ************************************************************************** */
/*                              Getters / Setters                             */
/* ************************************************************************** */

int Socket::getNewServerFd(void) const
{
	return (this->_newSocketFd);
}
