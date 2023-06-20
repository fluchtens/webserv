/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 16:17:26 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/20 19:52:09 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		const char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

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
