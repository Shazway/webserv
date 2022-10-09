#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <iostream>
# include <string>
# include "Server.hpp"

# define CODE_NO_METHOD 1
# define CODE_NO_PATH 2
# define CODE_NO_QUERY 3
# define CODE_NO_HTTP_VERSION 4
# define CODE_UNSUPPORTED_HTTP_VERSION 5

class HttpRequest
{

	public:

		HttpRequest(Server &serv);
		//HttpRequest( HttpRequest const & src );
		~HttpRequest();

		HttpRequest &		operator=( HttpRequest const & rhs );
		bool		getPartiallyCompleted() const;
		void		setPartiallyCompleted(bool partiallyCmpleted);
		std::string	getMethod() const;
		void		setMethod(std::string method);
		std::string	getPath() const;
		void		setPath(std::string path);
		std::string	getQueryString() const;
		void		setQueryString(std::string query);
		std::string	getHttpVersion() const;
		void		setHttpVersion(std::string httpVersion);
		std::string	getHost() const;
		void		setHost(std::string host);
		std::string	getConnection() const;
		void		setConnection(std::string connection);
		std::string	getContentType() const;
		void		setContentType(std::string contentType);
		unsigned int	getContentLength() const;
		void		setContentLength(std::string ContentLength);
		std::string	getBody() const;
		void		setBody(std::string body);
		Server	&_serv;

	private:
		//request line
		bool		_partiallyCompleted;
		std::string	_method;
		std::string	_path;
		std::string	_queryString;
		std::string	_httpVersion;
		//header
		std::string	_host;
		std::string	_connection;
		std::string	_contentType;
		unsigned int	_contentLength;
		//body
		std::string	_body;

};

std::ostream &			operator<<( std::ostream & o, HttpRequest const & i );

class ForbiddenMethodException : virtual public std::exception
{
	virtual const char* what() const throw()
	{
		return ("This method is unknown or forbidden in this space.");
	}
};

class InvalidPathException : virtual public std::exception
{
	virtual const char* what() const throw()
	{
		return ("This path seems wrong.");
	}
};

class HttpVersionException : virtual public std::exception
{
	virtual const char* what() const throw()
	{
		return ("This HTTP version is not correct.");
	}
};

class DoubleHostException : virtual public std::exception
{
	virtual const char* what() const throw()
	{
		return ("Bad request");
	}
	//int code = 400;
};

class UnexpectedValueException : virtual public std::exception
{
	virtual const char* what() const throw()
	{
		return ("This value is unkown.");
	}
};

class LongBodyException : virtual public std::exception
{
	virtual const char* what() const throw()
	{
		return ("This body is too long for our configuration.");
	}
};

#endif /* ***************************************************** HTTPREQUEST_H */