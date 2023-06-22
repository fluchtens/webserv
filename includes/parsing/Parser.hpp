/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 09:07:39 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/22 19:36:54 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include "utils.hpp"

class Parser
{
	private:
		std::string _cfgFile;
		int _port;
		std::string _host;
		std::string _serverName;
		std::string _root;
		std::string _index;
		// std::string _errorPage;

		void _openCfgFile(void);
		bool _checkCfgFileLine(const std::string &str);
		void _setPort(const std::string &str);
		void _setHost(const std::string &str);
		void _setServerName(const std::string &str);
		void _setRoot(const std::string &str);
		void _setIndex(const std::string &str);

	public:
		Parser(const std::string &_cfgFile);
		~Parser(void);
};

#endif
