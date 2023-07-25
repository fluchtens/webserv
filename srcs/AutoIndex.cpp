/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 18:02:54 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/25 19:39:46 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AutoIndex.hpp"

void startAutoIndex(Client &client, std::string path)
{
	DIR *dir = opendir(path.c_str());
	if (!dir) {
		printHttpError("Not Found", 404);
		sendHttpErrorResponse(client, 404);
		return ;
	}

	std::string htmlContent = "<!DOCTYPE html>\r\n";
	htmlContent += "<html lang=\"en\">\r\n";
	htmlContent += "<head>\r\n";
	htmlContent += "\t<meta charset=\"UTF-8\">\r\n";
	htmlContent += "\t<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\r\n";
	htmlContent += "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n";
	htmlContent += "\t<title>Index of " + path + "</title>\r\n";
	htmlContent += "</head>\r\n";
	htmlContent += "<body>\r\n";
	htmlContent += "\t<h1>Index of " + path + "</h1>\r\n";
	htmlContent += "\t<ul>\r\n";

	struct dirent *ent;
	while ((ent = readdir(dir))) {
		htmlContent += "\t<li><a href=\"" + std::string(ent->d_name) + "\">" + std::string(ent->d_name) + "</a></li>\r\n";
	}

	htmlContent += "\t</ul>\r\n";
	htmlContent += "</body>\r\n";
	htmlContent += "</html>";

	closedir(dir);
	client._bodyResp = htmlContent;
	createHttpResponse(client, 200, "text/html");
	sendHttpResponse(client);
}
