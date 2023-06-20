/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 10:40:37 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/20 20:02:19 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

class Socket
{
	private:
		int _serverFd;
		int _newSocketFd;
		struct sockaddr_in _address;
		int _domain;
		int _service;
		int _protocol;
		u_long _interface;
		int _port;
		int _backlog;

		void _create(void);
		void _identify(void);
		void _waitingConnection(void);

	public:
		Socket(void);
		~Socket(void);

		int getNewServerFd(void) const;
		void acceptConnection(void);
};

#endif
