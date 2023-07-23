/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/23 11:40:39 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/23 13:02:45 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <map>
# include "utils.hpp"
# include "Client.hpp"

struct Client;

void createHttpResponse(Client &client, int statusCode, const std::string &contentType);
void sendHttpResponse(Client &client);
void sendErrorResponse(Client &client, int code);

#endif
