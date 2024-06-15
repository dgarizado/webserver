/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   master2.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:35:35 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/15 15:54:57 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Master.hpp"

/**
 * @brief get the buffer from the client object and process the request. 
 * 
 * @param clientSocket 
 * @return int 
 */
int Master::processRequest(Connection &connection, RequestParser &request)
{
    std::cout << MAGENTA "Processing request" << RESET << std::endl;
    if (connection.uriCheck(request) == 404)
	{
		std::cout << RED "Error in uriCheck" RESET << std::endl;
		connection.servePage("./html/errorPages/404.html");
		return (-1);
	}
	if (connection.methodCheck(request) == false)
	{
		std::cout << RED "Error in Method check" RESET << std::endl;
		connection.servePage(".html/errorPages/405.html") 
		return (-1);
	}
	
	connection.servePage(connection.getPath());
	
    // close(clientSocket); //THIS IS IMPORTANT TO DEFINE!!
    std::cout << GREEN "Response sent" RESET << std::endl;

    return (0);
}