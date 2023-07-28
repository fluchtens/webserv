/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 19:41:56 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/28 15:58:23 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

void printError(const std::string &str)
{
	std::cerr << CLR_RED << "Error: " << str << "." << CLR_RESET << std::endl;
}

void printHttpError(const std::string &str, const int &code)
{
	std::cerr << CLR_RED << "Error " << code <<  ": " << str << CLR_RESET << std::endl;
}

void printWarning(const std::string &str)
{
	std::cout << CLR_YELLOW << "Warning: " << str << "." << CLR_RESET << std::endl;
}

void signalHandler(int signal)
{
    if (signal == SIGINT){
    	std::cout << "SIGINT signal received. Program is shutting down..." << std::endl;
		deleteServers();
		exit(1);
	}
	else if (signal == SIGPIPE) {
		std::cout << "Received SIGPIPE signal. Program is shutting down..." << std::endl;
		deleteServers();
		exit(1);
	}
}

void deleteServers(void)
{
	if (ws.connection) {
    	delete ws.connection;
	}
	for (size_t i = 0; i < ws.servers.size(); i++) {
        close(ws.servers[i]->getSocket());
        delete ws.servers[i];
    }
	for (size_t i = 0; i < ws.configs.size(); i++) {
		delete ws.configs[i];
	}
	std::remove("http_request_logs.txt");
	std::remove("http_response_logs.txt");
	std::system("rm -rf ./assets/base/tmp/");
}

bool isValidInputArgs(int ac, char **av, std::string &cfgFilePath)
{
	if (ac != 2) {
		printError("./webserv <config_file>");
		return (false);
	} else {
		if (!isValidFileExtension(static_cast<std::string>(av[1]), ".conf")) {
			printError("invalid configuration file (.conf file required)");
			return (false);
		}
		cfgFilePath = static_cast<std::string>(av[1]);
	}
	return (true);
}

bool isValidFileExtension(const std::string &cfgFile, const std::string &extension)
{
	size_t cfgFilelen = cfgFile.length();
	size_t extensionLen = extension.length();

	if (cfgFilelen >= extensionLen) {
		if (cfgFile.substr(cfgFilelen - extensionLen) == extension) {
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

void createTempDeleteFiles(void)
{
	std::string folderPath = "./assets/base/tmp/";
	mkdir(folderPath.c_str(), 0777);

	for (int i = 1; i <= 5; i++) {
		std::string filePath = folderPath + "file" + std::to_string(i) + ".txt";
		std::string fileContent = "Content of file" + std::to_string(i) + ".txt\n";
		std::ofstream file(filePath);
		if (file.is_open()) {
			file << fileContent;
			file.close();
		}
	}
}
