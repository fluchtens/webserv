/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:36:02 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/05 09:46:49 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/*                           Constructor Destructor                           */
/* ************************************************************************** */

Server::Server(Parser *server)
{
	this->_cfg = server;
	this->_currentConnection = 0;
	this->_maxConnection = 100;
	this->_location = this->_cfg->getLocation();
	this->_nbrLocation = this->_cfg->getNbrLocation();
	this->creatSocket();
	this->bindSocket();
	this->listenTCP();
}

Server::~Server(void)
{
	this->closeSocket();
}

/* ************************************************************************** */
/*                              Getters / Setters                             */
/* ************************************************************************** */

const int &Server::getSocket(void) const
{
	return (this->_serverFd);
}

Server &Server::getServer(void)
{
	return (*this);
}

const unsigned int &Server::getPort(void) const
{
	return (this->_cfg->getPort());
}

const std::string &Server::getHost(void) const
{
	return (this->_cfg->getHost());
}

const std::string &Server::getServerName(void) const
{
	return (this->_cfg->getServerName());
}

const std::string &Server::getRoot(void) const
{
	return (this->_cfg->getRoot());
}

const std::string &Server::getIndex(void) const
{
	return (this->_cfg->getIndex());
}

const std::string Server::getErrorPage(int code) const
{
	return (this->_cfg->getErrorPage(code));
}

size_t Server::getNbrLocation(void) const
{
	return (this->_cfg->getNbrLocation());
}

Parser &Server::getConfig(void)
{
	return (*this->_cfg);
}

std::vector<Location> &Server::getLocation(void)
{
	return (this->_location);
}

const int &Server::getCurrentConnection(void) const
{
	return (this->_currentConnection);
}

bool Server::hasCapacity(void) const
{
	return (this->_currentConnection < this->_maxConnection);
}

void	Server::incrementCurrentConnection(void)
{
	++this->_currentConnection;
}

void	Server::decrementCurrentConnection(void)
{
	--this->_currentConnection;
}

/* ************************************************************************** */
/*                          Private Member functions                          */
/* ************************************************************************** */

void Server::creatSocket(void)
{
	// this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	this->_serverFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->_serverFd < 0) {
		throw (std::runtime_error("socket()"));
	}
	if (fcntl(this->_serverFd, F_SETFL, O_NONBLOCK) < 0) {
		throw (std::runtime_error("fcntl()"));
	}
}

void Server::bindSocket(void)
{
	std::memset(static_cast<void *>(&this->_address), 0, sizeof(this->_address));

	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = convertIpAddress(this->_cfg->getHost()); // htonl(INADDR_ANY)
	this->_address.sin_port = htons(this->_cfg->getPort());

	if (bind(this->_serverFd, reinterpret_cast<sockaddr *>(&this->_address), sizeof(this->_address)) < 0) {
		throw (std::runtime_error("bind()"));
	}
}

void Server::listenTCP(void)
{
	if (listen(this->_serverFd, this->_maxConnection) < 0) {
		throw (std::runtime_error("listen()"));
	}
	std::cout << "\033[33mlistenTCP() : Listening to port " << this->_cfg->getPort() << "\033[0m" << std::endl;
}

void Server::closeSocket(void)
{
    close(this->_serverFd);
}

in_addr_t Server::convertIpAddress(const std::string &str)
{
	std::vector<std::string> octets;
    std::stringstream ss(str);
    std::string line;
    in_addr_t addr = 0;

    while (getline(ss, line, '.')) {
        octets.push_back(line);
	}
    if (octets.size() != 4) {
		throw (std::runtime_error("invalid IP address"));
    }

    for (int i = 3; i >= 0; --i) {
		for (size_t j = 0; j < octets[i].length(); j++) {
			if (!std::isdigit(octets[i][j])) {
				throw (std::runtime_error("invalid IP address"));
			}
		}

		std::stringstream ss(octets[i]);
		int octet;
		ss >> octet;

		if (ss.fail()) {
			throw (std::runtime_error("invalid IP address"));
		}
		if (octet < 0 || octet > 255) {
			throw (std::runtime_error("invalid IP address"));
		}

		addr |= static_cast<in_addr_t>(octet) << ((3 - i) * 8);
    }

    return htonl(addr);
}
