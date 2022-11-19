/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elems.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 19:36:47 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/19 19:53:03 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"
#include "Server.hpp"
#include "ErrorMessages.hpp"
bool	display_error(std::string error)
{
	std::cout << RED << error << END << std::endl;
	return (false);
}

bool	is_str_alpha(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
		if (!isalpha(str[i]))
			return (false);
	return (true);
}
bool	is_str_path(std::string str)
{
	if (str.find("https") == 0)
		return (true);
	else if(!str.empty() && str[0] != '/')
			return (display_error(PATH_SYNTAX));
	for (size_t i = 0; i < str.size(); i++)
		if (!isprint(str[i]) && str[i] != '/' && str[i] != '.')
			return (false);
	return (true);
}
bool	is_str_num(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
		if ((str[i]) < '0' || str[i] > '9')
			return (false);
	return (true);
}

bool	set_upload(v_str & args, Server& serv)
{
	if (args.size() != 1)
		return (display_error(WRONG_ARGS_NB));
	if (!is_str_path(args[0]))
		return (display_error(PATH_ERROR));
	serv.setUploadPath(args[0]);
	return (true);
}

bool	set_port(v_str & args, Server& serv)
{
	if (args.size() != 1)
		return (display_error(WRONG_ARGS_NB));
	std::string	arg = args.front();

	for (std::string::iterator it = arg.begin(); it != arg.end(); it++)
		if (!isdigit((*it)))
			return (display_error(ONLY_NUM));
	serv.setPort(arg);
	return (true);
}

bool	parse_ip(std::string ip)
{
	std::vector<std::string>	split_ip;

	ft_split(ip, split_ip, ".");
	if (split_ip.size() != 4)
		return (false);
	for (v_str_it it = split_ip.begin(); it != split_ip.end(); it++)
	{
		std::string tmp = (*it);
		if (tmp.size() < 1 || tmp.size() > 3)
			return (false);
	}
	return (true);
}

bool	set_ip(v_str & args, Server& serv)
{
	if (args.size() != 1)
		return (display_error(WRONG_ARGS_NB));
	std::string	arg = args.front();

	for (std::string::iterator it = arg.begin(); it != arg.end(); it++)
		if (!isdigit((*it)) && (*it) != '.')
			return (display_error(PATH_ERROR));
	if (!parse_ip(arg))
		return (false);
	serv.setIp(arg);
	return (true);
}
bool	set_name(v_str & args, Server& serv)
{
	if (args.size() != 1)
		return (display_error(WRONG_ARGS_NB));
	serv.setName(args[0]);
	return (true);
}
bool	set_root(v_str & args, Server& serv)
{
	if (args.size() != 1)
		return (display_error(WRONG_ARGS_NB));
	for (size_t i = 0; i < args[0].size(); i++)
		if (!isprint(args[0][i]) && args[0][i] != '/' && args[0][i] != '.')
			return (display_error(PATH_ERROR));
	serv.setRootPath(args[0]);
	return (true);
}


bool	set_bodysize(v_str & args, Server& serv)
{
	if (args.size() != 1)
		return (display_error(WRONG_ARGS_NB));

	if (!is_str_num(args[0]))
		return (display_error(ONLY_NUM));
	serv.setBody(args[0]);
	return (true);
}

bool	add_errorpath(v_str & args, Server& serv)
{
	if (args.size() != 2)
		return (display_error(WRONG_ARGS_NB));
	std::string	string = args.front();
	const char* tmp = string.c_str();
	if (!is_str_num(tmp))
		return (display_error(ONLY_NUM));
	if (!is_str_path(args[1]))
		return (display_error(PATH_ERROR));
	int error = atoi(tmp);
	serv.addErrorPath(error, args[1]);
	return (true);
}
bool	add_redir(v_str & args, std::string& path, int& error)
{
	if (args.size() != 2)
		return (display_error(WRONG_ARGS_NB));
	const char* tmp = args.front().c_str();
	if (!is_str_num(tmp))
		return (display_error(ONLY_NUM));
	error = atoi(tmp);
	path = args[1];
	if (!is_str_path(path))
		return (display_error(PATH_ERROR));
	return (true);
}

bool	set_autoIndex(v_str & args, Server& serv)
{
	if (args.size() != 1)
		return (display_error(WRONG_ARGS_NB));
	if (args[0] != "false" && args[0] != "true")
		return (display_error(STATE_ONLY));
	serv.setAutoIndex(args[0]);
	return (true);
}

bool	method_set_path(v_str& args, std::string& path)
{
	if (args.size() != 1)
		return (display_error(WRONG_ARGS_NB));
	path = args.back();
	if (!is_str_path(path))
		return (display_error(PATH_ERROR));
	return (true);
}

bool	add_methods(v_str& args, t_allowedMethods& allowed)
{
	if (args.empty())
		return (display_error(WRONG_ARGS_NB));
	allowed.del = false;
	allowed.get = false;
	allowed.post = false;
	for (v_str_it it = args.begin(); it != args.end(); it++)
	{
		if ((*it).find("GET") != std::string::npos)
			allowed.get = true;
		if ((*it).find("POST") != std::string::npos)
			allowed.post = true;
		if ((*it).find("DELETE") != std::string::npos)
			allowed.del = true;
	}
	return (true);
}

bool	add_index(v_str& args, std::string &index)
{
	if (args.size() != 1)
		return (display_error(WRONG_ARGS_NB));
	index = args.front();
	if (!is_str_path(index))
		return (display_error(PATH_ERROR));
	return (true);
}