/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 17:18:49 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/06 20:18:50 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UPLOAD_HPP
#define UPLOAD_HPP
#include "utilsTree.hpp"

class Upload
{
private:
	std::ifstream	_file;
	std::string		_path;
	bool			_complete;
public:
	Upload();
	~Upload();
	std::ifstream	getFile() const;
	std::string		getPath() const;
	bool			getComplete() const;

	void			createFile(std::string name);
	void			setPath(std::string path);
	bool			checkCompletion();
	void			addContent(std::string content);
	void			closeFile();
};

class FailedToOpenFileException: virtual public std::exception
{
	virtual const char* what() const throw()
	{
		return ("/!\\ Failed to open file for upload /!\\");
	}
};

#endif