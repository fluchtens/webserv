/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 08:33:22 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/12 11:59:29 by fluchten         ###   ########.fr       */
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
		void runSelect(void);

		//Fonctionnement coté client
		Client *findExistingClient(const sockaddr_in& csin);
		void acceptSocket();
		void traitement();
		void clearClientSockets();

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
		
		bool deadOrAlive(Client client, bool alive);

	private :
		std::vector<Server *> _servers;
		std::vector<Client> _client;
		int _maxFd;
		fd_set _read;
		fd_set _write;
		fd_set _error;
		timeval _timeout;
		std::map<std::string, std::string> _mimeTypes;

		void initMime(void);
		void addToFileDescriptorSet(int fd, fd_set &fds);
};

#endif
