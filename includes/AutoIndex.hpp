/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 19:42:36 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/25 19:42:37 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOINDEX_HPP
#define AUTOINDEX_HPP

# include <iostream>
# include <dirent.h>
# include "Utils.hpp"
# include "Client.hpp"

void startAutoIndex(Client &client, std::string path);

#endif
