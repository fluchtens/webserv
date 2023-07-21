/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 10:27:07 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/21 17:43:36 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

# include "Client.hpp"

struct Client;

class HTTPRequest
{
	public:
    	HTTPRequest(Client &client);
		HTTPRequest(const HTTPRequest &rhs);
		HTTPRequest &operator=(const HTTPRequest &rhs);
		~HTTPRequest(void);

	private:
		Client &_client;

		void parseRequest(void);
		void parseRequestLine(std::stringstream &requestStream);
		void parseRequestHeader(std::stringstream &requestStream);
};

#endif
