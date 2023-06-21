/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 16:17:26 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/21 08:35:32 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <iostream>
#include <fstream>
#include "Socket.hpp"

int main(int ac, char **av)
{
	if (ac != 2) {
		std::cerr << "Usage: ./webserv <config_file>" << std::endl;
		return (1);
	}

	try {
		std::string cfgFile = av[1];

		Socket socket;
		socket.launch();

		std::ifstream file("www/html/index.html");
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(content.length()) + "\n\n" + content;
		const char *hello = response.c_str();

		while (1)
		{
			socket.acceptConnection();
			int newSocket = socket.getNewServerFd();
			write(newSocket, hello , strlen(hello));
			close(newSocket);
		}
	}
	catch (std::exception &e) {
		std::cout << "Error: " << e.what() << "." << std::endl;
		return (1);
	}

	return (0);
}
