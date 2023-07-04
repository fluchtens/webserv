/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:36:02 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/04 20:00:59 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/*                           Constructor Destructor                           */
/* ************************************************************************** */

Server::Server(Parser *server) :
_config(server),
_currentConnection(0),
_maxConnection(100),
_location(_config->getLocation()),
_nbrLocation(_config->getNbrLocation())
{
	this->creatSocket();
	this->bindSocket();
	this->listenTCP();
}

Server::~Server(void)
{
	std::cout << "\033[32mLiberation des sockets serveur : " << getServerName() <<  "\033[0m" <<std::endl; 
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

const std::string &Server::getHost(void) const
{
	return (this->_config->getHost());
}
		
const unsigned int &Server::getPort(void) const
{
	return (this->_config->getPort());
}

const std::string &Server::getServerName(void) const
{
	return (this->_config->getServerName());
}

const std::string &Server::getRoot(void) const
{
	return (this->_config->getRoot());
}

const std::string &Server::getIndex(void) const
{
	return (this->_config->getIndex());
}

const std::string Server::getErrorPage(int code) const
{
	return (this->_config->getErrorPage(code));
}

size_t Server::getNbrLocation(void) const
{
	return (this->_config->getNbrLocation());
}

Parser &Server::getConfig(void)
{
	return (*this->_config);
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
		std::cerr << "\033[1;31mError : Server::creatSocket socket() " << strerror(errno) << "\033[0m" << std::endl;
		exit(1);
	}
	if (fcntl(this->_serverFd, F_SETFL, O_NONBLOCK) < 0) {
		std::cerr << "\033[1;31mError : Server::creatSocket fcntl() " << strerror(errno) << "\033[0m" << std::endl;
		exit(1);
	}
	std::cout << "\033[34mSocket created : \033[0m" << this->_serverFd << " en mode TCP/IP." << std::endl;
}

void Server::bindSocket(void)
{
	std::memset(static_cast<void *>(&this->_address), 0, sizeof(this->_address));

	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = convertIp(getHost()); // htonl(INADDR_ANY)
	this->_address.sin_port = htons(getPort());

	if (bind(this->_serverFd, reinterpret_cast<sockaddr *>(&this->_address), sizeof(this->_address)) < 0) {
		std::cerr << "\033[1;31mError : Server::bindSocket bind() " << strerror(errno) << "\033[0m" << std::endl;
		exit(1);
	}
}

void Server::listenTCP(void)
{
	if (listen(this->_serverFd, this->_maxConnection) < 0) {
		std::cerr << "\033[1;31mError : Server::linstenTCP listen() " << strerror(errno) << "\033[0m" << std::endl;
		exit (1);
	}
	std::cout << "\033[33mlistenTCP() - Ecoute du port " << getPort() << "\033[0m" << std::endl;
}

void Server::closeSocket(void)
{
    close(this->_serverFd);
}

in_addr_t Server::convertIp(const std::string &str)
{
	std::vector<std::string>	octets;
    std::stringstream			ss(str);
    std::string					token;

    while (getline(ss, token, '.'))
        octets.push_back(token);
    if (octets.size() != 4)
	{
        std::cerr << "\033[33mError: invalid IP address Server::convertIp() " << str << "\033[0m" << std::endl;
        exit(1);
    }
    in_addr_t addr = 0;
    for (int i = 3; i >= 0; --i)
	{
        try
		{
            int octet = stoi(octets[i]);
            if (octet < 0 || octet > 255) {
                std::cerr << "Error: invalid IP address " << str << std::endl;
                exit(-1);
            }
            addr |= static_cast<in_addr_t>(octet) << ((3 - i) * 8);
        }
		catch (const std::exception& e)
		{
            std::cerr << "\033[33mError: invalid IP address Server::convertIp() " << str << "\033[0m" << std::endl;
            exit(1);
        }
    }
    return htonl(addr);
}
