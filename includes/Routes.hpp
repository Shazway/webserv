#ifndef ROUTES_HPP
# define ROUTES_HPP
#include <iostream>
# include <string>

class Routes
{
	private:
		std::string	_name;
		std::string	_path;
		std::string	_default_file;
		bool		_auto_index;
		bool		_GET;
		bool		_POST;
		bool		_DELETE;

	public:
        std::string getName();
        void        setName(std::string name);
        std::string getPath();
        void        setPath(std::string path);
        std::string getDefaultFile();
        void        setDefaultFile(std::string default_file);
        bool        getAutoindex();
        void        setAutoindex(bool index);
        bool        getGET();
        void        setGET(bool get);
        bool        getPOST();
        void        setPOST(bool post);
        bool        getDELETE();
        void        setDELETE(bool del);
};

std::ostream&	operator<<(std::ostream& os, Routes const &route);

#endif