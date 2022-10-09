/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 17:34:54 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/09 15:57:53 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP
#include "Server.hpp"

#define METHOD method.allowed
class Server;
//Vector string//
typedef std::vector<std::string> v_str;
//Vector string iterator//
typedef std::vector<std::string>::iterator v_str_it;

//Parsing function
typedef bool	(*parse_elem)(std::vector<std::string>& , Server&);
//Duplicate checkers function
typedef bool		(*check_dup)(std::vector<Server>&);

typedef struct s_dup
{
	std::string	error_msg;
	check_dup	ft;
}	t_dup;

typedef struct s_parser
{
	std::string		serv_info;
	parse_elem		s;
} t_parser;

typedef struct s_method
{
	std::string			path;
	t_allowedMethods	allowed;
} t_method;

std::vector<Server>	parse_config(char* config_path);
void	fill_serv(Server& Serv, std::vector<std::string> content);
void	fill_info(Server& Serv, std::string info);
void	fill_content(std::ifstream& file, std::vector<std::string>& content);

//PARSE ELEMS
void	ft_split(std::string str, v_str& v, std::string delim);
bool	set_port(v_str& args, Server& serv);
bool	set_ip(v_str& args, Server& serv);
bool	set_name(v_str& args, Server& serv);
bool	set_root(v_str& args, Server& serv);
bool	set_bodysize(v_str& args, Server& serv);
bool	add_errorpath(v_str& args, Server& serv);
bool	set_autoIndex(v_str& args, Server& serv);
bool	add_methods(v_str& args, t_allowedMethods& allowed);
void	display_v_str(std::vector<std::string> str);
bool	add_index(v_str& args, std::string &index);

//PARSE ROOT ELEMS
bool	method_set_path(v_str& args, std::string& path);

//PRINT FUNCTIONS
void	print_separator();
void	display_v_str(std::vector<std::string> str);
void	display_servers(std::vector<Server> servers);

//DUPLICATE CHECKS FOR SERVERS
bool	check_duplicates(std::vector<Server> servers);
bool	check_port(std::vector<Server>& servers);
bool	check_name(std::vector<Server>& servers);


#endif