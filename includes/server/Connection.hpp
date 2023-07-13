/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 08:33:22 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/13 20:52:49 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <sys/select.h>
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <map>
#include <sstream>
#include "Server.hpp"
#include "Client.hpp"
#include "HTTPRequest.hpp"
#include "Response.hpp"
#include "Autoindex.hpp"
#include "Cgi.hpp"
#include <vector>

#define BSIZE 1024
#define MAX_URI_SIZE 64

class Connection
{
	public :
		Connection(void);
		Connection(std::vector<Server *> &servers);
		Connection(const Connection &rhs);
		Connection &operator=(const Connection &rhs);
		~Connection(void);

		void initConnection(void);
		void acceptSockets(void);
		void traitement(void);

		//Fonctionnement coté client
		Client *findExistingClient(const sockaddr_in& csin);

		// @brief lit la requete client et la parse, gere les erreurs
		// @param client client en cours de vecteur<Client> (*it)
		// @return true si client toujours en vie.
		bool receiveClientRequest(Client &client);

		/// @brief Check si la requete client est conforme
		/// @param request request du client a check.
		/// @return bool true si confrome
		bool checkRequest(char *request);

		/// @brief Gestion de la request parse afin de generer la reponse
		/// @param client 
		bool handleReponse(Client &client);

		/// @brief gestion de Get
		bool handleGET(Client& client);
		bool hanglGetLocation(Client &client);
		/// @brief gestion de Post
		void handlePOST(Client& client);
		/// @brief gestion de Delete
		void handleDELETE(Client& client);

		std::string getFilePath(const Client &client);
		std::string getFilePath(const Client &client, const Location *location);
		std::string getMimeType(const std::string& filePath);
		Location *findLocationForUri(const std::string& uri, const std::vector<Location>& locations);
		void executeCGI(Client &client, Location *location);

	private :
		std::vector<Server *> _servers;
		std::vector<Client> _client;

		fd_set _setReads;
		fd_set _setWrite;
		fd_set _setErrors;
		int _highestFd;
		timeval _timeout;

		std::map<std::string, std::string> _mimeTypes;

		void initMime(void);
		void addToFdSet(int fd, fd_set &fds);
		void checkFdStatus(void);
		bool isAlive(Client &client, bool isAlive);
		void closeClientSockets(void);
};

#endif
