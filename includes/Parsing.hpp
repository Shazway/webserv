/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 17:34:54 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/04 22:12:03 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP
#include "Server.hpp"

class Server;
//Vector string//
typedef std::vector<std::string> v_str;
//Vector string iterator//
typedef std::vector<std::string>::iterator v_str_it;

typedef bool	(*parse_elem)(std::vector<std::string>& , Server&);
typedef struct s_parser
{
	std::string		serv_info;
	parse_elem		s;
} t_parser;

std::vector<Server>	setup_config(char* config_path);
void	fill_serv(Server& Serv, std::vector<std::string> content);
void	fill_info(Server& Serv, std::string info);
void	fill_content(std::ifstream& file, std::vector<std::string>& content);
std::string	find_string_tab(std::string name, std::string *tab, int size);

//PARSE ELEMS
void	ft_split(std::string str, v_str& v, std::string delim);
bool	set_port(v_str& args, Server& serv);
bool	set_ip(v_str& args, Server& serv);
bool	set_name(v_str& args, Server& serv);
bool	set_root(v_str& args, Server& serv);
bool	add_method(v_str& args, Server& serv);
bool	set_bodysize(v_str& args, Server& serv);
bool	add_errorpath(v_str& args, Server& serv);
bool	set_autoIndex(v_str& args, Server& serv);

#endif