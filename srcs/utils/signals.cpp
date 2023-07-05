/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 18:05:13 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/05 09:57:55 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void signal_handler(int signal)
{
    if (signal == SIGINT)
	{
    	std::cout << "Signal SIGINT reçu. Fermeture du programme..." << std::endl;
		serverStatus = false;
	}
	if (signal == SIGPIPE)
	{
		std::cout << "Signal SIGPIPE recu. Fermeture du programme..." << std::endl;
		serverStatus = false;
	}
}
