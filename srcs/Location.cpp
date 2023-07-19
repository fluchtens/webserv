/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:13:40 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/19 12:44:15 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

/* ************************************************************************** */
/*                               Canonical form                               */
/* ************************************************************************** */

Location::Location(std::ifstream &cfgFile, const std::string &url) : _url(url), _autoIndex(false)
{
	// std::cout << "Location constructor called" << std::endl;
	this->parseLocation(cfgFile);
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
		this->_maxSize = rhs._maxSize;
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

	while (std::getline(cfgFile, line))
	{
		if (line.empty())
			continue;

		std::stringstream ss(line);
		std::string key;
		std::string value;

		ss >> key;

		if (key == "}")
			break;

		while (ss >> value)
		{
			std::string tmp = value.substr(0, value.size() - 1);
			// std::cout << key << " = " << value << std::endl;

			if (key == "allow")
			{
				std::string	method;
				if (!ss.eof()) {
					method = value.substr(0, value.size());
				} else {
					method = value.substr(0, value.size() -1);
				}
				if (method != "GET" && method != "POST" && method != "DELETE") {
					throw std::runtime_error("Invalid value for allow key (" + method + ")");
				}
				this->_allow.push_back(method);
			}
			else if (key == "root")
				this->_root = tmp;
			else if (key == "index")
				this->_index = tmp;
			else if (key == "path")
				this->_path = tmp;
			else if (key == "autoindex")
			{
				if (tmp == "on") {
					this->_autoIndex = true;
				}
			}
			else if (key == "return")
				this->_return = tmp;
            else if (key == "cgi_path")
                this->_cgiPath = tmp;
			else if (key == "cgi_script")
                this->_cgiScript = tmp;
			else if (key == "client_max_body_size")
                this->_maxSize = std::atoi(tmp.c_str());
		}
		line.clear();
		ss.clear();
		key.clear();
		value.clear();
	}
}

/* ************************************************************************** */
/*                              Getters / Setters                             */
/* ************************************************************************** */

const std::vector<std::string> &Location::getAllow(void) const
{
	return (this->_allow);
}

const std::string &Location::getUrl(void) const
{
	return (this->_url);
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

const std::string &Location::getCgiPath(void) const
{
	return (this->_cgiPath);
}

const std::string &Location::getCgiScript(void) const
{
	return (this->_cgiScript);
}

const int &Location::getMaxSize(void) const
{
	return (this->_maxSize);
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
	o << "path: " << rhs.getUrl() << " | ";
	return (o);
}
