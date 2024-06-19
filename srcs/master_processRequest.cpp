/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   master_processRequest.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:35:35 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/19 20:48:38 by vcereced         ###   ########.fr       */
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
       throw std::runtime_error("genResponse: " + std::string(e.what()));
    }
	
    std::cout << GREEN <<  "MESSAGE HTTP TO SENT\n------------\n" << response << RESET << std::endl;
    send(connection.getClientSocket(), response.c_str(), response.size(), 0);
    close(connection.getClientSocket());// IMPORTANT TO DEFINE!! ?????????
    std::cout << GREEN "----------------------------\nResponse sent" RESET << std::endl;

    return (0);
}