#ifndef METHODTREE_HPP
# define METHODTREE_HPP

# include <iostream>
# include <string>
# include <set>

typedef	struct s_allowedMethods
{
	bool	GET;
	bool	POST;
	bool	DELETE;
}				t_allowedMethods;

class MethodTree
{

	public:

		MethodTree();
		MethodTree( MethodTree const & src );
		~MethodTree();

		MethodTree &		operator=( MethodTree const & rhs );

		bool	isAllowed(std::string method, std::string path);
		void	addExecption(std::string path, bool get, bool post, bool del);

	private:
		std::set<std::string, t_allowedMethods>	_subdirectories;

};

std::ostream &			operator<<( std::ostream & o, MethodTree const & i );

#endif /* ****************************************************** METHODTREE_H */