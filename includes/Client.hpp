/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 17:58:47 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/21 18:55:45 by fluchten         ###   ########.fr       */
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
	Client(const Client &rhs);
	Client &operator=(const Client &rhs);
	~Client(void);

	Parser &_config;
	Server &_server;
    std::vector<Location> &_location;
	bool _isAlive;

	/* Client socket */
	int _socket;
	sockaddr_storage _csin;
	socklen_t _crecSize;
	timeval _lastGetTime;

	/* Client request */
	std::stringstream _requestStr;
	bool _requestPars;
    HTTPMethod _method;
    std::string _uri;
	std::string _query;
    std::string _httpVersion;
    std::map<std::string, std::string> _headers;
	int _contentLenght;
	std::string _cookie;
	std::stringstream _bodyReq;
	int _bodySize;

	std::string _filePath;
	std::string _bodyRep;
	std::string _response;
	int _sizeSend;
	int _sizeRep;
	std::vector<std::pair<std::pair<std::string, std::string>, int> > _autoIndex;
	
};

#endif
