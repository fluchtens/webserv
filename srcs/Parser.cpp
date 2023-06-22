/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 09:09:25 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/22 18:09:42 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

/* ************************************************************************** */
/*                           Constructor Destructor                           */
/* ************************************************************************** */

Parser::Parser(const std::string &cfgFile) : _cfgFile(cfgFile)
{
	if (!this->_checkCfgFileExtension()) {
		throw (std::runtime_error("wrong file extension"));
	}
	this->_openCfgFile();
}

Parser::~Parser(void)
{
	return ;
}

/* ************************************************************************** */
/*                          Private Member functions                          */
/* ************************************************************************** */

bool Parser::_checkCfgFileExtension(void)
{
	size_t len = this->_cfgFile.length();
	if (len >= 5) {
		std::string extension = this->_cfgFile.substr(len - 5);

		if (extension == ".conf") {
			return (true);
		} else {
			return (false);
		}
	}
	return (false);
}

void Parser::_openCfgFile(void)
{
	std::ifstream cfgFile(this->_cfgFile);
	if (!cfgFile.is_open()) {
		throw (std::runtime_error("failed to open config file"));
	}

	std::string line;
	while (std::getline(cfgFile, line))
	{
		if (line.empty())
			continue ;

		line = strTrimWhiteSpaces(line);

		if (line.find("listen") == 0) {
			this->_setPort(line.substr(6));
		}

	}
	cfgFile.close();
	// this->_checkCfgFile();
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
	} else {
		this->_port = port;
	}
	std::cout << this->_port << std::endl;
}

// void Parser::_checkCfgFile(void)
// {
// 	if (this->_fileContent.empty()) {
// 		throw (std::runtime_error("config file is empty"));
// 	}
// 	else if (!this->_isBracketsClosed()) {
// 		throw (std::runtime_error("config file invalid syntax"));
// 	}
// }

// bool Parser::_isBracketsClosed(void)
// {
// 	int count = 0;

// 	for (size_t i = 0; i < this->_fileContent.length(); i++) {
// 		if (this->_fileContent[i] == '{') {
// 			count++;
// 		}
// 		if (this->_fileContent[i] == '}') {
// 			if (!count) {
// 				return (false);
// 			}
// 			count--;
// 		}
// 	}
// 	if (count) {
// 		return (false);
// 	}
// 	return (true);
// }
