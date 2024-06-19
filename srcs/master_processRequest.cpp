/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   master_processRequest.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:35:35 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/19 17:39:06 by vcereced         ###   ########.fr       */
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
	
    std::string response;

    try{
        connection.requestParse(request);
    }catch(std::exception &e) {
        connection.serveErrorPage();
        throw std::runtime_error("Request Parse: " + std::string(e.what()));
    }
    
	try{
        response = connection.genResponse();
    }catch(std::exception &e) {
        connection.serveErrorPage();
       throw std::runtime_error("gen Response: " + std::string(e.what()));
    }
	
    send(connection.getClientSocket(), response.c_str(), response.size(), 0);
	
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