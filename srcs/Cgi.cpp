/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 10:07:53 by fluchten          #+#    #+#             */
/*   Updated: 2023/08/01 11:36:29 by fluchten         ###   ########.fr       */
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
		for (size_t i = 0; ws.env[i]; i++) {
			std::string line = ws.env[i];
			size_t separator = line.find("=");
			if (separator != std::string::npos) {
				std::string headerName = line.substr(0, separator);
				std::string headerValue = line.substr(separator + 1, line.size() - separator - 1);
				this->_envCgi[headerName] = headerValue;
			}
		}
	}
	else if (client._method == GET) {
		this->_envCgi["QUERY_STRING"] = client._query.substr(1, client._query.size());
		this->_envCgi["REQUEST_METHOD"] = "GET";
	}
	this->_envCgi["PATH_INFO"] = this->_cgiScript;
	this->_envCgi["HTTP_ACCEPT"] = client._headers["Accept"];
	this->_envCgi["HTTP_ACCEPT_LANGUAGE"] = client._headers["Accept-Language"];
	this->_envCgi["HTTP_ACCEPT_ENCODING"] = client._headers["Accept-Encoding"];
	this->_envCgi["HTTP_ACCEPT_CHARSET"] = client._headers["Accept-Charset"];
	this->_envCgi["HTTP_COOKIE"] = client._cookie;
	this->_envCgi["HTTP_USER_AGENT"] = client._headers["User-Agent"];
	this->_envCgi["GETEWAY_INTERFACE"] = "CGI/1.0";
	this->_envCgi["SERVER_NAME"] = client._config.getServerName();
	this->_envCgi["SERVER_PORT"] = std::to_string(client._config.getPort());
	this->_envCgi["SERVER_PROTOCOL"] = client._httpVersion;
	this->_envCgi["SERVER_SOFTWARE"] = "webserv/1.0";
}

Cgi::~Cgi(void)
{
	// std::cout << "Cgi destructor called" << std::endl;
}

char **Cgi::getenv() const
{
	char **env = new char*[_envCgi.size() + 1];
	int i = 0;
	for(std::map<std::string, std::string>::const_iterator j = _envCgi.begin(); j != _envCgi.end(); j++) {
		std::string res = j->first + "=" + j->second;
		env[i] = new char[res.size() + 1];
		env[i] = strcpy(env[i], (const char*)res.c_str());
		i++;
	}
	env[i] = NULL;
	return env;
}

char **Cgi::arg(Client &client)
{
	char **argv;
	if (_cgiScript.find(".py") != std::string::npos)
	{
		std::string truc = client._bodyReq.str();
		std::string _login = truc.substr(truc.find('=') + 1);


		argv = new char *[3];
		argv[0] = new char[strlen(_cgiScript.c_str()) + 1];
		argv[1] = new char[strlen(_login.c_str()) + 1];

		std::strcpy(argv[0], _cgiScript.c_str());
		std::strcpy(argv[1], _login.c_str());

		argv[2] = 0;
	}
	else
	{
		argv = new char *[3];
		argv[0] = new char[_cgiPath.size() + 1];
		std::strcpy(argv[0], _cgiPath.c_str());

		argv[1] = new char[strlen(_cgiScript.c_str()) + 1];
		std::strcpy(argv[1], _cgiScript.c_str());
		argv[2] = 0;
	}

	return (argv);
}

