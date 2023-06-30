#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#define localhost "127.0.0.1"

class Parser
{
	public :
		class Location
		{
			public :
				Location(std::ifstream &cfgFile, const std::string &url);
				Location(const Location &srcs);
				~Location();

				Location & operator=(const Location &srcs);

				const std::string &					getUrl() const;
				const std::vector<std::string> &	getAllow() const;
				const std::string &					getRoot() const;
				const std::string &					getIndex() const;
				const std::string &					getPath() const;
				const bool &						getAutoIndex() const;
				const std::string &					getReturn() const;
				const std::string &					getCgiPath() const;
				const std::string &					getCgiScript() const;
				const int &							getMaxSize() const;
				const bool &						getDeny() const;

				bool isMethodAllowed(std::string method) const;

			private :
				Location();
				std::vector<std::string>	_allow;
				std::string 				_url;
				std::string 				_root;
				std::string 				_index;
				std::string					_path;
				bool						_autoIndex;
				std::string					_return;
                std::string                 _cgiScript;
                std::string                 _cgiPath;
				int							_maxSize;
				bool						_deny;
		};

		Parser(std::ifstream &cfgFile);
		~Parser(void);

		const unsigned int &getPort(void) const;
		const std::string &getHost(void) const;
		const std::string &getServerName(void) const;
		const std::string &getRoot(void) const;
		const std::string &getIndex(void) const;
		const std::string getErrorPage(int code) const;

		std::string              		getLocationUrl(size_t size) const;
		std::vector<std::string>		getLocationAllow(std::string url) const;
		std::string              		getLocationRoot(std::string url) const;
		std::string              		getLocationIndex(std::string url) const;
		std::string              		getLocationPath(std::string url) const;
		bool              				getLocationAutoIndex(std::string url) const;
		std::string              		getLocationReturn(std::string url) const;
		int			              		getLocationMaxSize(std::string url) const;
		bool              				getLocationDeny(std::string url) const;
		std::vector<Location>			&getLocation();
		size_t              			getNbrLocation() const;

	private :
		unsigned int _port;
		std::string _host;
		std::string _serverName;
		std::string _root;
		std::string _index;
		std::map<int, std::string> _errorPage;
		std::vector<Location> _location;
		size_t _nbrLocation;

		void parseCfgFile(std::ifstream &cfgFile);

		void parsePort(const std::string &port);
		void parseHost(const std::string &host);
		void parseServerName(const std::string &serverName);
		void parseRoot(const std::string &root);
		void parseIndex(const std::string &index);
		void parseErrorPage(const std::string &error, const std::string &page);
		void parseLocation(std::ifstream &cfgFile, const std::string &url);

		std::string strTrimWhiteSpaces(const std::string &str);
		void printParsing(void) const;
};

#endif
