/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:31:14 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/19 13:13:52 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//HERE WE WILL INCLUDE ALL THE HEADER FILES NEEDED FOR THE WEB SERVER
#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
// #include <sys/event.h> // Include this for kqueue
#include <sys/time.h> 
#include <set>
#include <cstring>
#include <algorithm>
#include <sys/epoll.h>
#include <fcntl.h>
#include <iostream>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define RESET "\033[0m"
#define PINK "\033[95m"

enum location {
	ROOT,
	INDEX,
	AUTOINDEX,
	TOTAL_LOCATION
};

enum server {
	LISTEN,
	SERVER_NAME,
	LOCATION,
	TOTAL_SERVER
};

enum methods {
	GET,
	POST,
	PUT,
	DELETE,
	TOTAL_METHODS
};

typedef struct s_location {
	std::string					location;
	std::string					root;
	std::vector<std::string>	index;
	bool						autoIndex;
	bool						allowedMethods[TOTAL_METHODS];
}t_location;

typedef struct s_server {
    std::vector<std::string>	server_name;
    std::string					serverName;
    int 						listen;
    int 						port;
    std::string					errorLogs;
    std::string					workerConnections;
    std::vector<t_location>		locations;

} t_server;

typedef struct s_fileParse {
	int							test;
	std::set<int>				ports;
	std::string					errorLog;
	std::string					workerConnections;
	std::vector<t_server>		serverData;
}t_fileParse;


// #include "VHost.hpp"
// #include "FileParse.hpp"
// #include "RequestParser.hpp"
// // #include "Connection.hpp"
// #include "Master.hpp"

//UTILS
int ft_error(std::string msg);
std::string extractLocationUriStr(std::string uri);
std::string extractFileNameStr(std::string uri);
std::string extractQueryStr(std::string uri);

#endif