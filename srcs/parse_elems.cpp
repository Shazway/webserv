/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elems.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 19:36:47 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/02 15:40:11 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"
#include "Server.hpp"

bool	set_port(v_str const& args, Server& serv)
{
	if (args.size() != 1)
		return (false);
	serv.setPort(args[0]);
	return (true);
}
bool	set_ip(v_str const& args, Server& serv)
{
	if (args.size() != 1)
		return (false);
	serv.setIp(args[0]);
	return (true);
}
bool	set_name(v_str const& args, Server& serv)
{
	if (args.size() != 1)
		return (false);
	serv.setName(args[0]);
	return (true);
}
bool	set_root(v_str const& args, Server& serv)
{
	if (args.size() != 1)
		return (false);
	serv.setName(args[0]);
	return (true);
}
bool	add_method(v_str const& args, Server& serv)
{
	if (args.size() != 2)
		return (false);

	t_allowedMethods	allowed;

	if (args[0].find("GET"))
		allowed.get = true;
	if (args[0].find("POST"))
		allowed.post = true;
	if (args[0].find("DELETE"))
		allowed.del = true;
	Serv.routes.addExecption(args[1], allowed.get, allowed.post, allowed.del);
	return (true);
}

bool	set_bodysize(v_str const& args, Server& serv)
{
	if (args.size() != 1)
		return (false);
	serv.setBody(args[0]);
	return (true);
}

bool	add_errorpath(v_str const& args, Server& serv)
{
	if (args.size() != 2)
		return (false);
	int error = args[0];
	serv.addErrorPath(error, args[1]);
	return (true);
}

bool	set_autoIndex(v_str const& args, Server& serv)
{
	if (args.size() != 1)
		return (false);
	serv.setAutoIndex(args[0]);
	return (true);
}
