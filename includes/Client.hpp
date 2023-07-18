/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 17:58:47 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/14 12:25:55 by fluchten         ###   ########.fr       */
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

    /* Socket et contexte d'adressage du client */
	int					_socket;
	sockaddr_storage	_csin;
	socklen_t			_crecSize;
	timeval				_lastGetTime;

	/* Le client va t'il vivre ou mourir */
	bool				_isAlive;

	// Request du client
	std::stringstream							_requestStr;
	ssize_t										_contentLenght;
	ssize_t										_sizeBody;
	bool										_requestPars;

	// Request HTTP parsee.
    std::string									_httpVersion;
    std::string									_uri;
    HTTPMethod									_method;
    std::map<std::string, std::string>			_headers;
	std::stringstream							_bodyReq;
	std::string                                 _cookie;
	std::string                                 _query;

	// Reponse du serveur
	std::string									_filePath;
	std::string									_bodyRep;
	std::string									_response;
	ssize_t										_sizeSend;
	ssize_t										_sizeRep;

	//Autoindex
	std::vector<std::pair<std::pair<std::string, std::string>, int> > _autoIndex;
	
    // Config du serveur
    Parser									&_config;
	Server										&_server;
    std::vector<Location>			&_location;
	
};//struct Client

#endif
