/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 17:34:54 by tmoragli          #+#    #+#             */
/*   Updated: 2022/09/27 18:27:24 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP
#include "Server.hpp"


void	setup_config(Server& Serv);
void	fill_serv(Server& Serv, std::vector<std::string> content);
void	fill_info(Server& Serv, std::string info);
void	fill_content(std::ifstream& file, std::vector<std::string>& content);
std::string	find_string_tab(std::string name, std::string *tab, int size);


#endif