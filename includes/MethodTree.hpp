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
		std::pair<std::string, t_allowedMethods>	getClosestDirectory(std::string path) const;

	private:
		std::map<std::string, t_allowedMethods>	_subdirectories;
	//Dans le cas ou deux subdirectories commencent de la même manière, on a un problème, faire une fonction qui s'en occupe
};

std::ostream &			operator<<( std::ostream & o, MethodTree const & i );

#endif /* ****************************************************** METHODTREE_H */