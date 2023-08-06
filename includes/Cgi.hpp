/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 10:08:02 by fluchten          #+#    #+#             */
/*   Updated: 2023/08/06 11:28:02 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

# include <iostream>
# include <map>
# include "Client.hpp"
# include "Connection.hpp"
# include "WebServ.hpp"

class Cgi
{
	public:
		Cgi(Client &client, Location *location);
		~Cgi(void);

		char **getArgs(void);
		char **getEnv(void);

	private:
		std::string _cgiScript;
		std::string _cgiPath;
		std::map<std::string, std::string> _envCgi;
};

std::ostream &operator<<(std::ostream &o, const Cgi &rhs);

#endif
