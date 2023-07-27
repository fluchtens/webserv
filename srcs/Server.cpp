/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:36:02 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/27 13:50:59 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/*                               Canonical form                               */
/* ************************************************************************** */

Server::Server(Parser *server) : _cfg(server), _location(_cfg->getLocation())
{
	// std::cout << "Server constructor called" << std::endl;
	this->_maxConnection = 100;
	this->_nbrLocation = this->_cfg->getNbrLocation();
	this->creatSocket();
	this->configSocket();
	this->bindSocket();
	this->listenTCP();
}

Server::Server(const Server &rhs) : _cfg(rhs._cfg), _location(rhs._location)
{
	// std::cout << "Server copy constructor called" << std::endl;
	*this = rhs;
}

Server &Server::operator=(const Server &rhs)
{
	// std::cout << "Server copy assignment operator called" << std::endl;
	if (this != &rhs) {
		this->_cfg = rhs._cfg;
		this->_maxConnection = rhs._maxConnection;
		this->_serverFd = rhs._serverFd;
		this->_address = rhs._address;
		this->_location = rhs._location;
		this->_nbrLocation = rhs._nbrLocation;
	}
	return (*this);
}

Server::~Server(void)
{
	// std::cout << "Server destructor called" << std::endl;
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

const int &Server::getPort(void) const
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

/* ************************************************************************** */
/*                          Private Member functions                          */
/* ************************************************************************** */

void Server::creatSocket(void)
{
	this->_serverFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->_serverFd == -1) {
		throw (std::runtime_error("socket() failed"));
	}
	if (fcntl(this->_serverFd, F_SETFL, O_NONBLOCK) < 0) {
		throw (std::runtime_error("fcntl() failed"));
	}
}

void Server::configSocket(void)
{
	int	optval = 1;
	socklen_t optlen = sizeof(optval);
	if (setsockopt(this->_serverFd, SOL_SOCKET, SO_REUSEADDR, &optval, optlen) == -1) {
		throw (std::runtime_error("setsockopt() failed"));
	}
}

void Server::bindSocket(void)
{
	std::memset(static_cast<void *>(&this->_address), 0, sizeof(this->_address));

	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = convertIpAddress(this->_cfg->getHost()); // htonl(INADDR_ANY)
	this->_address.sin_port = htons(this->_cfg->getPort());

	if (bind(this->_serverFd, reinterpret_cast<sockaddr *>(&this->_address), sizeof(this->_address)) == -1) {
		throw (std::runtime_error("bind() failed"));
	}
}

void Server::listenTCP(void)
{
	if (listen(this->_serverFd, this->_maxConnection) == -1) {
		throw (std::runtime_error("listen()"));
	}
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
