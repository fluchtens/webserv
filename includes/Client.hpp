/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 17:58:47 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/21 12:53:52 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <netinet/tcp.h>
#include <sys/socket.h>
#include "Server.hpp"
#include "Parser.hpp"
#include "HTTPRequest.hpp"

enum HTTPMethod {
    GET,
    POST,
    DELETE,
    UNKNOWN
};

struct Client
{
	Client(Parser &config, Server &server, std::vector<Location> &location);
	Client (const Client &rhs);
	Client &operator=(const Client &rhs);
	~Client(void);

	Parser &_config;
	Server &_server;
    std::vector<Location> &_location;

	bool _isAlive;

	int _socket;
	sockaddr_storage _csin;
	socklen_t _crecSize;
	timeval _lastGetTime;

	std::stringstream _requestStr;
	ssize_t _contentLenght;
	ssize_t _sizeBody;
	bool _requestPars;

    HTTPMethod _method;
    std::string _httpVersion;
    std::string _uri;
	std::string _query;

    std::map<std::string, std::string>			_headers;
	std::stringstream							_bodyReq;
	std::string                                 _cookie;

	// Reponse du serveur
	std::string									_filePath;
	std::string									_bodyRep;
	std::string									_response;
	ssize_t										_sizeSend;
	ssize_t										_sizeRep;

	//Autoindex
	std::vector<std::pair<std::pair<std::string, std::string>, int> > _autoIndex;
	
};

#endif
