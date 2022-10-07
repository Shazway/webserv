#ifndef HTTPANSWER_HPP
# define HTTPANSWER_HPP

# include <iostream>
# include <string>

class HttpAnswer
{

	public:

		HttpAnswer();
		HttpAnswer( HttpAnswer const & src );
		~HttpAnswer();

		HttpAnswer &		operator=( HttpAnswer const & rhs );

	private:

};

std::ostream &			operator<<( std::ostream & o, HttpAnswer const & i );

#endif /* ****************************************************** HTTPANSWER_H */