/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 12:22:01 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/19 18:11:37 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

/* ************************************************************************** */
/*                               Canonical form                               */
/* ************************************************************************** */

Parser::Parser(std::ifstream &cfgFile)
{
	// std::cout << "Parser constructor called" << std::endl;
	this->_port = -1;
	this->_serverName = "default_name";
	this->parseCfgFile(cfgFile);
	this->hasAllInfos();
	this->printParsing();
}

Parser::Parser(const Parser &rhs)
{
	// std::cout << "Parser copy constructor called" << std::endl;
	*this = rhs;
}

Parser &Parser::operator=(const Parser &rhs)
{
	// std::cout << "Parser copy assignment operator called" << std::endl;
	if (this != &rhs) {
		this->_port = rhs._port;
		this->_host = rhs._host;
		this->_serverName = rhs._serverName;
		this->_root = rhs._root;
		this->_index = rhs._index;
		this->_nbrLocation = rhs._nbrLocation;
		this->_errorPage = rhs._errorPage;
		this->_location = rhs._location;
	}
	return (*this);
}

Parser::~Parser(void)
{
	// std::cout << "Parser destructor called" << std::endl;
	return ;
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

		if (key.empty())
			continue;
		else if (!ss.eof() && key != "location" && key != "error_page") {
			std::string tmp;
			ss >> tmp;
			if (ss) {
				throw std::runtime_error("unexpected additional content after value of " + key);
			}
		}
		else if (value.back() != ';' && key != "server" && key != "location" && key != "error_page") {
			throw std::runtime_error("missing final semicolon at the end of the value of " + key);
		}

		if (!insideServerBlock) {
			if (key == "server" && value == "{")
				insideServerBlock = true;
			else
				throw (std::runtime_error("information found outside a server block"));
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
			else if (key == "error_page") {
				std::string path;
				ss >> path;
				if (!ss.eof()) {
					std::string tmp;
					ss >> tmp;
					if (ss)
						throw std::runtime_error("unexpected additional content after page path of error_page");
				}
				if (path.back() != ';')
					throw std::runtime_error("missing final semicolon at the end of error_page");
				this->parseErrorPage(value, path.substr(0, path.size() - 1));
			}
			else if (key == "location") {
				std::string bracket;
				ss >> bracket;
				if (!ss.eof()) {
					std::string tmp;
					ss >> tmp;
					if (ss)
						throw std::runtime_error("unexpected additional content after bracket of location");
				}
				if (bracket != "{")
					throw std::runtime_error("missing or invalid bracket at the end of location");
				this->parseLocation(cfgFile, value);
			}
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

void Parser::parseLocation(std::ifstream &cfgFile, const std::string &url)
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

void Parser::hasAllInfos(void)
{
	std::string	missingInfo;

	if (this->_port < 0)
		missingInfo += "listen ";
	if (this->_host.empty())
		missingInfo += "host ";
	if (this->_serverName.empty())
		missingInfo += "serverName ";
	if (this->_root.empty())
		missingInfo += "root ";
	if (this->_index.empty())
		missingInfo += "index ";
	missingInfo = this->strTrimWhiteSpaces(missingInfo);
	if (!missingInfo.empty()) {
		throw (std::runtime_error("server missing information (" + missingInfo + ")"));
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
	std::cout << "\033[32m" << *this << "\033[0m";
	std::vector<Location>::const_iterator it = this->_location.begin();
	while (it != this->_location.end()) {
		std::cout << "\033[32m" << *it << "\033[0m";
		it++;
	}
	std::cout << std::endl;
}

/* ************************************************************************** */
/*                              Getters / Setters                             */
/* ************************************************************************** */

const int &Parser::getPort(void) const
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

const std::string Parser::getErrorPages(void) const
{
	std::string errorPages;
	std::map<int, std::string>::const_iterator it = this->_errorPage.begin();
	while (it != this->_errorPage.end()) {
		errorPages += std::to_string(it->first) + ": " + it->second;
		if (++it != this->_errorPage.end()) {
			errorPages += " | ";
		}
	}
	if (errorPages.empty())
		errorPages = "no pages configured";
	return (errorPages);
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

size_t	Parser::getNbrLocation(void) const
{
	return (this->_nbrLocation);
}

std::vector<Location> &Parser::getLocation(void)
{
	return (this->_location);
}

/* ************************************************************************** */
/*                             Operators Overloads                            */
/* ************************************************************************** */

std::ostream &operator<<(std::ostream &o, const Parser &rhs)
{
	o << "> Server config: ";
	o << "listen: " << rhs.getPort() << " | ";
	o << "host: " << rhs.getHost() << " | ";
	o << "server_name: " << rhs.getServerName() << " | ";
	o << "root: " << rhs.getRoot() << " | ";
	o << "index: " << rhs.getIndex() << std::endl;
	o << "> Error pages: " << rhs.getErrorPages() << std::endl;
	return (o);
}
