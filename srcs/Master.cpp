/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:48:47 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/19 18:23:01 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Master.hpp"

Master::Master()
{
}

Master::~Master()
{
}

Master::Master(Master const &src)
{
	*this = src;
}

Master &Master::operator=(Master const &src)
{
	if (this != &src)
	{
		//copy the attributes here
	}
	return (*this);
}


/**
 * @brief This function will return the VHost object for a given server name
 * it will iterate through the _vhosts vector and through the server_name vector of each VHost object
 * to find the correct one and return it. If the server name is not found, it will return the first VHost object
* as a default? or null? RESOLVE THIS!!!!. If servername is duplicated, it will return the first one found.
 * 
 * @param serverName 
 * @return VHost& 
 */
VHost &Master::getVHost(std::string serverName)
{
	std::vector<VHost>::iterator it = _vhosts.begin();
	for (; it != _vhosts.end(); it++)
	{
		std::vector<std::string> serverNames = (*it).getServerStruct().server_name;
		std::vector<std::string>::iterator it2 = serverNames.begin();
		for (; it2 != serverNames.end(); it2++)
		{
			if (*it2 == serverName)
				return (*it);
		}
	}
	
	return (_vhostMap[serverName]);
}

//print server names, for debugging purposes. iterating through vhosts
void Master::printServerNames()
{
	std::vector<VHost>::iterator it = _vhosts.begin();
	std::cout << "Printing server names" << std::endl;
	for (int i = 0; i < _vhosts.size(); i++)
	{
		std::vector<std::string> serverNames = _vhosts[i].getServerStruct().server_name;
		std::vector<std::string>::iterator it2 = serverNames.begin();
		std::cout<< CYAN <<"HOST number: "<<RESET<< i << std::endl;
		for (; it2 != serverNames.end(); it2++)
		{
			std::cout<<YELLOW << "Server name: " <<RESET<< *it2 << std::endl;
		}
	}
}

/**
 * @brief Here we will create the VHost objects for each server block in the configuration file
 * Also, we will populate the _vhostMap with the server name as the key and the VHost object as the value
 * Only one VHost object will be created for each server block, even if the server block has multiple server names.
 * @param config 
 * @return int 
 */
int Master::createVHosts(FileParse &config)
{
	std::vector<t_server> _servers = config.getStruct().serverData;
	if (_servers.size() == 0)
		ft_error("No servers found in the configuration file");
	//iterate the vector and confirm that the server names are correctly stored
	std::vector<t_server>::iterator it = _servers.begin();
	for(; it < _servers.end(); ++it)
	{
		VHost vhost;
		vhost.setServer(*it);
		_vhosts.push_back(vhost);
		//populate the map with the server name as the key and the VHost object as the value
		std::vector<std::string>::iterator it2 = (*it).server_name.begin();
		for (; it2 != (*it).server_name.end(); it2++)
			_vhostMap[*it2] = vhost;
	}
	return (0);
}

int Master::setSockets(std::vector<int> ports)
{
	std::vector<int>::iterator it = ports.begin();
	for (; it != ports.end(); ++it)
	{
		int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSocket < 0)
			ft_error("Error creating socket");
		struct sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(*it);
		serverAddr.sin_addr.s_addr = INADDR_ANY;
		
		if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
			ft_error("Error binding socket");
		if (listen(serverSocket, 10) < 0)
		 	ft_error("Error listening on socket");
		_ListenSockets.push_back(serverSocket);
		std::cout << GREEN << "Socket " << serverSocket << " listening on port " << *it << RESET << std::endl;
	}
	return (0);
}

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
					std::ifstream file("../html/index.html");
					std::stringstream bufferr;
					bufferr << file.rdbuf();
					std::string file_contents = bufferr.str();
					const char *response_headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
					send(socketToAccept, response_headers, strlen(response_headers), 0);
					send(socketToAccept, file_contents.c_str(), file_contents.size(), 0);
					close(socketToAccept);
					std::cout << GREEN "Response sent" RESET << std::endl;
				}
			}
		}
	}


	
	return (0);
}

