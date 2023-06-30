/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 12:22:01 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/30 15:46:49 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

/* ************************************************************************** */
/*                           Constructor Destructor                           */
/* ************************************************************************** */

Parser::Parser(std::ifstream &cfgFile) : _port(80), _host(localhost), _serverName("default_name"), _index("index.html"), _nbrLocation(0)
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

size_t	Parser::getNbrLocation() const
{ return (_nbrLocation); }

std::vector<Parser::Location> &Parser::getLocation()
{ return (_location); }

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
			if (key == "listen")
				this->parsePort(value.substr(0, value.size() - 1));
			else if (key == "host")
				this->parseHost(value.substr(0, value.size() - 1));
			else if (key == "server_name")
				this->parseServerName(value.substr(0, value.size() - 1));
			else if (key == "root")
				this->parseRoot(value.substr(0, value.size() - 1));
			else if (key == "index")
				this->parseIndex(value.substr(0, value.size() - 1));
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
	this->_host = (host == "localhost" ? localhost : host);
}

void Parser::parseServerName(const std::string &serverName)
{
	this->_serverName = serverName;
}

void Parser::parseRoot(const std::string &root)
{
	this->_root = root;
}

void Parser::parseIndex(const std::string &index)
{
	this->_index = index;
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

/*************************** Class Location ****************************/
/*************************** Class Location ****************************/
/*************************** Class Location ****************************/
/*************************** Class Location ****************************/
/*************************** Class Location ****************************/
/*************************** Class Location ****************************/
/*************************** Class Location ****************************/
/*************************** Class Location ****************************/
/*************************** Class Location ****************************/
/*************************** Class Location ****************************/
/*************************** Class Location ****************************/

Parser::Location::Location(std::ifstream &cfgFile, const std::string &url) : _url(url), _autoIndex(false), _deny(false)
{
	std::string line;

	while (getline(cfgFile, line))
	{
		std::stringstream	ss;
		std::string			key;
		std::string			value;

		ss << line;
		if (line.empty())
			continue;
		ss >> key;
		if (key == "}")
			break;
		while (ss >> value)
		{
			if (key == "allow")
			{
				std::string	tmp;
				if (!ss.eof())
					tmp = value.substr(0, value.size());
				else
					tmp = value.substr(0, value.size() -1);
				if (tmp != "GET" && tmp != "POST" && tmp != "DELETE")
				{
					std::cerr << "Error : file config not valide. " << tmp << " is not valide." << std::endl;
					exit (-1);
				}
				_allow.push_back(tmp);
			}
			else if (key == "root")
				_root = value.substr(0, value.size() - 1);
			else if (key == "index")
				_index = value.substr(0, value.size() - 1);
			else if (key == "path")
				_path = value.substr(0, value.size() - 1);
			else if (key == "autoindex")
			{
				if(value.substr(0, value.size() - 1) == "on")
					_autoIndex = true;
			}
			else if (key == "return")
				_return = value.substr(0, value.size() - 1);
            else if(key == "cgi_path")
                _cgiPath = value.substr(0, value.size() - 1);
			else if(key == "cgi_script")
                _cgiScript = value.substr(0, value.size() - 1);
			else if(key == "client_max_body_size")
                _maxSize = std::atoi(value.substr(0, value.size() - 1).c_str());
			else if (key == "deny")
			{
				if(value.substr(0, value.size() - 1) == "on")
					_deny = true;
			}
		}
		line.clear();
		ss.clear();
		key.clear();
		value.clear();
	}
}

Parser::Location::Location(const Location &srcs)
{ *this = srcs; }

Parser::Location::~Location()
{}

Parser::Location & Parser::Location::operator=(const Location & srcs)
{
	if (this != &srcs)
	{
		_url = srcs._url;
		for (std::vector<std::string>::const_iterator it = srcs._allow.begin(); it < srcs._allow.end(); it++)
			_allow.push_back(*it);
		_root = srcs._root;
		_index = srcs._index;
		_path = srcs._path;
		_autoIndex = srcs._autoIndex;
		_return = srcs._return;
        _cgiPath = srcs._cgiPath;
        _cgiScript = srcs._cgiScript;
		_maxSize = srcs._maxSize;
		_deny = srcs._deny;
	}
	return (*this);
}

const std::string & Parser::Location::getUrl() const
{ return (_url); }

const std::vector<std::string> & Parser::Location::getAllow() const
{ return (_allow); }

const std::string & Parser::Location::getRoot() const
{ return (_root); }

const std::string & Parser::Location::getIndex() const
{ return (_index); }

const std::string &	Parser::Location::getPath() const
{ return (_path); }

const bool & Parser::Location::getAutoIndex() const
{ return (_autoIndex); }

const std::string & Parser::Location::getReturn() const
{ return (_return); }

const std::string & Parser::Location::getCgiPath() const
{ return (_cgiPath); }

const std::string & Parser::Location::getCgiScript() const
{ return (_cgiScript); }

const int & Parser::Location::getMaxSize() const
{ return (_maxSize); }

const bool & Parser::Location::getDeny() const
{ return (_deny); }

bool Parser::Location::isMethodAllowed(std::string method) const
{
	for (std::vector<std::string>::const_iterator it = _allow.begin(); it != _allow.end(); it++)
	{
		if (*it == method)
			return (true);
	}
	return (false);
}