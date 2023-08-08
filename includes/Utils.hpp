/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 19:42:51 by fluchten          #+#    #+#             */
/*   Updated: 2023/08/08 13:02:18 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <fstream>
# include <vector>
# include "WebServ.hpp"

extern WebServ ws;

template <typename T>
std::string convToString(const T &value) {
    std::ostringstream oss;
    oss << value;
    return (oss.str());
}

/* Errors */
void printError(const std::string &str);
void printWarning(const std::string &str);

/* Signals */
void signalHandler(int signal);

/* Utils */
void deleteServers(void);
bool isValidInputArgs(int ac, char **av, std::string &cfgFilePath);
bool isValidFileExtension(const std::string &cfgFile, const std::string &extension);
std::string strTrimWhiteSpaces(const std::string &str);
unsigned int countServerBlock(std::ifstream &cfgFile);

#endif
