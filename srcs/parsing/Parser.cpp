/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 09:09:25 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/22 19:37:36 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

/* ************************************************************************** */
/*                           Constructor Destructor                           */
/* ************************************************************************** */

Parser::Parser(const std::string &cfgFile) : _cfgFile(cfgFile)
{
	this->_openCfgFile();
}

Parser::~Parser(void)
{
	return ;
}

/* ************************************************************************** */
/*                          Private Member functions                          */
/* ************************************************************************** */

void Parser::_openCfgFile(void)
{
	std::ifstream inputFile(this->_cfgFile);
	if (!inputFile.is_open()) {
		throw (std::runtime_error("failed to open configuration file"));
	}

	std::string line;
	while (std::getline(inputFile, line))
	{
		if (line.empty())
			continue ;
		line = strTrimWhiteSpaces(line);
		if (line.find("listen") == 0)
			this->_setPort(line.substr(6));
		else if (line.find("host") == 0)
			this->_setHost(line.substr(4));
		else if (line.find("server_name") == 0)
			this->_setServerName(line.substr(11));
		else if (line.find("root") == 0)
			this->_setRoot(line.substr(4));
		else if (line.find("index") == 0)
			this->_setIndex(line.substr(5));
	}
	inputFile.close();
}

bool Parser::_checkCfgFileLine(const std::string &str)
{
	for (size_t i = 0; str[i]; i++) {
		if (str[i] == ' ') {
			return (false);
		}
	}
	if (str[str.length() - 1] != ';') {
		return (false);
	}
	return (true);
}

void Parser::_setPort(const std::string &str)
{
	std::string line = strTrimWhiteSpaces(str);
	if (!this->_checkCfgFileLine(line)) {
		throw (std::runtime_error("wrong listen line format"));
	}

	line = line.substr(0, line.length() - 1);
	for (size_t i = 0; line[i]; i++) {
		if (!std::isdigit(line[i])) {
			throw (std::runtime_error("invalid port"));
		}
	}

	std::istringstream iss(line);
	int port;

	iss >> port;
	if (iss.fail() || (port < 1 || port > 65535)) {
		throw (std::runtime_error("invalid port"));
	}
	this->_port = port;
	std::cout << _port << std::endl;
}

void Parser::_setHost(const std::string &str)
{
	std::string line = strTrimWhiteSpaces(str);
	if (!this->_checkCfgFileLine(line)) {
		throw (std::runtime_error("wrong host line format"));
	}

	line = line.substr(0, line.length() - 1);
	if (line == "localhost") {
		this->_host = "127.0.0.1";
	} else {
		this->_host = line;
	}
	std::cout << _host << std::endl;
}

void Parser::_setServerName(const std::string &str)
{
	std::string line = strTrimWhiteSpaces(str);
	if (!this->_checkCfgFileLine(line)) {
		throw (std::runtime_error("wrong server_name line format"));
	}

	line = line.substr(0, line.length() - 1);
	this->_serverName = line;
	std::cout << _serverName << std::endl;
}

void Parser::_setRoot(const std::string &str)
{
	std::string line = strTrimWhiteSpaces(str);
	if (!this->_checkCfgFileLine(line)) {
		throw (std::runtime_error("wrong root line format"));
	}

	line = line.substr(0, line.length() - 1);
	this->_root = line;
	std::cout << _root << std::endl;
}

void Parser::_setIndex(const std::string &str)
{
	std::string line = strTrimWhiteSpaces(str);
	if (!this->_checkCfgFileLine(line)) {
		throw (std::runtime_error("wrong root line format"));
	}

	line = line.substr(0, line.length() - 1);
	this->_index = line;
	std::cout << _index << std::endl;
}
