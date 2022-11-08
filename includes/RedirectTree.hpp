#ifndef REDIRECTTREE_HPP
# define REDIRECTTREE_HPP

# include <iostream>
# include <string>
# include <map>
# include "utilsTree.hpp"

class RedirectTree
{

	public:

		RedirectTree();
		RedirectTree( RedirectTree const & src );
		~RedirectTree();

		RedirectTree &		operator=( RedirectTree const & rhs );
		std::string			redirectTo(int error, std::string path);
		void				addRedirect(int error, std::string path, std::string destination);
		std::map< int, std::map<std::string, std::string> >	getTree(void) const;

	private:
		std::map< int, std::map<std::string, std::string> > _errorcodes;

};

std::ostream &			operator<<( std::ostream & o, RedirectTree const & i );

#endif /* **************************************************** REDIRECTTREE_H */