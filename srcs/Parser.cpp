/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 09:09:25 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/21 10:47:44 by fluchten         ###   ########.fr       */
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
	std::ifstream cfgFile(this->_cfgFile);
	if (!cfgFile.is_open()) {
		throw (std::runtime_error("failed to open config file"));
	}

	std::string line;
	while (std::getline(cfgFile, line)) {
		this->_fileContent += line;
		if (!cfgFile.eof()) {
			this->_fileContent += "\n";
		}
	}
	cfgFile.close();
	this->_checkCfgFile();
}

void Parser::_checkCfgFile(void)
{
	if (this->_fileContent.empty()) {
		throw (std::runtime_error("config file is empty"));
	}
	else if (!this->_isBracketsClosed()) {
		throw (std::runtime_error("config file invalid syntax"));
	}
}

bool Parser::_isBracketsClosed(void)
{
	int count = 0;

	for (size_t i = 0; i < this->_fileContent.length(); i++) {
		if (this->_fileContent[i] == '{') {
			count++;
		}
		if (this->_fileContent[i] == '}') {
			if (!count) {
				return (false);
			}
			count--;
		}
	}
	if (count) {
		return (false);
	}
	return (true);
}
