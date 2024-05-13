/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VHOST.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:17:12 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/13 16:57:40 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VHOST_HPP
# define VHOST_HPP
# include "webserv.hpp"

class VHOST
{
	public:
		VHOST();
		VHOST(ConfParse &config);
		~VHOST();
		VHOST(VHOST const &src);
		VHOST &operator=(VHOST const &src);

	private:
		//Here we will store the VHost configuration
		std::string _server_name;
		std::string _root;
		std::string _index;
		std::vector<std::string> _error_pages;
		std::vector<std::string> _locations;
		in_port_t _port;
		int _max_body_size;
		bool _auto_index;
		bool _cgi;
		bool _redirection;
		std::string _cgi_path;
		std::vector<std::string> _cgi_extensions;
		//NETWORK
		int _serverSocket;
		struct sockaddr_in _serverAddr;
		
		
	
};
#endif