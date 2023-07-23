/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 17:58:47 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/23 11:52:38 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <map>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include "Parser.hpp"
# include "Server.hpp"
# include "Location.hpp"

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

	// Classes
	Parser &_config;
	Server &_server;
    std::vector<Location> &_location;
	/* Client life */
	bool _isAlive;
	// Client socket
	int _socketFd;
	sockaddr_storage _socketAddress;
	socklen_t _socketAddrLen;
	// Client request
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
