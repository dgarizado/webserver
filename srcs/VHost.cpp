/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VHost.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 15:11:51 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/19 00:32:18 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/VHost.hpp"

VHost::VHost()
{
}

VHost::VHost(FileParse &config)
{
	//parse the configuration and store it in the VHost object
}

VHost::~VHost()
{
}

VHost::VHost(VHost const &src)
{
	*this = src;
}

VHost &VHost::operator=(VHost const &src)
{
	if (this != &src)
	{
		//copy the attributes here
	}
	return (*this);
}

void VHost::setServer(t_server server)
{
	_server = server;
}

