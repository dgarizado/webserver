/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   master2.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:35:35 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/13 21:21:27 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Master.hpp"

/**
 * @brief get the buffer from the client object and process the request. 
 * 
 * @param clientSocket 
 * @return int 
 */
int Master::processRequest(int clientSocket, RequestParser &request)
{
    std::cout << MAGENTA "Processing request" << RESET << std::endl;
    if (_clientsMap[clientSocket].uriCheck(request) == 404)
	{
		std::cout << RED "Error in uriCheck" RESET << std::endl;
		_clientsMap[clientSocket].servePage("./html/errorPages/404.html");
		return (-1);
	}
	_clientsMap[clientSocket].servePage(_clientsMap[clientSocket].getPath());
	
    // close(clientSocket); //THIS IS IMPORTANT TO DEFINE!!
    std::cout << GREEN "Response sent" RESET << std::endl;

    return (0);
}