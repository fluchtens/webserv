/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:19:39 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/25 09:13:48 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <vector>
#include "Utils.hpp"
#include "Connection.hpp"
#include "Parser.hpp"
#include "Server.hpp"
#include "WebServ.hpp"

char **_env;
WebServ ws;

void creatFileDeleteMethod()
{
	std::ofstream outFile;
    for (int i = 1; i <= 4; ++i)
	{
        std::stringstream fileName;
        fileName << "fichier" << i << ".txt";

        outFile.open("./assets/example/a_supprimer/" + fileName.str(), std::ios::binary | std::ios::out);
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

	_env = env;
	signal(SIGINT, signalHandler);

	try {
		for (int i = 0; i < serverBlockCount; i++) {
			Parser *tmp = new Parser(cfgFile);
			ws.configs.push_back(tmp);
		}

		for (int i = 0; i < serverBlockCount; i++) {
			Server *tmp = new Server(ws.configs[i]);
			ws.servers.push_back(tmp);
		}

		creatFileDeleteMethod();

		Connection connection(ws.servers);
		while (true) {
			connection.start();
		}
	}
	catch (std::exception &e) {
		printError(e.what());
		deleteServers();
		return (1);
	}

	return (0);
}
