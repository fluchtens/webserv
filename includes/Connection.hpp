/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 08:33:22 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/27 13:32:23 by fluchten         ###   ########.fr       */
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

		//Fonctionnement coté client
		Client *findExistingClient(const sockaddr_in& csin);

		/// @brief Check si la requete client est conforme
		/// @param request request du client a check.
		/// @return bool true si confrome
		bool checkRequest(char *request);

		/// @brief Gestion de la request parse afin de generer la reponse
		/// @param client 

		void handlePOST(Client& client);
		void handleDELETE(Client& client);

		Location *findLocationForUri(const std::string& uri, const std::vector<Location>& locations);
		void executeCGI(Client &client, Location *location);

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
		/* Utils */
		void addToFdSet(int fd, fd_set &fds);
		void checkFdStatus(void);
		bool isAlive(Client &client, bool isAlive);
		void closeClientSockets(void);
		std::string getFilePath(Client &client);
		std::string getFilePath(Client &client, Location *location);
		std::string getAbsolutePath(Client &client, Location *location);
		Location *getLocation(Client &client);
};

#endif
