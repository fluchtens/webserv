/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 10:40:34 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/21 08:34:40 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

/* ************************************************************************** */
/*                           Constructor Destructor                           */
/* ************************************************************************** */

Socket::Socket(void)
{
	this->_domain = AF_INET;
	this->_service = SOCK_STREAM;
	this->_protocol = 0;
	this->_interface = INADDR_ANY;
	this->_port = 8080;
	this->_backlog = SOMAXCONN;
}

Socket::~Socket(void)
{
	close(this->_serverFd);
}

/* ************************************************************************** */
/*                              Getters / Setters                             */
/* ************************************************************************** */

int Socket::getNewServerFd(void) const
{
	return (this->_newSocketFd);
}

/* ************************************************************************** */
/*                          Public Member functions                           */
/* ************************************************************************** */

void Socket::launch(void)
{
	if ((this->_serverFd = socket(this->_domain, this->_service, this->_protocol)) < 0) {
		throw (std::runtime_error("socket"));
	}

	memset((char *)&this->_address, 0, sizeof(this->_address));

	this->_address.sin_family = this->_domain;
	this->_address.sin_addr.s_addr = htonl(this->_interface);
	this->_address.sin_port = htons(this->_port);

	if (bind(this->_serverFd, (struct sockaddr *)&this->_address, sizeof(this->_address)) < 0) { 
		throw (std::runtime_error("bind"));
	}

	if (listen(this->_serverFd, this->_backlog) < 0) {
		throw (std::runtime_error("listen"));
	}
}

void Socket::acceptConnection(void)
{
	int addrlen = sizeof(this->_address);

	if ((this->_newSocketFd = accept(this->_serverFd, (struct sockaddr *)&this->_address, (socklen_t*)&addrlen))<0) {
		throw (std::runtime_error("accept"));
	}
}
