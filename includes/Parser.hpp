/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 09:07:39 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/21 10:47:56 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
# define PARSER_HPP

# include <fstream>

class Parser
{
	private:
		std::string _cfgFile;
		std::string _fileContent;

		void _openCfgFile(void);
		void _checkCfgFile(void);
		bool _isBracketsClosed(void);

	public:
		Parser(const std::string &_cfgFile);
		~Parser(void);
};

#endif
