/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 10:40:37 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/25 11:12:58 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <unistd.h>
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include "Parser.hpp"

class Socket
{
	private:
		Parser *_cfg;
		int _serverFd;
		int _newSocketFd;
		struct sockaddr_in _address;

		void _createSocket(void);
		void _bindSocket(void);
		void _listenConnection(void);

	public:
		Socket(Parser *cfg);
		~Socket(void);

		int getNewServerFd(void) const;
		void acceptConnection(void);
};

#endif
