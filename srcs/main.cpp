/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:19:39 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/05 09:58:51 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <vector>
#include "utils.hpp"
#include "Connection.hpp"
#include "Parser.hpp"
#include "Server.hpp"

std::vector<Server *> _server;
std::vector<Parser *> _config;
Connection _connection;
volatile bool serverStatus = true;
char **_env;

void creatFileDeleteMethod()
{
	std::ofstream outFile;
    for (int i = 1; i <= 4; ++i)
	{
        std::stringstream fileName;
        fileName << "fichier" << i << ".txt";

        outFile.open("./www/a_supprimer/" + fileName.str(), std::ios::binary | std::ios::out);
        if (outFile.is_open())
		{
            outFile << "Contenu du fichier " << i << std::endl;
            outFile.close();
        }
		else
            std::cerr << "Impossible de créer le fichier " << fileName.str() << std::endl;
    }
}

int main(int ac, char **av, char **env)
{
	signal(SIGINT, signal_handler);
	_env = env;

	std::string cfgFilePath;
	if (ac > 2) {
		printError("wrong number of input arguments");
		return (1);
	}
	else if (ac == 1) {
		printWarning("no file specified, use default configuration file");
		cfgFilePath = "config/default.conf";
	}
	else {
		if (!isValidFileExtension(static_cast<std::string>(av[1]))) {
			printError("invalid configuration file (.conf file required)");
			return (1);
		}
		cfgFilePath = static_cast<std::string>(av[1]);
	}

	std::ifstream cfgFile(cfgFilePath);
	if (!cfgFile.is_open()) {
		printError("failed to open configuration file");
		return (1);
	}

	int serverBlockCount = countServerBlock(cfgFile);
	if (!serverBlockCount) {
		printError("no block server found");
		return (1);
	}

	try {
		for (int i = 0; i < serverBlockCount; i++) {
			Parser *tmp = new Parser(cfgFile);
			_config.push_back(tmp);
		}

		for (int i = 0; i < serverBlockCount; i++) {
			Server *tmp = new Server(_config[i]);
			_server.push_back(tmp);
		}

		creatFileDeleteMethod();

		Connection tmp(_server);
		_connection = tmp;
		while (serverStatus)
		{
			_connection.initConnection();
			_connection.runSelect();
			_connection.acceptSocket();
			_connection.traitement();
		}
		delProg();
		std::cout << "Fin du Programme\n";
	}
	catch (std::exception &e) {
		printError(e.what());
		return (1);
	}

	return (0);
}
