/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 18:05:13 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/13 19:34:57 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void signalHandler(int signal)
{
    if (signal == SIGINT){
    	std::cout << "SIGINT signal received. Program is shutting down..." << std::endl;
		serverStatus = false;
	}
	else if (signal == SIGPIPE) {
		std::cout << "Received SIGPIPE signal. Program is shutting down..." << std::endl;
		serverStatus = false;
	}
}
