#ifndef ROUTES_HPP
# define ROUTES_HPP

class Routes
{
	private:
		std::string	name;
		std::string	path;
		std::string	default_file;
		bool		auto_index;
		bool		GET;
		bool		POST;
		bool		DELETE;

	public:
}
std::ostream&	operator<<(std::ostream& os, Routes const &route);

#endif