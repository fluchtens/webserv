/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 08:54:22 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/26 09:12:55 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <map>
# include "Client.hpp"

class HttpRequest
{
	public :
		HttpRequest(void);
		~HttpRequest(void);

		void parse(Client &client);

	private :
		void parseRequestLine(Client &client, std::stringstream &requestStream);
		void parseRequestHeader(Client &client, std::stringstream &requestStream);
};

#endif
