/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 17:36:09 by fluchten          #+#    #+#             */
/*   Updated: 2023/08/03 11:32:07 by fluchten         ###   ########.fr       */
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
# include <netinet/in.h>
# include <netinet/tcp.h>
# include "Parser.hpp"
 
class Server
{
	public :
		Server(Parser *config);
		Server(const Server &rhs);
		Server &operator=(const Server &rhs);
		~Server(void);

		const int &getSocket(void) const;
		Server &getServer(void);
		Parser &getConfig(void);
		std::vector<Location> &getLocation(void);

	private :
		Parser *_cfg;
		int _maxConnection;
		int _serverFd;
		sockaddr_in _address;
		std::vector<Location> &_location;

		void creatSocket(void);
		void configSocket(void);
		void bindSocket(void);
		void listenTCP(void);
		in_addr_t convertIpAddress(const std::string &str);
};

#endif
