/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:11:34 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/26 20:46:38 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MASTER_HPP
# define MASTER_HPP
# define MAX_CLIENTS 100
#include "webserv.hpp"
#include "FileParse.hpp"
#include "VHost.hpp"
#include "Connection.hpp"
#include "RequestParser.hpp"
#include <exception>
#include <sstream>
#include <string>

class VHost;
class Connection;

class Master
{
	public:
		Master();
		~Master();
		Master(Master const &src);
		Master &operator=(Master const &src);

		int createVHosts(FileParse &config);
		
		//NETWORK METHODS
		int setSockets(std::set<int> &ports);
		int setEvents();
		int startEventLoop();
		int clientAccept(int serverSocket);
		int clientRead(int clientSocket);
		int processRequest(int clientSocket);
		int manageConnection(int clientSocket);

		//FINDS AND RETURNS THE VHOST OBJECT FOR A GIVEN SERVER NAME
		VHost &getVHost(std::string serverName, int port);
		VHost &assignVHost(std::string);

		//AUXILIARY FUNCTIONS
		void printServerNames();
		
	private:

		std::vector<int> _ListenSockets;
		std::vector<int> _clientSockets;
		std::vector<VHost> _vhosts;
		std::vector<t_server> _servers;
		std::map<std::string, VHost> _vhostMap;
		std::map<int, Connection> _clientsMap;
		int _kq;
		int _epoll_fd;
};


#endif