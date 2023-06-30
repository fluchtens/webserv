/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 19:41:56 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/28 20:39:20 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void	delProg()
{
	//delete _server
	std::cout << "\033[34mDelete des serveurs : \033[0m\n";
	for (size_t i = 0; i < _server.size(); i++) {
        close(_server[i]->getSocket());
        delete _server[i];
    }
	//delete _config
	std::cout << "\033[34mDelete de la config : \033[0m\n";
	for (size_t i = 0; i < _config.size(); i++)
		delete _config[i];
}

bool isValidFileExtension(const std::string &cfgFile)
{
	size_t len = cfgFile.length();

	if (len >= 5) {
		if (cfgFile.substr(len - 5) == ".conf") {
			return (true);
		} else {
			return (false);
		}
	}
	return (false);
}

std::string strTrimWhiteSpaces(const std::string &str)
{
	size_t first = str.find_first_not_of(" \t");
	if (first == std::string::npos)
		return ("");
	size_t last = str.find_last_not_of(" \t");
	return (str.substr(first, last - first + 1));
}

unsigned int countServerBlock(std::ifstream &config_file)
{
	std::string line;
	int count = 0;

	while (!config_file.eof())
	{
		getline(config_file, line);
	
		std::stringstream ss;
		std::string key;
		std::string value;

		ss << line;
		ss >> key;
		ss >> value;

		if (key == "server" && value == "{") {
			count++;
		}
	}
	config_file.seekg(0);
	return (count);
}
