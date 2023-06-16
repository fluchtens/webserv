/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 16:17:26 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/16 16:18:57 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int main(int ac, char **av)
{
	if (ac != 2) {
		std::cerr << "Usage: ./webserv <config_file>" << std::endl;
		return (1);
	}

	(void) av;
	return (0);
}
