/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:48:47 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/19 18:43:19 by dgarizad         ###   ########.fr       */
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



