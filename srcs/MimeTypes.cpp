/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fluchten <fluchten@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 19:19:08 by fluchten          #+#    #+#             */
/*   Updated: 2023/07/27 13:43:39 by fluchten         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MimeTypes.hpp"

/* ************************************************************************** */
/*                           Constructor Destructor                           */
/* ************************************************************************** */

MimeTypes::MimeTypes(void)
{
	// std::cout << "MimeTypes default constructor called" << std::endl;
	this->initCommonMimeTypes();
}

MimeTypes::~MimeTypes(void)
{
	// std::cout << "MimeTypes destructor called" << std::endl;
}

/* ************************************************************************** */
/*                          Private Member functions                          */
/* ************************************************************************** */

void MimeTypes::initCommonMimeTypes(void)
{
	this->_mimeTypes.insert(std::make_pair(".aac", "audio/aac"));
	this->_mimeTypes.insert(std::make_pair(".abw", "application/x-abiword"));
	this->_mimeTypes.insert(std::make_pair(".arc", "application/x-freearc"));
	this->_mimeTypes.insert(std::make_pair(".avif", "image/avif"));
	this->_mimeTypes.insert(std::make_pair(".avi", "video/x-msvideo"));
	this->_mimeTypes.insert(std::make_pair(".azw", "application/vnd.amazon.ebook"));
	this->_mimeTypes.insert(std::make_pair(".bin", "application/octet-stream"));
	this->_mimeTypes.insert(std::make_pair(".bmp", "image/bmp"));
	this->_mimeTypes.insert(std::make_pair(".bz", "application/x-bzip"));
	this->_mimeTypes.insert(std::make_pair(".bz2", "application/x-bzip2"));
	this->_mimeTypes.insert(std::make_pair(".cda", "application/x-cdf"));
	this->_mimeTypes.insert(std::make_pair(".csh", "application/x-csh"));
	this->_mimeTypes.insert(std::make_pair(".css", "text/css"));
	this->_mimeTypes.insert(std::make_pair(".csv", "text/csv"));
	this->_mimeTypes.insert(std::make_pair(".doc", "application/msword"));
	this->_mimeTypes.insert(std::make_pair(".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"));
	this->_mimeTypes.insert(std::make_pair(".eot", "application/vnd.ms-fontobject"));
	this->_mimeTypes.insert(std::make_pair(".epub", "application/epub+zip"));
	this->_mimeTypes.insert(std::make_pair(".gz", "application/gzip"));
	this->_mimeTypes.insert(std::make_pair(".gif", "image/gif"));
	this->_mimeTypes.insert(std::make_pair(".html", "text/html"));
	this->_mimeTypes.insert(std::make_pair(".ico", "image/vnd.microsoft.icon"));
	this->_mimeTypes.insert(std::make_pair(".ics", "text/calendar"));
	this->_mimeTypes.insert(std::make_pair(".jar", "application/java-archive"));
	this->_mimeTypes.insert(std::make_pair(".jpeg", "image/jpeg"));
	this->_mimeTypes.insert(std::make_pair(".jpg", "image/jpeg"));
	this->_mimeTypes.insert(std::make_pair(".js", "text/javascript"));
	this->_mimeTypes.insert(std::make_pair(".json", "application/json"));
	this->_mimeTypes.insert(std::make_pair(".jsonld", "application/ld+json"));
	this->_mimeTypes.insert(std::make_pair(".mid", "audio/midi"));
	this->_mimeTypes.insert(std::make_pair(".midi", "audio/midi"));
	this->_mimeTypes.insert(std::make_pair(".mjs", "text/javascript"));
	this->_mimeTypes.insert(std::make_pair(".mp3", "audio/mpeg"));
	this->_mimeTypes.insert(std::make_pair(".mp4", "video/mp4"));
	this->_mimeTypes.insert(std::make_pair(".mpeg", "video/mpeg"));
	this->_mimeTypes.insert(std::make_pair(".mpkg", "application/vnd.apple.installer+xml"));
	this->_mimeTypes.insert(std::make_pair(".odp", "application/vnd.oasis.opendocument.presentation"));
	this->_mimeTypes.insert(std::make_pair(".ods", "application/vnd.oasis.opendocument.spreadsheet"));
	this->_mimeTypes.insert(std::make_pair(".odt", "application/vnd.oasis.opendocument.text"));
	this->_mimeTypes.insert(std::make_pair(".oga", "audio/ogg"));
	this->_mimeTypes.insert(std::make_pair(".ogv", "video/ogg"));
	this->_mimeTypes.insert(std::make_pair(".ogx", "application/ogg"));
	this->_mimeTypes.insert(std::make_pair(".opus", "audio/opus"));
	this->_mimeTypes.insert(std::make_pair(".otf", "font/otf"));
	this->_mimeTypes.insert(std::make_pair(".png", "image/png"));
	this->_mimeTypes.insert(std::make_pair(".pdf", "application/pdf"));
	this->_mimeTypes.insert(std::make_pair(".php", "application/x-httpd-php"));
	this->_mimeTypes.insert(std::make_pair(".ppt", "application/vnd.ms-powerpoint"));
	this->_mimeTypes.insert(std::make_pair(".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"));
	this->_mimeTypes.insert(std::make_pair(".rar", "application/vnd.rar"));
	this->_mimeTypes.insert(std::make_pair(".rtf", "application/rtf"));
	this->_mimeTypes.insert(std::make_pair(".sh", "application/x-sh"));
	this->_mimeTypes.insert(std::make_pair(".svg", "image/svg+xml"));
	this->_mimeTypes.insert(std::make_pair(".tar", "application/x-tar"));
	this->_mimeTypes.insert(std::make_pair(".tif", "image/tiff"));
	this->_mimeTypes.insert(std::make_pair(".tiff", "image/tiff"));
	this->_mimeTypes.insert(std::make_pair(".ts", "video/mp2t"));
	this->_mimeTypes.insert(std::make_pair(".ttf", "font/ttf"));
	this->_mimeTypes.insert(std::make_pair(".txt", "text/plain"));
	this->_mimeTypes.insert(std::make_pair(".vsd", "application/vnd.visio"));
	this->_mimeTypes.insert(std::make_pair(".wav", "audio/wav"));
	this->_mimeTypes.insert(std::make_pair(".weba", "audio/webm"));
	this->_mimeTypes.insert(std::make_pair(".webm", "video/webm"));
	this->_mimeTypes.insert(std::make_pair(".webp", "image/webp"));
	this->_mimeTypes.insert(std::make_pair(".woff", "font/woff"));
	this->_mimeTypes.insert(std::make_pair(".woff2", "font/woff2"));
	this->_mimeTypes.insert(std::make_pair(".xhtml", "application/xhtml+xml"));
	this->_mimeTypes.insert(std::make_pair(".xls", "application/vnd.ms-excel"));
	this->_mimeTypes.insert(std::make_pair(".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
	this->_mimeTypes.insert(std::make_pair(".xml", "application/xml"));
	this->_mimeTypes.insert(std::make_pair(".xul", "application/vnd.mozilla.xul+xml"));
	this->_mimeTypes.insert(std::make_pair(".zip", "application/zip"));
	this->_mimeTypes.insert(std::make_pair(".3gp", "video/3gpp"));
	this->_mimeTypes.insert(std::make_pair(".3g2", "video/3gpp2"));
	this->_mimeTypes.insert(std::make_pair(".7z", "application/x-7z-compressed"));
}

/* ************************************************************************** */
/*                          Public Member functions                           */
/* ************************************************************************** */

const std::string MimeTypes::getType(const std::string &filePath) const
{
	size_t dotPos = filePath.find_last_of('.');
	if (dotPos == std::string::npos) {
		return ("application/octet-stream");
	}
	std::string extension = filePath.substr(dotPos);
	std::map<std::string, std::string>::const_iterator it = this->_mimeTypes.find(extension);
	if (it != this->_mimeTypes.end()) {
		return (it->second);
	}
	return ("application/octet-stream");
}