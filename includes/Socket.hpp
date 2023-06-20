/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/20 10:40:37 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/20 12:34:35 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

class Socket
{
	private:
		struct sockaddr_in	_sockaddr;
		int					_fd;
		int					_domain;
		int					_service;
		int					_protocol;
		u_long				_interface;
		int					_port;

		void _createSocket(void);
		void _bindSocket(void);

	public:
		Socket(void);
		~Socket(void);
};

#endif
