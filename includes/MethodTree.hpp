#ifndef METHODTREE_HPP
# define METHODTREE_HPP

# include <iostream>
# include <string>
# include <map>

typedef	struct s_allowedMethods
{
	bool	get;
	bool	post;
	bool	del;
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
		std::map<std::string, t_allowedMethods>	getTree() const;

	private:
		std::map<std::string, t_allowedMethods>	_subdirectories;

};

std::ostream &			operator<<( std::ostream & o, MethodTree const & i );

#endif /* ****************************************************** METHODTREE_H */