#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <iostream>
# include <string>

class HttpRequest
{

	public:

		HttpRequest();
		HttpRequest( HttpRequest const & src );
		~HttpRequest();

		HttpRequest &		operator=( HttpRequest const & rhs );
		std::string	getMethod();
		void		setMethod(std::string method);
		std::string	getPath();
		void		setPath(std::string path);
		std::string	getQueryString();
		void		setQueryString(std::string query);
		std::string	getHttpVersion();
		void		setHttpVersion(std::string httpVersion);
		std::string	getHost();
		void		setHost(std::string host);
		std::string	getConnection();
		void		setConnection(std::string connection);
		std::string	getContentType();
		void		setContentType(std::string contentType);
		std::string	getContentLengh();
		void		setContentLengh(std::string contentLengh);
		std::string	getBody();
		void		setBody(std::string body);

	private:
		//request line
		std::string	_method;
		std::string _path;
		std::string	_queryString;
		std::string _httpVersion;
		//header
		std::string	_host;
		std::string	_connection;
		std::string _contentType;
		std::string _contentLengh;
		//body
		std::string	_body;



};

std::ostream &			operator<<( std::ostream & o, HttpRequest const & i );

#endif /* ***************************************************** HTTPREQUEST_H */