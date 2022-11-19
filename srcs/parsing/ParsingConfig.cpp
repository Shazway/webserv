/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:45:03 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/19 19:41:18 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <vector>

#include "Parsing.hpp" 
#include "ErrorMessages.hpp"
#include "Server.hpp"

void	fill_content(std::ifstream& file, std::vector<std::string>& content)
{
	std::string	line;

	while (std::getline(file, line)){
		content.push_back(line);
	}
}

bool	parse_root(Server& serv, v_str& content, v_str_it& it)
{
	t_method	method;
	v_str		args;
	std::string	index;
	int			error;
	std::string	redir;

	if (it != content.end() && (*it).find("root:") != std::string::npos)
		it++;
	while (it != content.end() && (*it).find("\t\t") != std::string::npos)
	{
		if ((*it).find("r_path") != std::string::npos)
		{
			ft_split((*it), args, " ");
			args.erase(args.begin());
			set_root(args, serv);
			args.clear();
		}
		if ((*it).find("r_method") != std::string::npos)
		{
			ft_split((*it), args, " ");
			args.erase(args.begin());
			if (!add_methods(args, method.allowed))
				return (false);
			args.clear();
		}
		if ((*it).find("r_index") != std::string::npos)
		{
			ft_split((*it), args, " ");
			args.erase(args.begin());
			if (!add_index(args, index))
				return (false);
			args.clear();
		}
		if ((*it).find("r_redir") != std::string::npos)
		{
			ft_split((*it), args, " ");
			args.erase(args.begin());
			if (!add_redir(args, redir, error))
				return (false);
			args.clear();
		}
		it++;
	}
	if (!index.empty())
		serv.html.addExecption("/", index);
	serv.routes.addExecption("/", METHOD.get, METHOD.post, METHOD.del);
	if (error != -1 && !redir.empty() && !method.path.empty())







































































































































































	
		serv.redirect.addRedirect(error, method.path, redir);
	return (true);
}

bool	parse_location(Server& serv, v_str& content, v_str_it& it)
{
	t_method	method;
	std::string	index;
	v_str		args;
	std::string	redir;
	int			error = -1;
	bool		l_method = false;

	if (it != content.end() && (*it).find("location:") != std::string::npos)
		it++;
	while (it != content.end() && (*it).find("\t\t") != std::string::npos)
	{
		if ((*it).find("l_path") != std::string::npos)
		{
			ft_split((*it), args, " ");
			args.erase(args.begin());
			if (!method_set_path(args, method.path))
				return (false);
			args.clear();
		}
		if ((*it).find("l_method") != std::string::npos)
		{
			ft_split((*it), args, " ");
			args.erase(args.begin());
			if (!add_methods(args, method.allowed))
				return (false);
			l_method = true;
			args.clear();
		}
		if ((*it).find("l_index") != std::string::npos)
		{
			ft_split((*it), args, " ");
			args.erase(args.begin());
			if (!add_index(args, index))
				return (false);
			args.clear();
		}
		if ((*it).find("l_redir") != std::string::npos)
		{
			ft_split((*it), args, " ");
			args.erase(args.begin());
			if (!add_redir(args, redir, error))
				return (false);
			args.clear();
		}
		it++;
	}
	if (method.path.empty())
		return (display_error(NO_L_PATH));
	if (!index.empty())
		serv.html.addExecption(method.path, index);
	if (l_method)
		serv.routes.addExecption(method.path, METHOD.get, METHOD.post, METHOD.del);
	if (error != -1 && !redir.empty() && !method.path.empty())
		serv.redirect.addRedirect(error, method.path, redir);
	return (true);
}


bool	parse_server(Server& serv, v_str& content, v_str_it& it)
{
	v_str	args;
	t_parser	parse[7] = {{"error_path", &add_errorpath}, {"ip", &set_ip},
							{"name", &set_name}, {"body_size", &set_bodysize},
							{"autoindex", &set_autoIndex}, {"port", &set_port},
							{"upload_path", &set_upload}};

	if (it != content.end() && (*it).find("server:") != std::string::npos)
		it++;
	while (it != content.end() && (*it).find("\t") != std::string::npos)
	{
		if ((*it).find("location:") != std::string::npos)
		{
			if (!parse_location(serv, content, it))
				return (display_error(LOCATION_PROB));
			continue ;
		}
		if ((*it).find("root:") != std::string::npos)
		{
			if (!parse_root(serv, content, it))
				return (display_error(ROOT_PROB));
			continue ;
		}
		for (int i = 0; i < 7; i++)
		{
			if ((*it).find(parse[i].serv_info) != std::string::npos)
			{
				ft_split((*it).data(), args, " ");
				args.erase(args.begin());
				if (!parse[i].s(args, serv))
				{
					std::cerr << RED << "Encountered problem at line: " << *it << std::endl
					<< parse[i].serv_info << " wrong arguments: " << END;
					display_v_str(args);
					return (false);
				}
				args.clear();
			}
		}
		it++;
	}
	return (true);
}

bool	fill_servers(std::vector<Server>& servers, v_str content)
{
	v_str	args;
	v_str_it it = content.begin();

	while (it != content.end())
	{
		if (it != content.end() && (*it).find("server:") != std::string::npos)
		{
			Server	tmp_serv;
			if (!parse_server(tmp_serv, content, it))
				return (display_error(SERVER_PROB));
			servers.push_back(tmp_serv);
		}
		else
			it++;
	}
	return (true);
}


bool	parse_config(char const* config_path, std::vector<Server>& servers)
{
	std::ifstream				file;
	v_str						content;

	file.open(config_path, std::ios_base::in);
	if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof())
	{
		std::cerr << RED
		<< "Failed to open config file from path: "
		<< config_path << END << std::endl;
		return (false);
	}
	fill_content(file, content);
	file.close();
	if (!fill_servers(servers, content) || !check_duplicates(servers))
	{
		std::cerr << RED << "Error while parsing file, see error message above" << END << std::endl;
		return (false);
	}
	display_servers(servers);
	return (true);
}
