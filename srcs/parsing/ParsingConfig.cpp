/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdelwaul <mdelwaul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:45:03 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/19 23:59:20 by mdelwaul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>

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
	int			error = -1;
	std::string	redir;
	bool		found[3] = {0, 0, 0};

	if (it != content.end() && (*it).find("root:") != std::string::npos)
		it++;
	while (it != content.end() && (*it).find("\t\t") != std::string::npos)
	{
		if ((*it).find("r_path") != std::string::npos)
		{
			ft_split((*it), args, " ");
			args.erase(args.begin());
			if (found[PATH] == true)
				return (display_error(DUPLICATE_ERROR));
			found[PATH] = true;
			if (!set_root(args, serv))
				return (false);
			args.clear();
		}
		if ((*it).find("r_method") != std::string::npos)
		{
			ft_split((*it), args, " ");
			args.erase(args.begin());
			if (found[METHOD] == true)
				return (display_error(DUPLICATE_ERROR));
			found[METHOD] = true;
			if (!add_methods(args, method.allowed))
				return (false);
			args.clear();
		}
		if ((*it).find("r_index") != std::string::npos)
		{
			ft_split((*it), args, " ");
			args.erase(args.begin());
			if (found[INDEX] == true)
				return (display_error(DUPLICATE_ERROR));
			found[INDEX] = true;
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
			if (error != -1 && !redir.empty())
				serv.redirect.addRedirect(error, "/", redir);
		}
		it++;
	}
	if (!found[PATH])
		return (display_error(NO_R_PATH));
	if (!found[METHOD])
	{
		std::cout << YELLOW << "/!\\ Strongly recoomended to add r_method in root: scope" << END << std::endl;
		return (true);
	}
	if (!index.empty())
		serv.html.addExecption("/", index);
	serv.routes.addExecption("/", METHODS.get, METHODS.post, METHODS.del);
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
	bool		found[3] = {0, 0, 0};

	if (it != content.end() && (*it).find("location:") != std::string::npos)
		it++;
	while (it != content.end() && (*it).find("\t\t") != std::string::npos)
	{
		if ((*it).find("l_path") != std::string::npos)
		{
			ft_split((*it), args, " ");
			args.erase(args.begin());
			if (found[PATH] == true)
				return (display_error(DUPLICATE_ERROR));
			found[PATH] = true;
			if (!method_set_path(args, method.path))
				return (false);
			args.clear();
		}
		if ((*it).find("l_method") != std::string::npos)
		{
			ft_split((*it), args, " ");
			args.erase(args.begin());
			if (found[METHOD] == true)
				return (display_error(DUPLICATE_ERROR));
			found[METHOD] = true;
			if (!add_methods(args, method.allowed))
				return (false);
			l_method = true;
			args.clear();
		}
		if ((*it).find("l_index") != std::string::npos)
		{
			ft_split((*it), args, " ");
			args.erase(args.begin());
			if (found[INDEX] == true)
				return (display_error(DUPLICATE_ERROR));
			found[INDEX] = true;
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
			if (error != -1 && !redir.empty())
				serv.redirect.addRedirect(error, method.path, redir);
		}
		it++;
	}
	if (method.path.empty())
		return (display_error(NO_L_PATH));
	if (!index.empty())
		serv.html.addExecption(method.path, index);
	if (l_method)
		serv.routes.addExecption(method.path, METHODS.get, METHODS.post, METHODS.del);
	return (true);
}

bool	encountered_problem(std::string info, v_str args, v_str_it &it)
{
	std::cerr << WHITE << "Encountered problem at line: " << RED << *it << END << std::endl
	<< WHITE << "Wrong arguments for: " << RED << info << END;
	display_v_str(args);
	return (false);
}


bool	parse_server(Server& serv, v_str& content, v_str_it& it)
{
	v_str	args;
	t_parser	parse[7] = {{"error_path", &add_errorpath, 0}, {"ip", &set_ip, 0},
							{"name", &set_name, 0}, {"body_size", &set_bodysize, 0},
							{"autoindex", &set_autoIndex, 0}, {"port", &set_port, 0},
							{"upload_path", &set_upload, 0}};

	if (it != content.end() && (*it).find("server:") != std::string::npos)
		it++;
	while (it != content.end() && (*it).find("\t") != std::string::npos)
	{
		if ((*it).find("location:") != std::string::npos)
		{
			if (!parse_location(serv, content, it))
			{
				encountered_problem("location ", args, it);
				return (display_error(LOCATION_PROB));
			}
			continue ;
		}
		if ((*it).find("root:") != std::string::npos)
		{
			if (!parse_root(serv, content, it))
			{
				encountered_problem("root ", args, it);
				return (display_error(ROOT_PROB));
			}
			continue ;
		}
		for (int i = 0; i < 7; i++)
		{
			if ((*it).find(parse[i].serv_info) != std::string::npos)
			{
				ft_split((*it).data(), args, " ");
				args.erase(args.begin());
				if (parse[i].found == 1 && parse[i].serv_info != "error_path")
				{
					display_error(DUPLICATE_ERROR);
					encountered_problem(parse[i].serv_info, args, it);
					return (false);
				}
				parse[i].found = 1;
				if (!parse[i].s(args, serv))
					return (encountered_problem(parse[i].serv_info, args, it));
				args.clear();
			}
		}
		it++;
	}
	if (!parse[5].found)
		return (display_error(NO_PORT));
	if (!parse[3].found)
		return (display_error(NO_BODY_SIZE));
	return (true);
}

bool	valid_upload_path(Server& serv)
{
	std::string		foldername;
	foldername = serv.getRootPath() + serv.getUploadPath();
	DIR * rep = opendir(foldername.c_str());

    if (!rep)
        return display_error(UPLOAD_ERROR);
    closedir(rep);
    return true;

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
			if (!valid_upload_path(tmp_serv))
				return (false);
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
