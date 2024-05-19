/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MasterNetworking.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 18:28:09 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/19 20:09:15 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Master.hpp"
#include <sys/types.h>
#include <fcntl.h>

/**
 * @brief This function will set the sockets for the Master object
 * it will iterate through the ports vector and create a socket for each port
 * it wont return anything, but it will store the sockets in the _ListenSockets vector.
 * @param ports 
 * @return int 
 */
int Master::setSockets(std::set<int> &ports)
{
	std::set<int>::iterator it = ports.begin();
	for (; it != ports.end(); ++it)
	{
		int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSocket < 0)
			ft_error("Error creating socket");
		struct sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(*it);
		serverAddr.sin_addr.s_addr = INADDR_ANY;
		// Set socket to non-blocking
		// fcntl(serverSocket, F_SETFL, O_NONBLOCK);
		if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
			ft_error("Error binding socket");
		if (listen(serverSocket, 10) < 0)
		 	ft_error("Error listening on socket");
		_ListenSockets.push_back(serverSocket);
		std::cout << GREEN << "Sockuet " << serverSocket << " listening on port " << *it << RESET << std::endl;
	}
	return (0);
}

/**
 * @brief The networking communication will be handled here
 * using the kqueue system call.
 * 
 * @return int 
 */
int Master::setEvents()
{
	struct kevent ev;

	_kq = kqueue();
	if (_kq < 0)
		ft_error("Error creating kqueue");
	std::vector<int>::iterator it = _ListenSockets.begin();
	for(; it != _ListenSockets.end(); it++)
	{
		EV_SET(&ev, *it, EVFILT_READ, EV_ADD, 0, 0, NULL);
		if (kevent(_kq, &ev, 1, NULL, 0, NULL) < 0)
			ft_error("Error adding socket to kqueue");
	}
	return (0);
}

/**
 * @brief This function will start the event loop for the multuple VHOSTs
 * 
 * 
 * @return int 
 */
int Master::startEventLoop()
{
	struct kevent events[_ListenSockets.size() + MAX_CLIENTS];
	
	struct timespec timeout = {0, 100000000};
	//timeout is 100ms
	while (42)
	{
		int nev = kevent(_kq, NULL, 0, events, _ListenSockets.size() + MAX_CLIENTS, &timeout);
		if (nev < 0)
		 	ft_error("Error in kevent");
		for (int i = 0; i < nev; ++i)
		{
			int socketToAccept = events[i].ident;

			bool isListenSocket = false;
			std::vector<int>::iterator it = _ListenSockets.begin();
			for (; it != _ListenSockets.end(); it++)
			{
				if (*it == socketToAccept)
				{
					isListenSocket = true;
					break;
				}
			}

			if(isListenSocket)
			{
				sockaddr_in clientAddr;
				socklen_t clientAddrSize = sizeof(clientAddr);
				int clientSocket = accept(socketToAccept, (struct sockaddr *)&clientAddr, &clientAddrSize);
				if (clientSocket < 0)
					ft_error("Error accepting connection");
				std::cout << GREEN << "Accepted connection on socket " << socketToAccept << " from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << RESET << std::endl;
				struct kevent ev;
				EV_SET(&ev, clientSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
				if (kevent(_kq, &ev, 1, NULL, 0, NULL) < 0)
					ft_error("Error adding client socket to kqueue");
			} else 
			{
				char buffer[1024];
				int bytesRead = read(socketToAccept, buffer, 1024);
				if (bytesRead < 0)
				 	ft_error("Error reading from socket");
				if (bytesRead == 0)
					ft_error("Client disconnected");
				else
				{
					buffer[bytesRead] = '\0';
					std::cout << "Received: " << buffer << std::endl;
					//SERVING the ./html/index.html file
					std::ifstream file("./html/index.html"); //PATH FROM THE EXECUTABLE.
					//print the current working directory
					if (!file) {
						std::cerr << "Error opening file" << std::endl;
						ft_error("Error opening file");
					}

					std::stringstream buffer;
					buffer << file.rdbuf();
					std::string file_contents = buffer.str();

					const char *response_headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
					send(socketToAccept, response_headers, strlen(response_headers), 0);
					send(socketToAccept, file_contents.c_str(), file_contents.size(), 0);
					// send(socketToAccept, response_headers, strlen(response_headers), 0);
					// send(socketToAccept, file_contents.c_str(), file_contents.size(), 0);
					close(socketToAccept);
					std::cout << GREEN "Response sent" RESET << std::endl;
				}
			}
		}
	}	
	return (0);
}