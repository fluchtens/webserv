/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 19:15:38 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/27 13:43:34 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIMETYPES_HPP
# define MIMETYPES_HPP

# include <iostream>
# include <map>

class MimeTypes
{
	public:
		MimeTypes(void);
		~MimeTypes(void);

		const std::string getType(const std::string &filePath) const;

	private:
		std::map<std::string, std::string> _mimeTypes;

		void initCommonMimeTypes(void);
};

#endif
