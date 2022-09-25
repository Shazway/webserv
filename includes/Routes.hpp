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
		Routes();
		~Routes();
	std::string getName() const;
	void	setName(std::string name);
	std::string getPath() const;
	void	setPath(std::string path);
	std::string getDefaultFile() const;
	void	setDefaultFile(std::string default_file);
	bool	getAutoindex() const;
	void	setAutoindex(bool index);
	bool	getGET() const;
	void	setGET(bool get);
	bool	getPOST() const;
	void	setPOST(bool post);
	bool	getDELETE() const;
	void	setDELETE(bool del);
};

std::ostream&	operator<<(std::ostream& os, Routes const &route);

#endif