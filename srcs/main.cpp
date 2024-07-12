/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:41:19 by dgarizad          #+#    #+#             */
/*   Updated: 2024/07/12 19:20:59 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"
#include "../includes/Master.hpp"
#include "../includes/FileParse.hpp"
#include "../includes/VHost.hpp"

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#define PORT 8198

int main(int argc, char **argv)
{
	FileParse 	fileParser;
	Master 		master;
	std::string path;

	
	if (argc > 2)
		return (ft_error("Bad arguments, use: ./webserv [config_file]"));	
	else if (argc == 2 )
		path = argv[1];
	else
		path = "./config/default.com";

	std::cout << GREEN << "Parsing configuration file: " << path << RESET<< std::endl;
	
	try{		
		fileParser.loadConfigFromFile(path);
	} catch (std::exception &e){
		return ft_error("Exception: main: "+ std::string(e.what()));
	}

	fileParser.showConfig();
	master.createVHosts(fileParser);
	master.setSockets(fileParser.getStruct().ports);
	master.setErrPages(fileParser.getStruct().errPageMap);
	master.setclientMaxBodySize(fileParser.getStruct().clientMaxBodySize);
	master.setEvents();
	master.startEventLoop();
	return (0);
}


// #include <sys/epoll.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <iostream>
// #include <cstring>

// #define MAX_EVENTS 10
// #define PORT 8081

// void setNonBlocking1(int sockfd) {
//     int flags = fcntl(sockfd, F_GETFL, 0);
//     if (flags == -1) {
//         std::cerr << "Error getting socket flags\n";
//         exit(EXIT_FAILURE);
//     }
//     if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
//         std::cerr << "Error setting non-blocking\n";
//         exit(EXIT_FAILURE);
//     }
// }

// int main() {
//     int server_fd, new_socket, epoll_fd;
//     struct epoll_event event, events[MAX_EVENTS];
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);

//     // Create server socket
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//         perror("socket failed");
//         exit(EXIT_FAILURE);
//     }

//     // Set server socket to non-blocking
//     setNonBlocking1(server_fd);

//     // Bind to port 8080
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);
//     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
//         perror("bind failed");
//         exit(EXIT_FAILURE);
//     }

//     if (listen(server_fd, 10) < 0) {
//         perror("listen");
//         exit(EXIT_FAILURE);
//     }

//     // Create epoll instance
//     epoll_fd = epoll_create1(0);
//     if (epoll_fd == -1) {
//         perror("epoll_create1");
//         exit(EXIT_FAILURE);
//     }

//     event.events = EPOLLIN;
//     event.data.fd = server_fd;
//     if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
//         perror("epoll_ctl: server_fd");
//         exit(EXIT_FAILURE);
//     }

//     // Event loop
//     while (true) {
//         int n = epoll_wait(epoll_fd, events, MAX_EVENTS, 300);
//         for (int i = 0; i < n; i++) {
//             if (events[i].data.fd == server_fd) {
//                 // Accept connection
//                 new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
// 				std::cout << "Accepted connection!" << std::endl;
//                 if (new_socket < 0) {
//                     perror("accept");
//                     exit(EXIT_FAILURE);
//                 }
//                 setNonBlocking1(new_socket);
//                 event.data.fd = new_socket;
//                 event.events = EPOLLIN | EPOLLET;
//                 epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_socket, &event);
//             } else {
//                 // Handle client request
//                 char buffer[1024] = {0};
//                 read(events[i].data.fd, buffer, 1024);
//                 // Send simple HTTP response
//                 char response[] = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, world!";
//                 write(events[i].data.fd, response, sizeof(response));
//                 close(events[i].data.fd);
//             }
//         }
//     }

//     close(server_fd);
//     return 0;
// }