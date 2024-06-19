/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   master_processRequest.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:35:35 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/19 14:26:10 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Master.hpp"


// int isAutoIndex(Connection &connection)
// {
// 	if (connection.getAutoIndex() == true && connection.getFileName().empty())
// 		return 1;
// 	return 0;
// }



/**
 * @brief get the buffer from the client object and process the request. 
 * 
 * @param clientSocket 
 * @return int 
 */
int Master::processRequest(Connection &connection, RequestParser &request)
{
    std::cout << MAGENTA "Processing request" << RESET << std::endl;
	
    std::string response;

    try{
        connection.requestParse(request);
    }catch(std::exception &e) {
        throw std::runtime_error("Request Parse: " + std::string(e.what()));
    }
    
	try{
        response = connection.genResponse();
    }catch(std::exception &e) {
        throw std::runtime_error("gen Response: " + std::string(e.what()));
    }
		
	
	// if (isAutoIndex(connection))
	// {
    //     path     = connection.getFinalPath();
	// 	response = connection.genAutoIndex(path);
	// 	send(connection.getClientSocket(), response.c_str(), response.size(), 0);
	// }
	// else
	// {
    //     path     = connection.getPath();
    //     response = connection.genResponsePage(path);
    //     send(connection.getClientSocket(), response.c_str(), response.size(), 0);
	// }

	
    //close(clientSocket); //THIS IS IMPORTANT TO DEFINE!!
    std::cout << GREEN "Response sent" RESET << std::endl;

    return (0);
}