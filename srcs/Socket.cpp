/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 10:40:34 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/20 14:31:40 by fluchten         ###   ########.fr       */
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

	this->_create();
	this->_bind();
	this->_listen();
}

Socket::~Socket(void)
{
	return ;
}

/* ************************************************************************** */
/*                          Private Member functions                          */
/* ************************************************************************** */

void Socket::_create(void)
{
	this->_fd = socket(this->_domain, this->_service, this->_protocol);
	if (this->_fd == 0) {
		throw (std::runtime_error("socket"));
	}
}

void Socket::_bind(void)
{
	memset((char *)&this->_sockaddr, 0, sizeof(this->_sockaddr));

	this->_sockaddr.sin_family = this->_domain;
	this->_sockaddr.sin_addr.s_addr = htonl(this->_interface);
	this->_sockaddr.sin_port = htons(this->_port);

	if (bind(this->_fd,(struct sockaddr *)&this->_sockaddr,sizeof(this->_sockaddr)) < 0) { 
		throw (std::runtime_error("bind"));
	}
}

void Socket::_listen(void)
{
	if (listen(this->_fd, 10) < this->_backlog) {
		throw (std::runtime_error("listen"));
	}
}

/* ************************************************************************** */
/*                          Public Member functions                           */
/* ************************************************************************** */
