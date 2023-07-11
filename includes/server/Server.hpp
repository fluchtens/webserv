/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:36:09 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/05 18:13:46 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

# include <iostream>
# include <sstream>
# include <vector>
# include <fcntl.h>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/tcp.h>
# include "Parser.hpp"
 
class Server
{
	public :
		Server(Parser *config);
		~Server(void);

		const int				&getSocket(void) const;
		Server					&getServer(void);
		const std::string		&getHost(void) const;
		const unsigned int		&getPort(void) const;
		const std::string		&getServerName(void) const;
		const std::string		&getRoot(void) const;
		const std::string		&getIndex(void) const;
		const std::string		getErrorPage(int code) const;
		size_t					getNbrLocation(void) const;
		Parser					&getConfig(void);
		std::vector<Location>	&getLocation(void);
		const int				&getCurrentConnection(void) const;
		bool					hasCapacity(void) const;
		void					incrementCurrentConnection(void);
		void					decrementCurrentConnection(void);

	private :
		Parser					*_cfg;
		int						_currentConnection;
		int						_maxConnection;
		int						_serverFd;
		sockaddr_in				_address;
		std::vector<Location>	_location;
		size_t					_nbrLocation;

		void					creatSocket(void);
		void					bindSocket(void);
		void					listenTCP(void);
		void					closeSocket();
		in_addr_t				convertIpAddress(const std::string &str);
};

#endif
