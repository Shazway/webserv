#ifndef INDEXTREE_HPP
# define INDEXTREE_HPP

# include <iostream>
# include <string>
# include <map>
class IndexTree
{

	public:

		IndexTree();
		IndexTree( IndexTree const & src );
		~IndexTree();

		IndexTree &		operator=( IndexTree const & rhs );

		void	addExecption(std::string path, std::string index);
		std::map<std::string, std::string>	getTree() const;
		std::pair<std::string, std::string>	getClosestDirectory(std::string path) const;

	private:
		std::map<std::string, std::string>	_subdirectories;

};

std::ostream &			operator<<( std::ostream & o, IndexTree const & i );

#endif /* ******************************************************* INDEXTREE_H */