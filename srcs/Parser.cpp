/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 12:22:01 by fluchten          #+#    #+#             */
/*   Updated: 2023/08/03 11:34:23 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

/* ************************************************************************** */
/*                           Constructor Destructor                           */
/* ************************************************************************** */

Parser::Parser(std::ifstream &cfgFile)
{
	// std::cout << "Parser constructor called" << std::endl;
	this->_port = -1;
	this->_host = "";
	this->_serverName = "default_name";
	this->_root = "";
	this->_index = "";
	this->_maxBodySize = -1;
	this->_errorPage.clear();
	this->_location.clear();
	this->parseCfgFile(cfgFile);
	this->hasAllInfos();
	this->printParsing();
}

Parser::~Parser(void)
{
	// std::cout << "Parser destructor called" << std::endl;
}

/* ************************************************************************** */
/*                          Private Member functions                          */
/* ************************************************************************** */

void Parser::parseCfgFile(std::ifstream &cfgFile)
{
	std::string line;
	bool insideServerBlock = false;
	bool closeServerBlock = false;

	while (std::getline(cfgFile, line))
	{
		if (line.empty()) {
			continue;
		}

		std::stringstream ss(line);
		std::string key;
		std::string value;
		ss >> key;
		ss >> value;

		if (key.empty()) {
			continue;
		}

		if (key == "}") {
			if (ss) {
				throw std::runtime_error("unexpected additional content after server close bracket");
			}
			closeServerBlock = true;
			break;
		}

		if (key != "location" && key != "error_page") {
			std::string tmp;
			ss >> tmp;
			if (ss) {
				throw std::runtime_error("unexpected additional content after value of " + key);
			}
		}

		if (value.back() != ';' && key != "server" && key != "location" && key != "error_page") {
			throw std::runtime_error("missing final semicolon at the end of the value of " + key);
		}

		if (!insideServerBlock) {
			if (key == "server" && value == "{") {
				insideServerBlock = true;
			} else {
				throw (std::runtime_error("information found outside a server block"));
			}
		} else {
			std::string tmp = value.substr(0, value.size() - 1);

			if (key == "listen") {
				this->parsePort(tmp);
			}
			else if (key == "host") {
				this->parseHost(tmp);
			}
			else if (key == "server_name") {
				this->parseServerName(tmp);
			}
			else if (key == "root") {
				this->parseRoot(tmp);
			}
			else if (key == "index") {
				this->parseIndex(tmp);
			}
			else if (key == "max_body_size") {
				this->parseMaxBodySize(tmp);
			}
			else if (key == "error_page") {
				std::string path;
				ss >> path;
				std::string tmp;
				ss >> tmp;
				if (ss) {
					throw std::runtime_error("unexpected additional content after page path of error_page");
				}
				else if (path.back() != ';') {
					throw std::runtime_error("missing final semicolon at the end of error_page");
				}
				this->parseErrorPage(value, path.substr(0, path.size() - 1));
			}
			else if (key == "location") {
				std::string bracket;
				ss >> bracket;
				std::string tmp;
				ss >> tmp;
				if (ss) {
					throw std::runtime_error("unexpected additional content after bracket of location");
				}
				else if (bracket != "{") {
					throw std::runtime_error("missing or invalid bracket at the end of location");
				}
				this->parseLocation(cfgFile, value);
			}
			else {
				throw (std::runtime_error("unknown key in configuration file (" + key + ")"));
			}
		}
		line.clear();
		ss.clear();
		key.clear();
		value.clear();
	}
	if (!closeServerBlock) {
		throw std::runtime_error("server block not closed with a bracket");
	}
}

void Parser::parsePort(const std::string &port)
{
	if (this->_port != -1) {
		throw (std::runtime_error("duplicate listen key"));
	}
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
	if (!this->_host.empty()) {
		throw (std::runtime_error("duplicate host key"));
	}
	this->_host = (host == "localhost" ? "127.0.0.1" : host);
}

void Parser::parseServerName(const std::string &serverName)
{
	if (this->_serverName != "default_name") {
		throw (std::runtime_error("duplicate server_name key"));
	}
	this->_serverName = serverName;
}

void Parser::parseRoot(const std::string &root)
{
	if (!this->_root.empty()) {
		throw (std::runtime_error("duplicate root key"));
	}
	this->_root = root;
}

void Parser::parseIndex(const std::string &index)
{
	if (!this->_index.empty()) {
		throw (std::runtime_error("duplicate index key"));
	}
	this->_index = index;
}

void Parser::parseMaxBodySize(const std::string &maxBoySize)
{
	if (this->_maxBodySize != -1) {
		throw (std::runtime_error("duplicate max_body_size key"));
	}
	for (size_t i = 0; i < maxBoySize.length(); i++) {
		if (!std::isdigit(maxBoySize[i])) {
			throw (std::runtime_error("max body size contains non-digital characters"));
		}
	}
	std::stringstream ss(maxBoySize);
	unsigned int nb;
	ss >> nb;
	if (ss.fail())
		throw (std::runtime_error("invalid max body size number"));
	this->_maxBodySize = nb;
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
	if (page.empty()) {
		throw (std::runtime_error("empty path error page"));
	}
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
		missingInfo += "server_name ";
	if (this->_root.empty())
		missingInfo += "root ";
	if (this->_index.empty())
		missingInfo += "index ";
	if (this->_maxBodySize < 0)
		missingInfo += "max_body_size ";
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
	std::cout << *this;
	std::vector<Location>::const_iterator it = this->_location.begin();
	while (it != this->_location.end()) {
		std::cout << *it;
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

const int &Parser::getMaxBodySize(void) const
{
	return (this->_maxBodySize);
}

const std::string Parser::getErrorPages(void) const
{
	std::string errorPages;
	std::map<int, std::string>::const_iterator it = this->_errorPage.begin();
	while (it != this->_errorPage.end()) {
		errorPages += "	- " + std::to_string(it->first) + ": " + it->second;
		if (++it != this->_errorPage.end()) {
			errorPages += "\n";
		}
	}
	if (errorPages.empty())
		errorPages = "	- no pages configured";
	return (errorPages);
}

const std::string Parser::getErrorPage(int code) const
{
	std::map<int, std::string>::const_iterator it = this->_errorPage.find(code);
	if (it != this->_errorPage.end()) {
		return (it->second);
	}
	return ("");
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
	o << CLR_GREENB << "> Server config: " << CLR_RESET << std::endl;
	o << CLR_GREEN;
	o << "	- listen: " << rhs.getPort() << std::endl;
	o << "	- host: " << rhs.getHost() << std::endl;
	o << "	- server_name: " << rhs.getServerName() << std::endl;
	o << "	- root: " << rhs.getRoot() << std::endl;
	o << "	- index: " << rhs.getIndex() << std::endl;
	o << CLR_RESET;
	o << CLR_REDB << "> Error pages: " << CLR_RESET << std::endl;
	o << CLR_RED;
	o << rhs.getErrorPages() << std::endl;
	o << CLR_RESET;
	return (o);
}
