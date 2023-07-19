/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:19:39 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/19 17:53:26 by fluchten         ###   ########.fr       */
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
	signal(SIGINT, signalHandler);
	_env = env;

	std::string cfgFilePath;
	if (!isValidInputArgs(ac, av, cfgFilePath)) {
		return (1);
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

		// creatFileDeleteMethod();

		Connection tmp(_server);
		_connection = tmp;
		while (true)
		{
			_connection.initConnection();
			_connection.acceptSockets();
			_connection.traitement();
		}
	}
	catch (std::exception &e) {
		printError(e.what());
		deleteServers();
		return (1);
	}

	return (0);
}
