/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 09:07:39 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/22 18:09:57 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include "Utils.hpp"

class Parser
{
	private:
		std::string _cfgFile;
		std::string _fileContent;
		int _port;

		bool _checkCfgFileExtension(void);
		void _openCfgFile(void);

		bool _checkCfgFileLine(const std::string &str);
		void _setPort(const std::string &str);
		// void _checkCfgFile(void);
		// bool _isBracketsClosed(void);

	public:
		Parser(const std::string &_cfgFile);
		~Parser(void);
};

#endif
