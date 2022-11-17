/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 21:52:23 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/13 22:40:07 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"
#include "Webserv.hpp"
#include <sys/wait.h>

void	CgiHandler::str_arr_free()
{
	int	i;

	i = 0;
	while (_args && _args[i])
	{
		free(_args[i]);
		i++;
	}
	delete[] _args;
}

int	atoi_hexa(std::string hex)
{
	int	res;

	res = 0;
	for (size_t i = 0; i < hex.size(); i++)
	{
		res *= 16;
		if (isalpha(hex[i]))
			res += hex[i] - 'A' + 10;
		else
			res += hex[i] - '0';
	}
	return res;
}

int	count_args(std::string str)
{
	int i = 1;
	long unsigned int pos = 0;
	const char *str_char = str.c_str();
	while (pos < str.size())
	{
		if (str_char[pos] == '&')
			i++;
		pos++;
	}
	return i;
}

void	CgiHandler::generate_success(int fd, std::map<int, std::string>& answers, std::string str)
{
	std::string content;
	std::string	line;

	//ici, check allowedmethod et faire une erreur adaptee
	answers[fd] = "HTTP/1.1 200 OK\n";
	answers[fd] += "Content-Type: text/html\n";
	answers[fd] += "Content-Length: ";
	answers[fd] += itoa((long)str.size());
	answers[fd] += "\n\n";
	answers[fd] += str;
}

void CgiHandler::separate_args(std::string str, HttpRequest request)
{
	int	size = count_args(str) + 1;
	long unsigned int old_pos = 0;
	long unsigned int new_pos = 0;
	int loop = 1;

	_args = new char*[size + 1];
	_args[size] = NULL;
	_args[0] = strdup((request._serv.getRootPath() + request.getPath()).c_str());
	for (long unsigned int i = 0; i < str.size(); i++)
	{
		if (str[new_pos] == '&')
		{
			if (new_pos == old_pos)
			{
				//_args[loop] = new char[1];
				_args[loop] = (char*)malloc(sizeof(char) * 1);
				memset(_args[loop], 0, sizeof(char) * 1);
			}
			else
				_args[loop] = strdup(str_convert(str.substr(old_pos, new_pos - old_pos)).c_str());
			old_pos = new_pos + 1;
			loop++;
		}
		new_pos++;
	}
	if (new_pos == old_pos)
	{
		_args[loop] = new char[1];
		memset(_args[loop], 0, sizeof(char) * 1);
	}
	else
		_args[loop] = strdup(str_convert(str.substr(old_pos, new_pos - old_pos)).c_str());
}

CgiHandler::CgiHandler(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answer): _args(NULL)
{
	if ((*it).second.getMethod() == "GET")
		get_handler(it, answer);
	else if ((*it).second.getMethod() == "POST")
		post_handler(it, answer);
	return;
}

CgiHandler::~CgiHandler()
{
	str_arr_free();
	return ;
}

void	CgiHandler::get_handler(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answer)
{
	separate_args((*it).second.getQueryString(), (*it).second);
	exec_cgi(it, answer);
}

void	CgiHandler::post_handler(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answer)
{
	separate_args((*it).second.getBody(), (*it).second);
	exec_cgi(it, answer);
}

void	CgiHandler::exec_child()
{
	close(_fd[0]);
	dup2(_fd[1], 1);
	close(_fd[1]);
	execve(_args[0], _args, __environ);
	std::cout << BLINK_RED << "Error with execve, change this message" << END << std::endl;
	close(_stdin);
	exit(1);
}

void	CgiHandler::exec_cgi(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answer)
{
	int	status;
	std::string str;
	std::string str_inter;
	for(int i = 0; _args[i]; i++)
	{
		std::cout << BLUE << "-> " << _args[i] << END << std::endl;
	}
	_stdin = dup(0);
	if (pipe(_fd) < 0 || (_pid = fork()) < 0)
		return ;
	if (!_pid)
		exec_child();
	close(_fd[1]);
	dup2(_fd[0], 0);
	close(_fd[0]);
	waitpid(_pid, &status, 0);
	/* --------RECUP LE RESULTAT DE EXECVE ICI----------- */
	while (std::getline(std::cin, str))
		str_inter += str + '\n';
	std::cout << str_inter << std::endl;
	/* -------------------------------------------------- */
	if (WEXITSTATUS(status))
		gen_error(it, answer, 400); /* CODE A REVOIR */
	else
		generate_success((*it).first, answer, str_inter);
	dup2(_stdin, 0);
	close(_stdin);
}

std::string CgiHandler::str_convert(std::string arg)
{
	std::string	new_arg;
	for (size_t i = 0; i < arg.size(); i++)
	{
		if (arg[i] != '+' && arg[i] != '%')
			new_arg += arg[i];
		else if (arg[i] == '+')
			new_arg += ' ';
		else if (arg[i] == '%')
		{
			int hexval = atoi_hexa(arg.substr(i + 1, 2));
			unsigned char c = (unsigned char)hexval;
			new_arg += c;
			i += 2;
		}
	}
	return new_arg;
}