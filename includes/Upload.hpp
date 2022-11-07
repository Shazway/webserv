/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 17:18:49 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/07 23:55:37 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UPLOAD_HPP
#define UPLOAD_HPP
#include "utilsTree.hpp"
#define COMPLETE 1
#define INCOMPLETE 0

class Upload
{
private:
	std::ofstream	_file;
	std::string		_path;
	std::string		_delim;
	bool			_complete;
public:
	Upload();
	~Upload();
	std::ofstream	getFile() const;
	std::string		getPath() const;
	std::string		getDelim() const;
	bool			getComplete() const;

	void			openFile(std::string name);
	void			setDelim(std::string delim);
	void			setPath(std::string path);
	bool			checkCompletion();
	void			addContent(std::string content);
	void			closeFile();
	Upload& operator=(Upload const& assign);
};

class FailedToOpenFileException: virtual public std::exception
{
	virtual const char* what() const throw()
	{
		return ("/!\\ Failed to open file for upload /!\\");
	}
};

#endif