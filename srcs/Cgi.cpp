/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 10:07:53 by fluchten          #+#    #+#             */
/*   Updated: 2023/08/07 08:34:41 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cgi.hpp"
#include "Utils.hpp"

/* ************************************************************************** */
/*                           Constructor Destructor                           */
/* ************************************************************************** */

Cgi::Cgi(Client &client, Location *location)
{
	// std::cout << "Cgi constructor called" << std::endl;
	this->_cgiScript = location->getCgiScript();
	this->_cgiPath = location->getCgiPath();

	if (client._method == POST) {
		this->_envCgi["CONTENT_LENGTH"] = std::to_string(client._contentLenght);
		this->_envCgi["CONTENT_TYPE"] = client._headers["Content-Type"];
		this->_envCgi["REQUEST_METHOD"] = "POST";
	}
	else if (client._method == GET) {
		if (!client._query.empty()) {
			this->_envCgi["QUERY_STRING"] = client._query.substr(1, client._query.size());
		}
		this->_envCgi["REQUEST_METHOD"] = "GET";
	}
	this->_envCgi["PATH_INFO"] = this->_cgiScript;
	this->_envCgi["HTTP_ACCEPT"] = client._headers["Accept"];
	this->_envCgi["HTTP_ACCEPT_LANGUAGE"] = client._headers["Accept-Language"];
	this->_envCgi["HTTP_ACCEPT_ENCODING"] = client._headers["Accept-Encoding"];
	this->_envCgi["HTTP_ACCEPT_CHARSET"] = client._headers["Accept-Charset"];
	this->_envCgi["HTTP_USER_AGENT"] = client._headers["User-Agent"];
	this->_envCgi["GETEWAY_INTERFACE"] = "CGI/1.0";
	this->_envCgi["SERVER_NAME"] = client._config.getServerName();
	this->_envCgi["SERVER_ADDR"] = client._config.getHost();
	this->_envCgi["SERVER_PORT"] = std::to_string(client._config.getPort());
	this->_envCgi["SERVER_PROTOCOL"] = client._httpVersion;
	this->_envCgi["SERVER_SOFTWARE"] = "webserv/1.0";
}

Cgi::~Cgi(void)
{
	// std::cout << "Cgi destructor called" << std::endl;
}

char **Cgi::getArgs(void)
{
	char **av = new char *[3];

	av[0] = new char[this->_cgiPath.size() + 1];
	std::strcpy(av[0], this->_cgiPath.c_str());

	av[1] = new char[this->_cgiScript.size() + 1];
	std::strcpy(av[1], this->_cgiScript.c_str());

	av[2] = NULL;
	return (av);
}

char **Cgi::getEnv(void)
{
	char **env = new char *[this->_envCgi.size() + 1];

	size_t i = 0;
	std::map<std::string, std::string>::const_iterator it = this->_envCgi.begin();
	while (it != this->_envCgi.end()) {
		std::string res = it->first + "=" + it->second;
		env[i] = new char[res.size() + 1];
		env[i] = std::strcpy(env[i], res.c_str());
		it++;
		i++;
	}
	env[i] = NULL;
	return (env);
}
