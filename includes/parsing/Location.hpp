/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/30 16:13:21 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/30 16:14:56 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

class Location
{
	public:
		Location(std::ifstream &cfgFile, const std::string &url);
		Location(const Location &srcs);
		~Location();

		Location & operator=(const Location &srcs);

		const std::string &					getUrl() const;
		const std::vector<std::string> &	getAllow() const;
		const std::string &					getRoot() const;
		const std::string &					getIndex() const;
		const std::string &					getPath() const;
		const bool &						getAutoIndex() const;
		const std::string &					getReturn() const;
		const std::string &					getCgiPath() const;
		const std::string &					getCgiScript() const;
		const int &							getMaxSize() const;
		const bool &						getDeny() const;

		bool isMethodAllowed(std::string method) const;

	private:
		Location();
		std::vector<std::string>	_allow;
		std::string 				_url;
		std::string 				_root;
		std::string 				_index;
		std::string					_path;
		bool						_autoIndex;
		std::string					_return;
		std::string                 _cgiScript;
		std::string                 _cgiPath;
		int							_maxSize;
		bool						_deny;
};

#endif
