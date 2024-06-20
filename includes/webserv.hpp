/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:31:14 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/20 20:37:21 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//HERE WE WILL INCLUDE ALL THE HEADER FILES NEEDED FOR THE WEB SERVER
#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include <string>
#include <array>
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
#include <cstdio>
#include <iomanip>
#include <sys/wait.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define RESET "\033[0m"
#define PINK "\033[95m"

#define MAX_EVENTS 64 //IMPORATNTEEEEE PARA EL SUBJECTTTTTTT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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
    int 						listen;
    int 						port;
    std::vector<t_location>		locations;

} t_server;

typedef struct s_fileParse {
	std::set<int>				ports;
	std::vector<t_server>		serverData;
}t_fileParse;


//UTILS
int 		ft_error(std::string msg);
std::string extractLocationUriStr(std::string uri);
std::string extractFileNameStr(std::string uri);
std::string extractQueryStr(std::string uri);
bool        endsWith(const std::string &str, const std::string &ending);
std::string readOutputCgi(std::string filePath);
void 		showParamsConsole(std::string &, std::string &, std::string &, std::string &, std::string &);
void 		showParamsConsoleHTTP(std::string , size_t , int, int, bool);
void 		printWaitConsole(void);

#endif