/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elems.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 19:36:47 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/09 16:35:30 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"
#include "Server.hpp"

bool	set_port(v_str & args, Server& serv)
{
	if (args.size() != 1)
		return (false);
	std::string	arg = args.front();

	for (std::string::iterator it = arg.begin(); it != arg.end(); it++)
		if (!isdigit((*it)))
			return (false);
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
		return (false);
	std::string	arg = args.front();

	for (std::string::iterator it = arg.begin(); it != arg.end(); it++)
		if (!isdigit((*it)) && (*it) != '.')
			return (false);
	if (!parse_ip(arg))
		return (false);
	serv.setIp(arg);
	return (true);
}
bool	set_name(v_str & args, Server& serv)
{
	if (args.size() != 1)
		return (false);
	serv.setName(args[0]);
	return (true);
}
bool	set_root(v_str & args, Server& serv)
{
	if (args.size() != 1)
		return (false);
	serv.setName(args[0]);
	return (true);
}


bool	set_bodysize(v_str & args, Server& serv)
{
	if (args.size() != 1)
		return (false);
	serv.setBody(args[0]);
	return (true);
}

bool	add_errorpath(v_str & args, Server& serv)
{
	if (args.size() != 2)
		return (false);
	std::string	string = args.front();
	const char* tmp = string.c_str();
	int error = atoi(tmp);
	serv.addErrorPath(error, args[1]);
	return (true);
}

bool	set_autoIndex(v_str & args, Server& serv)
{
	if (args.size() != 1)
		return (false);
	serv.setAutoIndex(args[0]);
	return (true);
}

bool	method_set_path(v_str& args, std::string& path)
{
	if (args.size() != 1)
		return (false);
	path = args.back();
	return (true);
}

bool	add_methods(v_str& args, t_allowedMethods& allowed)
{
	if (args.empty())
		return (false);
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
		return (false);
	index = args.front();
	return (true);
}