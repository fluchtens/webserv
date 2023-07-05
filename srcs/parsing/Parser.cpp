/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 12:22:01 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/05 09:48:15 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

/* ************************************************************************** */
/*                           Constructor Destructor                           */
/* ************************************************************************** */

Parser::Parser(std::ifstream &cfgFile) : _port(80), _host("127.0.0.1"), _serverName("default_name"), _index("index.html"), _nbrLocation(0)
{
	this->parseCfgFile(cfgFile);
	this->printParsing();
}

Parser::~Parser(void)
{
	return ;
}

/* ************************************************************************** */
/*                              Getters / Setters                             */
/* ************************************************************************** */

const unsigned int &Parser::getPort(void) const
{
	return (this->_port);
}

const std::string &Parser::getHost(void) const
{
	return (this->_host);
}

const std::string &Parser::getServerName(void) const
{
	return (this->_serverName);
}

const std::string &Parser::getRoot(void) const
{
	return (this->_root);
}

const std::string &Parser::getIndex(void) const
{
	return (this->_index);
}

const std::string Parser::getErrorPage(int code) const
{
	std::map<int, std::string>::const_iterator it = this->_errorPage.find(code);
	if (it != this->_errorPage.end()) {
		return (it->second);
	}
	return ("notFound");
}

std::string	Parser::getLocationUrl(size_t pos) const
{
	if (pos >= _location.size())
	{
		std::cerr << "\033[1;31mgetLocationUrl : Bad size !\033[0m" << std::endl;
		exit (-4);
	} 
	return (_location[pos].getUrl());
}

std::vector<std::string>	Parser::getLocationAllow(std::string url) const
{
	for(std::vector<Location>::const_iterator it = _location.begin(); it != _location.end(); it++)
	{
		if(it->getUrl() == url)
			return (it->getAllow());
	}
	std::cerr << "\033[1;31mgetLocationAllow : url don't exist ! \033[0m" << std::endl;
	exit (-3);
}

std::string	Parser::getLocationRoot(std::string url) const
{
	for(std::vector<Location>::const_iterator it = _location.begin(); it != _location.end(); it++)
	{
		if(it->getUrl() == url)
			return (it->getRoot());
	}
	std::cerr << "\033[1;31mgetLocationRoot : url don't exist ! \033[0m" << std::endl;
	exit (-3);
}

std::string	Parser::getLocationIndex(std::string url) const
{
	for(std::vector<Location>::const_iterator it = _location.begin(); it != _location.end(); it++)
	{
		if(it->getUrl() == url)
			return (it->getIndex());
	}
	std::cerr << "\033[1;31mgetLocationIndex : url don't exist ! \033[0m" << std::endl;
	exit (-3);
}

std::string	Parser::getLocationPath(std::string url) const
{
	for(std::vector<Location>::const_iterator it = _location.begin(); it != _location.end(); it++)
	{
		if(it->getUrl() == url)
			return (it->getPath());
	}
	std::cerr << "\033[1;31mgetLocationIndex : url don't exist ! \033[0m" << std::endl;
	exit (-3);
}

bool	Parser::getLocationAutoIndex(std::string url) const
{
	for(std::vector<Location>::const_iterator it = _location.begin(); it != _location.end(); it++)
	{
		if(it->getUrl() == url)
			return (it->getAutoIndex());
	}
	std::cerr << "\033[1;31mgetLocationIndex : url don't exist ! \033[0m" << std::endl;
	exit (-3);
}

std::string	Parser::getLocationReturn(std::string url) const
{
	for(std::vector<Location>::const_iterator it = _location.begin(); it != _location.end(); it++)
	{
		if(it->getUrl() == url)
			return (it->getReturn());
	}
	std::cerr << "\033[1;31mgetLocationReturn : url don't exist ! \033[0m" << std::endl;
	exit (-3);
}

int	Parser::getLocationMaxSize(std::string url) const
{
	for(std::vector<Location>::const_iterator it = _location.begin(); it != _location.end(); it++)
	{
		if(it->getUrl() == url)
			return (it->getMaxSize());
	}
	std::cerr << "\033[1;31mgetLocationReturn : url don't exist ! \033[0m" << std::endl;
	exit (-3);
}

bool	Parser::getLocationDeny(std::string url) const
{
	for(std::vector<Location>::const_iterator it = _location.begin(); it != _location.end(); it++)
	{
		if(it->getUrl() == url)
			return (it->getDeny());
	}
	std::cerr << "\033[1;31mgetLocationIndex : url don't exist ! \033[0m" << std::endl;
	exit (-3);
}

size_t	Parser::getNbrLocation(void) const
{
	return (this->_nbrLocation);
}

std::vector<Location> &Parser::getLocation(void)
{
	return (this->_location);
}

/* ************************************************************************** */
/*                          Private Member functions                          */
/* ************************************************************************** */

void Parser::parseCfgFile(std::ifstream &cfgFile)
{
	std::string line;
	bool insideServerBlock = false;

	while (std::getline(cfgFile, line) && this->strTrimWhiteSpaces(line) != "}")
	{
		if (line.empty())
			continue;

		std::stringstream ss(line);
		std::string key;
		std::string value;

		ss >> key;
		ss >> value;

		if (key.empty() && value.empty())
			continue;

		if (!ss.eof() && key != "location" && key != "error_page") {
			std::string tmp;
			ss >> tmp;
			if (ss) {
				throw std::runtime_error("unexpected additional content after value of " + key);
			}
		}

		if (value.back() != ';' && key != "server" && key != "location" && key != "error_page") {
			throw std::runtime_error("missing final semicolon at the end of " + key);
		}

		// std::cout << key << " = " << value << std::endl;

		if (!insideServerBlock) {
			if (key == "server" && value == "{") {
				insideServerBlock = true;
			} else {
				throw (std::runtime_error("information found outside a server block"));
			}
		} else {
			std::string tmp = value.substr(0, value.size() - 1);

			if (key == "listen")
				this->parsePort(tmp);
			else if (key == "host")
				this->parseHost(tmp);
			else if (key == "server_name")
				this->_serverName = tmp;
			else if (key == "root")
				this->_root = tmp;
			else if (key == "index")
				this->_index = tmp;
			else if (key == "error_page")
			{
				std::string page;
				ss >> page;
				if (!ss.eof()) {
					std::string tmp;
					ss >> tmp;
					if (ss) {
						throw std::runtime_error("unexpected additional content after page path of error_page");
					}
				}
				if (page.back() != ';') {
					throw std::runtime_error("missing final semicolon at the end of error_page");
				}
				this->parseErrorPage(value, page.substr(0, page.size() - 1));
			}
			else if (key == "location")
				this->parseLocation(cfgFile, value);
			else
				throw (std::runtime_error("unknown key in configuration file (" + key + ")"));
		}

		line.clear();
		ss.clear();
		key.clear();
		value.clear();
	}
}

void Parser::parsePort(const std::string &port)
{
	for (size_t i = 0; i < port.length(); i++) {
		if (!std::isdigit(port[i])) {
			throw (std::runtime_error("port contains non-digital characters"));
		}
	}
	std::stringstream ss(port);
	unsigned int nb;
	ss >> nb;
	if (ss.fail())
		throw (std::runtime_error("invalid port number"));
	this->_port = nb;
}

void Parser::parseHost(const std::string &host)
{
	this->_host = (host == "localhost" ? "127.0.0.1" : host);
}

void Parser::parseErrorPage(const std::string &error, const std::string &page)
{
	for (size_t i = 0; i < error.length(); i++) {
		if (!std::isdigit(error[i])) {
			throw (std::runtime_error("error page code contains non-digital characters"));
		}
	}
	std::stringstream ss(error);
	int nb;
	ss >> nb;
	if (ss.fail())
		throw (std::runtime_error("invalid error page code number"));
	this->_errorPage.insert(make_pair(nb, page));
}

void    Parser::parseLocation(std::ifstream &cfgFile, const std::string &url)
{
	size_t i = 0;
	Location tmp(cfgFile, url);

	while (i != _location.size() && _location[i].getUrl() != url)
		i++;
	if (i != _location.size())
		_location[i] = tmp;
	else
	{
		std::vector<Location>::const_iterator it = _location.end();
		_location.insert(it, tmp);
		_nbrLocation += 1;
	}
}

std::string Parser::strTrimWhiteSpaces(const std::string &str)
{
	size_t first = str.find_first_not_of(" \t");
	if (first == std::string::npos)
		return ("");
	size_t last = str.find_last_not_of(" \t");
	return (str.substr(first, last - first + 1));
}

void Parser::printParsing(void) const
{
	std::cout << "/********************************/" << std::endl;
	std::cout << "Port: " << this->_port << std::endl;
	std::cout << "Host: " << this->_host << std::endl;
	std::cout << "Servername: " << this->_serverName << std::endl;
	std::cout << std::endl;
	std::cout << "Root: " << this->_root << std::endl;
	std::cout << "Index: " << this->_index << std::endl;
	std::cout << std::endl;
	std::cout << "NbrLocation: " << this->_nbrLocation << std::endl;
	std::cout << std::endl;
	std::map<int, std::string>::const_iterator it = this->_errorPage.begin();
	while (it != this->_errorPage.end()) {
		std::cout << "ErrorPage: " << it->first << " " << it->second << std::endl;
		it++;
	}
	std::cout << "/********************************/" << std::endl;
}
