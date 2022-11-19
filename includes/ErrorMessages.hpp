/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorMessages.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/18 17:27:09 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/19 23:21:19 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORMESSAGES_HPP
# define ERRORMESSAGES_HPP

bool	display_error(std::string error);
//GENERIC ERRORS
# define SERVER_PROB "Encountered problem while parsing server info, check above"
# define ROOT_PROB "Encountered problem while parsing root info, check above"
# define LOCATION_PROB "Encountered problem while parsing location info, check above"
//SERVER ERRORS
# define NO_PORT "Missing port for server"
# define NO_NAME "Server name missing"
# define NO_BODY_SIZE "Body size missing for server"
# define NO_AUTOINDEX "Autoindex missing for server"
# define NO_UPLOAD_PATH "Upload path missing for server"
# define NO_ROOT "Root missing for server"
# define NO_LOCATION "Location missing for server"
//ROOT ROUTE ERRORS
# define NO_R_PATH "Missing root path"
# define NO_R_METHOD "Missing root method"
# define NO_R_INDEX "Missing root index"
//LOCATION ROUTE ERRORS
# define NO_L_PATH "Missing location path"
# define NO_L_METHOD "Missing location method"
# define NO_L_INDEX "Missing location index"
# define NO_L_REDIR "Missing location redirection"
//SYNTAX ERRORS
# define ONLY_NUM "Argument must contain only numeric values"
# define PATH_ERROR "Wrong syntax for path </path/to/file> or </path/to/folder>"
# define ONLY_ALPHA "Argument must contain only alphabetic values"
# define STATE_ONLY "Argument must either be a false, or true state"
# define EXTENSION_ERR "File must end with <.conf>, redirecting to default configuration file"
# define INVALID_FILE "Configuration file is invalid, either empty or a folder"
# define MISSING_ARGUMENTS "Missing arguments to configuration file"
# define WRONG_ARGS_NB "Wrong number of arguments"
# define PATH_SYNTAX "Path must start with a </> character or if it's a link <https://website.com>"
//GUIDE MESSAGES
# define TRY_THIS "Try this syntax:\nserver:\n\tport <number>\n\tip <ip>\n\tserver_name <server_name>\n\tbody_size <nb>\n\tautoindex <state>\n\terror_path <nb> <path>\n\tupload_path <path>\n\troot:\n\t\tr_path <filename>\n\t\tr_method <methods>\n\t\tr_index <path>\n\tlocation:\n\t\tl_path <path>\n\t\tl_method <methods>\n\t\tl_index <path>\n\t\tl_redir <error code> <link>\n\t\tl_path <path>\n\t\tl_method <methods>"
# define ARG_ERR "Arguments must be spaced with one space, <port> <number>"
# define DUPLICATE_ERROR "Duplicate information provided"
# define UPLOAD_ERROR "Wrong upload path, please try an existing folder"
#endif