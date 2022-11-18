/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_answers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdelwaul <mdelwaul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 16:22:03 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/18 17:55:04 by mdelwaul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include "Parsing.hpp"
#include "Webserv.hpp"
#include "CgiHandler.hpp"
#include <map>
#include <dirent.h>

std::string	itoa(long nb)
{
	std::stringstream	ss;

	ss << nb;
	return (ss.str());
}

std::string	write_body(std::string filename)
{
	std::ifstream	file;
	std::string		content;
	std::string		line;

	file.open(filename.c_str());
	if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof())
		return ("Error while loading page /!\\");
	if (file.is_open())
		while (std::getline(file, line))
			content += line + "\n";
	return (content);
}

void	generate_ok(int fd, std::map<int, std::string>& answers, std::ifstream& file, std::string type = "")
{
	std::string content;
	std::string	line;

	//ici, check allowedmethod et faire une erreur adaptee
	answers[fd] = "HTTP/1.1 200 OK\n";
	content.clear();
	if (file.is_open())
	{
		while (std::getline(file, line))
			content += line + "\n";
	}
	if (type == "favicon")
		answers[fd] += "Content-Type: html/favicon.ico\n";
	else if (type == "html")
		answers[fd] += "Content-Type: text/html\n";
	answers[fd] += "Content-Length: ";
	answers[fd] += itoa((long)content.size());
	answers[fd] += "\n\n";
	answers[fd] += content;
}

void	gen_error(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answers, int code)
{
	std::string content;
	HttpRequest	request = (*it).second;
	std::string errmsg;
	std::string errorfile_1 = "<html><head><title>";
	std::string errorfile_2 = "</title></head><body><h2>";
	std::string errorfile_3 = "</h2><br><a href=\"/index/index.html\">Go back to index</a></body></html>";
	int global_size = errorfile_1.size() + errorfile_2.size() + errorfile_3.size();
	int	fd = (*it).first;
	int i = 0;
	t_error_codes	codes[NB_CODES] = {{400, "Bad Request"}, {403, "Forbidden"},
								{404, "Not found"}, {405, "Method not allowed"},
								{413, "Request Entity Too Large"}, {500, "Internal Server Error"},
								{501, "Not Implemented"}, {505, "HTTP Version not supported"}};

	answers[fd] = "HTTP/1.1 " ;
	answers[fd] += itoa(code);
	answers[fd] += " ";
	while (i < NB_CODES)
	{
		if (codes[i].code == code)
		{
			global_size += (codes[i].message.size() + 4) * 2;
			errmsg = codes[i].message;
			answers[fd] += codes[i].message;
			break ;
		}
		i++;
	}
	if (i == NB_CODES + 1)
	{
		std::cout << BLINK_RED << "T'AS PAS GERE LE " << code << " CONNARD" << END << std::endl;
		return ;
	}
	answers[fd] += "\n";

	answers[fd] += "Content-Length: " + itoa(global_size);
	answers[fd] += "\n\n" + errorfile_1 + itoa(code) + " " + errmsg;
	answers[fd] += errorfile_2 + itoa(code) + " " + errmsg + errorfile_3;

	/*
	if (request._serv.getErrorPath(code).empty())
	{
		answers[fd] += "Content-Length: 0\n\n";
		return ;
	}
	content += write_body(request._serv.getRootPath() + request._serv.getErrorPath(code));
	answers[fd] += ("Content-Length: " + itoa(content.size())) + ("\n\n" + content);
	*/
}

// void	gen_body_too_long(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answers)
// {
// 	std::string content;

// 	answers[(*it).first] = "HTTP/1.1 413 Request Entity Too Large\n";
// 	content = write_body((*it).second._serv.getRootPath() + (*it).second._serv.getErrorPath(413));
// 	answers[(*it).first] += ("Content-Length: " + itoa(content.size())) + ("\n\n" + content);
// }

void	gen_ls(std::string abs_path, std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answers)
{
	DIR *rep = NULL;
	std::string str;
	struct dirent *file = NULL;
	
	rep = opendir(abs_path.c_str());
	if (!rep)
		gen_error(it, answers, 404);
	else
	{
		str = "<html><head>" + (*it).second.getPath() + "</head><body>";
		file = readdir(rep);
		file = readdir(rep);
		while (file)
		{
			str += "<br><a href=" + (*it).second.getPath() + "/" + file->d_name + "> " + file->d_name + "</a>";
			file = readdir(rep);
		}
		str += "</body>";
		generate_success((*it).first, answers, str);
	}
}

void	gen_get(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answers)
{
	HttpRequest	request = (*it).second;
	int 		fd = (*it).first;

	//GET peut avoir des variables dans la query
	if (request._serv.checkAllowedMethods("GET", request.getPath()))
	{
		std::ifstream file;
		std::string abs_path = request._serv.getRootPath() + request.getPath();

		if (!request.getQueryString().empty())
		{
			CgiHandler	handler(it, answers);
			return ;
		}
		//std::cout << YELLOW << "First open: [" << abs_path << "]" << END << std::endl;
		file.open(abs_path.c_str());
		if (file.is_open() && file.peek() == std::ifstream::traits_type::eof())
		{
			file.close();
			std::string	closest_directory = request._serv.html.getClosestDirectory(request.getPath()).second;
			if (closest_directory.empty())
			{
				if ((*it).second._serv.getAutoIndex())
					gen_ls(abs_path, it, answers);
				else
					gen_error(it, answers, 404);
				return ;
			}
			abs_path = request._serv.getRootPath() + closest_directory;
			//std::cout << YELLOW << "[" << abs_path << "]" << END << std::endl;
			file.open(abs_path.c_str());
			if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof())
				gen_error(it, answers, 404);
			else
				generate_ok(fd, answers, file, "html");
		}
		else if (file.is_open())
		{
			if (request.getPath().find("favicon") != std::string::npos)
				generate_ok(fd, answers, file, "favicon");
			else
				generate_ok(fd, answers, file, "html");
		}
		else
			gen_error(it, answers, 404);
		file.close();
	}
	else
		gen_error(it, answers, 405);
	//Header à rajouter plus tard \n \n
}

void	download_file(HttpRequest& request, std::map<int, std::string>& answers,
					std::map<int, Upload> uploads, int fd)
{
	Upload			up(request._serv);
	std::ifstream	file;
	std::string		body;

	if (uploads.find(fd) == uploads.end())
	{
		if (upload(up, request.getBody()) == COMPLETE)
			answers[fd] = "HTTP/1.1 200 OK\n";
		else
		{
			answers[fd] = "HTTP/1.1 206 Partial Content\n";
			uploads[fd] = up;
		}
	}
	else
	{

		if (upload(uploads[fd], request.getBody()) == COMPLETE)
		{
			answers[fd] = "HTTP/1.1 200 OK\n";
			uploads.erase(uploads.find(fd));
		}
		else
			answers[fd] = "HTTP/1.1 206 Partial Content\n";
	}
	body = write_body("./html/index/validation.html");
	answers[fd] += ("Content-Length: " + itoa((long)body.size())) + ("\n\n" + body);

}

void	gen_post(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answers, std::map<int, Upload> uploads)
{
	int	fd = (*it).first;
	HttpRequest request = (*it).second;

	if (request._serv.checkAllowedMethods("POST", request.getPath()))
	{
		if (request.getContentType().find("multipart/form-data") != std::string::npos)// Faut download
			download_file(request, answers, uploads, fd);
		if (request.getContentType().find("application/x-www-form-urlencoded") != std::string::npos)
			CgiHandler handler(it, answers);
			//run_cgi();*/ // Gérer les forms et executer le cgi
		//
		// METTRE UN VRAI BODY
		//
	}
	else
		gen_error(it, answers, 405);
		
}

void	gen_delete(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answers)
{
	std::ifstream empty;
	std::string		path =(*it).second._serv.getRootPath() + (*it).second.getPath();

	std::cout << GREEN << (*it).second.getPath() << END << std::endl;
	if ((*it).second._serv.checkAllowedMethods("DELETE", (*it).second.getPath()))
	{
		if (remove(path.c_str()))
			gen_error(it, answers, 404);
		else
			generate_ok((*it).first, answers, empty);
	}
	else
		gen_error(it, answers, 405);
}

void	answers_gen(std::map<int, HttpRequest>& requests, std::map<int, std::string>& answers, std::map<int, Upload>& uploads, std::map<int, Server> &client_serv)
{
	//gerer ici si le body est trop grand
	for (std::map<int, HttpRequest>::iterator it = requests.begin(); it != requests.end(); it++)
	{
		if ((*it).second.getContentLength() > client_serv[(*it).first].getBody())
			gen_error(it, answers, 413);
		else if ((*it).second.getMethod() == "GET")
			gen_get(it, answers);
		else if ((*it).second.getMethod() == "POST")
			gen_post(it, answers, uploads);
		else if ((*it).second.getMethod() == "DELETE")
			gen_delete(it, answers);
		else
			gen_error(it, answers, 405);
		std::cout << BLUE << "[" <<(*it).second.getMethod()<<"]" << " Body: " << ((*it).second.getBody()) << END << std::endl;
		
	}
	requests.clear();
}
