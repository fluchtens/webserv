/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 19:42:51 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/28 20:39:25 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include "Connection.hpp"
#include "Parser.hpp"
#include "Server.hpp"

extern std::vector<Server *>		_server;
extern std::vector<Parser *>	_config;
extern Connection					_connection;
extern volatile bool				boolStart;
extern char							**_env;

/* Errors */
void printError(const std::string &str);
void printWarning(const std::string &str);

/* Signals */
void signal_handler(int signal);

/* Utils */
void delProg(void);
bool isValidFileExtension(const std::string &cfgFile);
std::string strTrimWhiteSpaces(const std::string &str);
unsigned int countServerBlock(std::ifstream &cfgFile);

#endif
