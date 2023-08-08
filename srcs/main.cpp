/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:19:39 by fluchten          #+#    #+#             */
/*   Updated: 2023/08/08 13:32:14 by fluchten         ###   ########.fr       */
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

WebServ ws;

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

	signal(SIGINT, signalHandler);
	signal(SIGQUIT, SIG_IGN);
	ws.env = env;

	try {
		for (int i = 0; i < serverBlockCount; i++) {
			Parser *tmp = new Parser(cfgFile);
			ws.configs.push_back(tmp);
		}

		for (int i = 0; i < serverBlockCount; i++) {
			Server *tmp = new Server(ws.configs[i]);
			ws.servers.push_back(tmp);
		}

		ws.connection = new Connection(ws.servers);
		while (true) {
			ws.connection->start();
		}
	}
	catch (std::exception &e) {
		printError(e.what());
		deleteServers();
		return (1);
	}

	return (0);
}
