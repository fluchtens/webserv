/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:13:40 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/20 13:04:24 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

/* ************************************************************************** */
/*                               Canonical form                               */
/* ************************************************************************** */

Location::Location(std::ifstream &cfgFile, const std::string &url)
{
	// std::cout << "Location constructor called" << std::endl;
	this->_url = url;
	this->_autoIndex = false;
	this->parseLocation(cfgFile);
	// this->hasAllInfos();
}

Location::Location(const Location &rhs)
{
	// std::cout << "Location copy constructor called" << std::endl;
	*this = rhs;
}

Location &Location::operator=(const Location &rhs)
{
	// std::cout << "Location copy assignment operator called" << std::endl;
	if (this != &rhs) {
		this->_allow = rhs._allow;
		this->_url = rhs._url;
		this->_root = rhs._root;
		this->_index = rhs._index;
		this->_path = rhs._path;
		this->_autoIndex = rhs._autoIndex;
		this->_return = rhs._return;
		this->_cgiScript = rhs._cgiScript;
		this->_cgiPath = rhs._cgiPath;
	}
	return (*this);
}

Location::~Location(void)
{
	// std::cout << "Location destructor called" << std::endl;
	return ;
}

/* ************************************************************************** */
/*                          Private Member functions                          */
/* ************************************************************************** */

void Location::parseLocation(std::ifstream &cfgFile)
{
	std::string line;
	bool closeLocationBlock = false;

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
			continue ;
		}

		if (key == "}") {
			closeLocationBlock = true;
			std::string tmp;
			ss >> tmp;
			if (ss) {
				throw std::runtime_error("unexpected additional content after location close bracket");
			}
			break;
		}

		if (key != "allow") {
			std::string tmp;
			ss >> tmp;
			if (ss) {
				throw std::runtime_error("unexpected additional content after value of " + key);
			}
		}

		if (value.back() != ';' && key != "allow") {
			throw std::runtime_error("missing final semicolon at the end of the value of " + key);
		}

		std::string tmp = value.substr(0, value.size() - 1);
		// std::cout << key << " = " << value << std::endl;

		if (key == "allow") {
			this->parseAllowedMethods(value, ss);
		}
		else if (key == "root") {
			parseRoot(tmp);
		}
		else if (key == "index") {
			parseIndex(tmp);
		}
		else if (key == "path") {
			parsePath(tmp);
		}
		else if (key == "autoindex") {
			parseAutoIndex(tmp);
		}
		else if (key == "return") {
			parseReturn(tmp);
		}
		else if (key == "cgi_script") {
			parseCgiScript(tmp);
		}
		else if (key == "cgi_path") {
			parseCgiPath(tmp);
		}
		else {
			throw (std::runtime_error("unknown key in configuration file (" + key + ")"));
		}
		line.clear();
		ss.clear();
		key.clear();
		value.clear();
	}
	if (!closeLocationBlock) {
		throw std::runtime_error("location block not closed with a bracket");
	}
}

void Location::parseAllowedMethods(const std::string &value, std::stringstream &ss)
{
	if (this->_allow.size()) {
		throw (std::runtime_error("duplicate allow key"));
	}
	std::string method;
	std::string tmp;

	while (ss >> method) {
		if (!ss.eof()) {
			tmp = method;
		} else {
			if (method.back() != ';') {
				throw (std::runtime_error("missing final semicolon at the end of method"));
			}
			tmp = method.substr(0, method.size() - 1);
		}
		if (!this->isValidMethod(tmp)) {
			throw (std::runtime_error("unknown allow method (" + tmp + ")"));
		} else {
			if (std::find(this->_allow.begin(), this->_allow.end(), tmp) != this->_allow.end()) {
				throw (std::runtime_error("duplicate allowed method"));
			}
			this->_allow.push_back(tmp);
		}
	}

	if (method.empty()) {
		if (value.back() != ';') {
			throw (std::runtime_error("missing final semicolon at the end of method"));
		}
		tmp = value.substr(0, value.size() - 1);
	} else {
		tmp = value;
	}
	if (!this->isValidMethod(tmp)) {
		throw (std::runtime_error("unknown allow method (" + tmp + ")"));
	} else {
		if (std::find(this->_allow.begin(), this->_allow.end(), tmp) != this->_allow.end()) {
			throw (std::runtime_error("duplicate allowed method"));
		}
		this->_allow.push_back(tmp);
	}
}

void Location::parseRoot(const std::string &root)
{
	if (!this->_root.empty()) {
		throw (std::runtime_error("duplicate root key"));
	}
	this->_root = root;
}

void Location::parseIndex(const std::string &index)
{
	if (!this->_index.empty()) {
		throw (std::runtime_error("duplicate index key"));
	}
	this->_index = index;
}

void Location::parsePath(const std::string &path)
{
	if (!this->_path.empty()) {
		throw (std::runtime_error("duplicate path key"));
	}
	this->_path = path;
}

void Location::parseAutoIndex(const std::string &autoIndex)
{
	if (this->_autoIndex) {
		throw (std::runtime_error("duplicate autoindex key"));
	}

	if (autoIndex == "on") {
		this->_autoIndex = true;
	} else {
		throw (std::runtime_error("invalid value of autoindex"));
	}
}

void Location::parseReturn(const std::string &ret)
{
	if (!this->_return.empty()) {
		throw (std::runtime_error("duplicate return key"));
	}
	this->_return = ret;
}

void Location::parseCgiScript(const std::string &cgiScript)
{
	if (!this->_cgiScript.empty()) {
		throw (std::runtime_error("duplicate cgi_script key"));
	}
	this->_cgiScript = cgiScript;
}

void Location::parseCgiPath(const std::string &cgiPath)
{
	if (!this->_cgiPath.empty()) {
		throw (std::runtime_error("duplicate cgi_path key"));
	}
	this->_cgiPath = cgiPath;
}

void Location::hasAllInfos(void)
{
	std::string	missingInfo;

	if (!this->_allow.size())
		missingInfo += "allow ";
	if (this->_root.empty())
		missingInfo += "root ";
	if (this->_index.empty())
		missingInfo += "index ";
	missingInfo = this->strTrimWhiteSpaces(missingInfo);
	if (!missingInfo.empty()) {
		throw (std::runtime_error("location missing information (" + missingInfo + ")"));
	}
}

bool Location::isValidMethod(const std::string &method) const
{
	if (method != "GET" && method != "POST" && method != "DELETE") {
		return (false);
	}
	return (true);
}

std::string Location::strTrimWhiteSpaces(const std::string &str)
{
	size_t first = str.find_first_not_of(" \t");
	if (first == std::string::npos)
		return ("");
	size_t last = str.find_last_not_of(" \t");
	return (str.substr(first, last - first + 1));
}

/* ************************************************************************** */
/*                              Getters / Setters                             */
/* ************************************************************************** */

const std::string &Location::getUrl(void) const
{
	return (this->_url);
}

const std::vector<std::string> &Location::getAllow(void) const
{
	return (this->_allow);
}

const std::string &Location::getRoot(void) const
{
	return (this->_root);
}

const std::string &Location::getIndex(void) const
{
	return (this->_index);
}

const std::string &Location::getPath(void) const
{
	return (this->_path);
}

const bool &Location::getAutoIndex(void) const
{
	return (this->_autoIndex);
}

const std::string &Location::getReturn(void) const
{
	return (this->_return);
}

const std::string &Location::getCgiScript(void) const
{
	return (this->_cgiScript);
}

const std::string &Location::getCgiPath(void) const
{
	return (this->_cgiPath);
}

bool Location::isMethodAllowed(std::string method) const
{
	for (std::vector<std::string>::const_iterator it = _allow.begin(); it != _allow.end(); it++)
	{
		if (*it == method)
			return (true);
	}
	return (false);
}

/* ************************************************************************** */
/*                             Operators Overloads                            */
/* ************************************************************************** */

std::ostream &operator<<(std::ostream &o, const Location &rhs)
{
	o << "> Location: ";
	o << "url: " << rhs.getUrl() << " | ";
	// o << "allow: " << rhs.getAllow() << " | ";
	o << "root: " << rhs.getRoot() << " | ";
	o << "index: " << rhs.getIndex() << " | ";
	o << "path: " << rhs.getPath() << " | ";
	o << "autoindex: " << rhs.getAutoIndex() << " | ";
	o << "return: " << rhs.getReturn() << " | ";
	o << "cgi_script: " << rhs.getCgiScript() << " | ";
	o << "cgi_path: " << rhs.getCgiPath() << " | ";
	o << std::endl;
	return (o);
}
