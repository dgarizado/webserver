/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VHost.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:11:51 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/19 18:23:50 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/VHost.hpp"

VHost::VHost(): _server_name("default VISTRO")
{
	// std::cout << "\nONE VHOST HAS BORN!" << std::endl;
}

VHost::VHost(FileParse &config)
{
	//parse the configuration and store it in the VHost object
}

VHost::~VHost()
{
	// std::cout << "ONE VHOST HAS BEEN DESTROYED!" << std::endl;
}

VHost::VHost(VHost const &src)
{
	*this = src;
	// std::cout << "COPY CONSTRUCTOR OF VHOST!" << std::endl;
}

VHost &VHost::operator=(VHost const &src)
{
	// std::cout << "COPY ASSINGMENT! IS VOID" << std::endl;
	
	if (this != &src)
	{
		//copy the attributes here
		_server_name = src._server_name;
		_server = src._server;
	}
	return (*this);
}

//GETTERS

/**
 * @brief This function will return the server struct of the VHost object
 * 
 * @return t_server 
 */
t_server &VHost::getServerStruct()
{
	return (_server);
}

std::string VHost::getServerName()
{
	return (_server.server_name[0]);
}

void VHost::setServer(t_server &server)
{
	_server = server;
}

void VHost::printServerNames()
{
	//this print the servernames stored in the t_server struct t_server _server;
	std::vector<std::string>::iterator it = _server.server_name.begin();
	for (; it != _server.server_name.end(); it++)
	{
		std::cout << *it << std::endl;
	}
}