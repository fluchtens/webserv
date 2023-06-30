/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:13:40 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/30 16:20:27 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(std::ifstream &cfgFile, const std::string &url) : _url(url), _autoIndex(false), _deny(false)
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

Location::Location(const Location &srcs)
{ *this = srcs; }

Location::~Location()
{}

Location & Location::operator=(const Location & srcs)
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

const std::string & Location::getUrl() const
{ return (_url); }

const std::vector<std::string> & Location::getAllow() const
{ return (_allow); }

const std::string & Location::getRoot() const
{ return (_root); }

const std::string & Location::getIndex() const
{ return (_index); }

const std::string &	Location::getPath() const
{ return (_path); }

const bool & Location::getAutoIndex() const
{ return (_autoIndex); }

const std::string & Location::getReturn() const
{ return (_return); }

const std::string & Location::getCgiPath() const
{ return (_cgiPath); }

const std::string & Location::getCgiScript() const
{ return (_cgiScript); }

const int & Location::getMaxSize() const
{ return (_maxSize); }

const bool & Location::getDeny() const
{ return (_deny); }

bool Location::isMethodAllowed(std::string method) const
{
	for (std::vector<std::string>::const_iterator it = _allow.begin(); it != _allow.end(); it++)
	{
		if (*it == method)
			return (true);
	}
	return (false);
}