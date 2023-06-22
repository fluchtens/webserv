/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:19:39 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/22 19:01:38 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "File.hpp"

/* ************************************************************************** */
/*                           Constructor Destructor                           */
/* ************************************************************************** */

File::File(const std::string &cfgFile) : _cfgFile(cfgFile)
{
	if (!this->_isValidExtension()) {
		throw (std::runtime_error("wrong configuration file"));
	} else {
		this->_readFile();
	}
}

File::~File(void)
{
	return ;
}

/* ************************************************************************** */
/*                          Private Member functions                          */
/* ************************************************************************** */

bool File::_isValidExtension(void)
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

void File::_readFile(void)
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
		this->_fileContent += line;

	}
	inputFile.close();
	this->_checkFile();
}

void File::_checkFile(void)
{
	if (this->_fileContent.empty()) {
		throw (std::runtime_error("configuration file is empty"));
	}
	else if (!this->_isBracketsClosed()) {
		throw (std::runtime_error("config file invalid syntax"));
	}
}

bool File::_isBracketsClosed(void)
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
