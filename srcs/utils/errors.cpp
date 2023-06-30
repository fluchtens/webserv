/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 18:05:36 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/28 18:05:42 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

void printError(const std::string &str)
{
	std::cerr << "\033[1;31m" << "Error: " << str << "." << "\033[0m" << std::endl;
}

void printWarning(const std::string &str)
{
	std::cerr << "\033[1;33m" << "Warning: " << str << "." << "\033[0m" << std::endl;
}
