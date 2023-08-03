/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 10:13:09 by fluchten          #+#    #+#             */
/*   Updated: 2023/08/03 11:06:53 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <map>
# include "Location.hpp"
# include "Colors.hpp"

class Parser
{
	public:
		Parser(std::ifstream &cfgFile);
		~Parser(void);

		const int &getPort(void) const;
		const std::string &getHost(void) const;
		const std::string &getServerName(void) const;
		const std::string &getRoot(void) const;
		const std::string &getIndex(void) const;
		const int &getMaxBodySize(void) const;
		const std::string getErrorPages(void) const;
		const std::string getErrorPage(int code) const;

		std::string getLocationUrl(size_t size) const;
		std::vector<std::string> getLocationAllow(std::string url) const;
		std::string getLocationRoot(std::string url) const;
		std::string getLocationIndex(std::string url) const;
		std::string getLocationPath(std::string url) const;
		bool getLocationAutoIndex(std::string url) const;
		std::string getLocationReturn(std::string url) const;
		std::vector<Location> &getLocation();
		size_t getNbrLocation() const;

	private:
		int _port;
		std::string _host;
		std::string _serverName;
		std::string _root;
		std::string _index;
		int _maxBodySize;
		std::map<int, std::string> _errorPage;
		std::vector<Location> _location;
		size_t _nbrLocation;

		void parseCfgFile(std::ifstream &cfgFile);
		void parsePort(const std::string &port);
		void parseHost(const std::string &host);
		void parseServerName(const std::string &serverName);
		void parseRoot(const std::string &root);
		void parseIndex(const std::string &index);
		void parseMaxBodySize(const std::string &maxBoySize);
		void parseErrorPage(const std::string &error, const std::string &page);
		void parseLocation(std::ifstream &cfgFile, const std::string &url);

		void hasAllInfos(void);
		std::string strTrimWhiteSpaces(const std::string &str);
		void printParsing(void) const;
};

std::ostream &operator<<(std::ostream &o, const Parser &rhs);

#endif
