/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 19:42:51 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/25 09:12:33 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <fstream>
# include <vector>
# include "Parser.hpp"
# include "Server.hpp"
# include "Connection.hpp"
# include "WebServ.hpp"

extern char **_env;
extern WebServ ws;

/* Errors */
void printError(const std::string &str);
void printHttpError(const std::string &str, const int &code);
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
