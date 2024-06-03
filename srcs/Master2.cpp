/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master2.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 21:05:38 by vcereced          #+#    #+#             */
/*   Updated: 2024/06/03 18:32:13 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Master.hpp"




/**
 * @brief get the buffer from the client object and process the request. 
 * 
 * @param clientSocket 
 * @return int 
 */
int Master::processRequest(int clientSocket)
{
    
    //std::cout << "Received:\n" << _clientsMap[clientSocket].getBuffer() << std::endl;
    // SERVING the ./html/index.html file
    std::ifstream file("./html/index.html");
    std::stringstream bufferr;
    bufferr << file.rdbuf();
    std::string file_contents = bufferr.str();
    const char *response_headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    send(clientSocket, response_headers, strlen(response_headers), 0);
    send(clientSocket, file_contents.c_str(), file_contents.size(), 0);
    close(clientSocket);
    std::cout << GREEN "Response sent" RESET << std::endl;

    return (0);
}