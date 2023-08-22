/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 07:56:39 by fluchten          #+#    #+#             */
/*   Updated: 2023/08/22 19:13:50 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "Parser.hpp"
# include "Server.hpp"
# include "Connection.hpp"

class Connection;

struct WebServ
{
	std::vector<Parser *> configs;
	std::vector<Server *> servers;
	Connection *connection;
};

#endif
