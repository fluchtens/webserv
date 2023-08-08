/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 10:08:02 by fluchten          #+#    #+#             */
/*   Updated: 2023/08/08 14:15:17 by fluchten         ###   ########.fr       */
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
		void exit(char **av, char **env);

	private:
		std::string _cgiScript;
		std::string _cgiPath;
		std::map<std::string, std::string> _envCgi;

		void createEnvironmentVariables(Client &client);
		void freeCharArray(char **array);
};

std::ostream &operator<<(std::ostream &o, const Cgi &rhs);

#endif
