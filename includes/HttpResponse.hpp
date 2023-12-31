/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:54:22 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/27 12:23:41 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <map>
# include <dirent.h>
# include "Client.hpp"
# include "Location.hpp"

class HttpResponse
{
	public :
		HttpResponse(void);
		~HttpResponse(void);

		/* Creating */
		void create(Client &client, int statusCode, const std::string &contentType);
		void createRedirection(Client &client, Location *location);
		void createAutoIndex(Client &client, std::string path);
		/* Sending */
		void sendResponse(Client &client);
		void sendError(Client &client, int errorCode);

	private :
		std::map<int, std::string> _httpCodes;

		/* Utils */
		void initHttpCodes(void);
		std::string getHttpMessage(int errorCode) const;
};

#endif
