/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   master_manageConnection.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:35:35 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/20 10:02:33 by vcereced         ###   ########.fr       */
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
        response = connection.genResponse(request);
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

int Master::manageConnection(Connection &connection)
{
    RequestParser   request;
    VHost           VHostAssigned;

    if (this->clientRead(connection.getClientSocket()) < 0)  //read from the socket
        return ft_error("Error reading from client");

    if (request.loadConfigFromRequest(connection.getBuffer()) < 0)
        return ft_error("Error loading config from request");
        
    request.showConfig();

    VHostAssigned = this->assignVHost(request.get()["HTTP_HOST"]);

    connection = VHostAssigned;         //post-herencia, clase BASE de Connection con la clase Base asignada

    this->processRequest(connection, request);

    return 0;
}