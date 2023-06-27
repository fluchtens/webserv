/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:10:20 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/27 09:06:58 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <iostream>

// extern volatile bool serverStatus;

std::string strTrimWhiteSpaces(const std::string &str);
void signal_handler(int signal);

#endif
