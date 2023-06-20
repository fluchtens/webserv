/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 16:17:26 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/20 10:39:37 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int ac, char **av)
{
	if (ac != 2) {
		std::cerr << "Usage: ./webserv <config_file>" << std::endl;
		return (1);
	}

	try {
		std::string cfgFile = av[1];
		std::cout << "cfgFile = " << cfgFile << std::endl;

		int kq = kqueue();
		if (kq < 0) {
			std::cerr << "kqueue error" << std::endl;
			exit(1);
		}
	}
	catch (std::exception &e) {
		std::cout << "Error: " << e.what() << std::endl;
		return (1);
	}

	return (0);
}
