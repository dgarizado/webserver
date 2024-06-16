/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   master2.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:35:35 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/16 20:36:22 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Master.hpp"

int isAutoIndex(Connection &connection)
{
	if (connection.getAutoIndex() == true && connection.getFileName().empty())
		return 1;
	return 0;
}

bool isPage(Connection &connection)
{
	std::string filename = connection.getPath();

	if (connection.endsWith(filename, ".html")) return true;
	if (connection.endsWith(filename, ".php"))  return true;
	if (connection.endsWith(filename, ".css"))  return true;
	if (connection.endsWith(filename, ".jpg"))  return true;
	
	return false;
}

bool isCgi(Connection &connection)
{
	std::string filename = connection.getPath();
	
	if (connection.endsWith(filename, ".py")) return true;

	return false;
}


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
    std::string path;

	if (connection.requestCheck(request) == -1)
		return -1;
	
	std::cout << RED << "FinalPATh" << connection.getFinalPath()<< RESET << std::endl;
	std::cout << RED << "PATh" << connection.getPath()<< RESET << std::endl;
	
	if (isAutoIndex(connection))
	{
        path     = connection.getFinalPath();
		//response = connection.genAutoIndex(path);
		response = connection.genResponseAutoIndex(path);
		send(connection.getClientSocket(), response.c_str(), response.size(), 0);
	}
	else if (isPage(connection))
	{
        path     = connection.getPath();
        response = connection.genResponsePage(path);
        send(connection.getClientSocket(), response.c_str(), response.size(), 0);
	}
	else if (isCgi(connection))
	{
		path 	 = connection.getPath();
		response = connection.genResponseCgi(path, request);
		send(connection.getClientSocket(), response.c_str(), response.size(), 0);
	}
	else
		std::cout << RED "Error in uriCheck" RESET << std::endl;
	
		
	//else if (isCgi(connection))

	

	
	// 	genAutoIndex(connection.getFinalPath()) ;
	
	// 	send (verveePage)
	// AUTOINDEX
	// servirpagina
	// cgi
    //if (connection.uriCheck(request) != 200)
	// {
	// 	std::cout << RED "Error in uriCheck" RESET << std::endl;
	// 	connection.servePage("./html/errorPages/404.html");
	// 	return (-1);
	// }
	// if (connection.methodCheck(request) == false)
	// {
	// 	std::cout << RED "Error in Method check" RESET << std::endl;
	// 	connection.servePage(".html/errorPages/405.html");
	// 	return (-1);
	// }
	
	//connection.servePage(connection.getPath());
	
    // close(clientSocket); //THIS IS IMPORTANT TO DEFINE!!
    std::cout << GREEN "Response sent" RESET << std::endl;

    return (0);
}