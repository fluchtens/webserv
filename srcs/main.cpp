/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 16:17:26 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/25 14:43:06 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <iostream>
#include <fstream>
#include "File.hpp"
#include "Parser.hpp"
#include "Socket.hpp"

int main(int ac, char **av)
{
	if (ac != 2) {
		std::cerr << "Usage: ./webserv <config_file>" << std::endl;
		return (1);
	}

	try {
		File config(static_cast<std::string>(av[1]));
		std::vector<Parser *> cfg;
		std::vector<Socket *> server;
		int serverCount = 1;

		for (int i = 0; i < serverCount; i++) {
			Parser *tmp = new Parser(static_cast<std::string>(av[1]));
			cfg.push_back(tmp);
		}

		cfg[0]->printParser();

		for (int i = 0; i < serverCount; i++) {
			Socket *tmp = new Socket(cfg[i]);
			server.push_back(tmp);
		}

		std::ifstream file("www/html/index.html");
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(content.length()) + "\n\n" + content;
		const char *hello = response.c_str();

		while (1)
		{
			server[0]->acceptConnection();
			int newSocket = server[0]->getNewServerFd();
			write(newSocket, hello, strlen(hello));
			close(newSocket);
		}
	}
	catch (std::exception &e) {
		std::cout << "Error: " << e.what() << "." << std::endl;
		return (1);
	}

	return (0);
}
