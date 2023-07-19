/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:13:21 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/19 18:19:45 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>

class Location
{
	public:
		Location(std::ifstream &cfgFile, const std::string &url);
		Location(const Location &rhs);
		Location &operator=(const Location &rhs);
		~Location(void);

		const std::string &getUrl(void) const;
		const std::vector<std::string> &getAllow(void) const;
		const std::string &getRoot(void) const;
		const std::string &getIndex(void) const;
		const std::string &getPath(void) const;
		const bool &getAutoIndex(void) const;
		const std::string &getReturn(void) const;
		const std::string &getCgiPath(void) const;
		const std::string &getCgiScript(void) const;
		const int &getMaxSize(void) const;

		bool isMethodAllowed(std::string method) const;

	private:
		std::string _url;
		std::vector<std::string> _allow;
		std::string _root;
		std::string _index;
		std::string _path;
		bool _autoIndex;
		std::string _return;
		std::string _cgiScript;
		std::string _cgiPath;
		int _maxSize;

		void parseLocation(std::ifstream &cfgFile);
};

std::ostream &operator<<(std::ostream &o, const Location &rhs);

#endif
