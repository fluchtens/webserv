/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 18:03:09 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/24 19:06:21 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_HPP
# define HTTP_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <map>
# include "Client.hpp"

/* Requests */
void parseHttpRequest(Client &client);

/* Responses */
void createHttpResponse(Client &client, int statusCode, const std::string &contentType);
void sendHttpResponse(Client &client);
void sendHttpErrorResponse(Client &client, int errorCode);

#endif
