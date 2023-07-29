/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 08:33:22 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/29 17:40:36 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <map>
# include <fcntl.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <sys/stat.h>
# include <netinet/in.h>
# include <netinet/tcp.h>
# include <dirent.h>
# include "Colors.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include "MimeTypes.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "Cgi.hpp"

class Connection
{
	public :
		Connection(std::vector<Server *> &servers);
		~Connection(void);

		void start(void);

	private :
		std::vector<Server *> _servers;
		std::vector<Client> _client;
		MimeTypes _mimeTypes;
		HttpRequest _httpRequest;
		HttpResponse _httpResponse;
		fd_set _setReads;
		fd_set _setWrite;
		fd_set _setErrors;
		int _highestFd;
		timeval _timeout;

		/* Initialization */
		void initConnection(void);
		void acceptSockets(void);
		void traitement(void);
		/* Requests */
		bool parseClientRequest(Client &client);
		/* Responses */
		bool handleReponse(Client &client);
		bool getRequest(Client& client);
		bool getRequestLocation(Client &client);
		void postRequest(Client& client);
		void deleteRequest(Client& client);
		/* CGI */
		void executeCGI(Client &client, Location *location);
		/* Utils */
		void addToFdSet(int fd, fd_set &fds);
		void checkFdStatus(void);
		bool isAlive(Client &client, bool isAlive);
		void closeClientSockets(void);
		std::string getIndexPath(Client &client);
		std::string getIndexPath(Client &client, Location *location);
		std::string getAbsolutePath(Client &client, Location *location);
		Location *getLocation(Client &client);
};

#endif
