/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/21 10:27:07 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/21 13:03:41 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

# include <sstream>
// # include <map>
# include "Client.hpp"

struct Client;

class HTTPRequest
{
	public:
		HTTPRequest(void);
    	HTTPRequest(Client &client);
		HTTPRequest(const HTTPRequest &rhs);
		HTTPRequest &operator=(const HTTPRequest &rhs);
		~HTTPRequest(void);

		void parseRequest(Client &client);

	private:
		void parseRequestLine(Client &client, std::stringstream &requestStream);
};

#endif
