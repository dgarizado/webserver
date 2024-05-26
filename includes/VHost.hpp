/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VHost.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:17:12 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/26 18:02:36 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VHOST_HPP
# define VHOST_HPP
# include "webserv.hpp"

class FileParse;

class VHost
{
	public:
		VHost();
		VHost(FileParse &config);
		~VHost();
		VHost(VHost const &src);
		VHost &operator=(VHost const &src);

		//getters
		t_server &getServerStruct();
		std::string getServerName();
		
		//setters
		void setServer(t_server &server);
		//AUXILIARY FUNCTIONS
		void printServerNames();

		//VHOST WILL HAVE THE METHODS 

	private:
		//Here we will store the VHost configuration
		std::string _server_name;
		std::string _root;
		std::string _index;
		std::vector<std::string> _error_pages;
		std::vector<std::string> _locations;
		in_port_t _port;
		int _listen;
		int _max_body_size;
		bool _auto_index; 
		bool _cgi;
		bool _redirection;
		std::string _cgi_path;
		std::vector<std::string> _cgi_extensions;
		//NETWORK
		int _serverSocket;
		struct sockaddr_in _serverAddr;

		//THIS IS THE SERVER BLOCK STRUCTURE
		t_server _server;
		
	//having a static variable for storing all the VHosts sockets in a set
	static std::set<int> _ListenSockets;
	
};
#endif