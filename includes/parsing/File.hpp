/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   File.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/22 18:19:00 by fluchten          #+#    #+#             */
/*   Updated: 2023/06/22 18:51:48 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FILE_HPP
# define FILE_HPP

# include <iostream>
# include <fstream>
# include "utils.hpp"

class File
{
	private:
		std::string _cfgFile;
		std::string _fileContent;

		bool _isValidExtension(void);
		void _readFile(void);
		void _checkFile(void);
		bool _isBracketsClosed(void);

	public:
		File(const std::string &cfgFile);
		~File(void);
};

#endif
