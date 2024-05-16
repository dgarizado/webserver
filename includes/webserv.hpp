/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:31:14 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/16 18:58:19 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//HERE WE WILL INCLUDE ALL THE HEADER FILES NEEDED FOR THE WEB SERVER
#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <fstream>
# include <sstream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
#include <sys/event.h> // Include this for kqueue
#include <sys/time.h>  // Include this for kqueue
#include <set>

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define RESET "\033[0m"

# include "VHost.hpp"
# include "ConfParse.hpp"
int ft_error(std::string msg);
#endif