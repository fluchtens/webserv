/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:07:12 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/27 09:06:56 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string strTrimWhiteSpaces(const std::string &str)
{
	size_t first = str.find_first_not_of(" \t");
	if (first == std::string::npos) {
		return ("");
	}
	size_t last = str.find_last_not_of(" \t");

	return (str.substr(first, last - first + 1));
}

/* void signal_handler(int signal)
{
    if (signal == SIGINT) {
		std::cout << "Closing the server.." << std::endl;
		serverStatus = false;
	}
} */
