/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   master2.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:35:35 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/08 13:11:34 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Master.hpp"


int Master::servePage(Connection connection, const std::string &path)
{
	std::ifstream file(path);
	std::stringstream bufferr;
	bufferr << file.rdbuf();
	std::string file_contents = bufferr.str();
	const char *response_headers = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n";
	send(connection.getClientSocket(), response_headers, strlen(response_headers), 0);
	send(connection.getClientSocket(), file_contents.c_str(), file_contents.size(), 0);
	return (0);
}

/**
 * @brief get the buffer from the client object and process the request. 
 * 
 * @param clientSocket 
 * @return int 
 */
int Master::processRequest(int clientSocket, RequestParser &request)
{
    //uriCheck(client);
    std::cout << MAGENTA "Processing request" << RESET << std::endl;
    if (_clientsMap[clientSocket].uriCheck(request) == 404)
	{
		std::cout << RED "Error in uriCheck" RESET << std::endl;
		servePage(_clientsMap[clientSocket], "./html/errorPages/404.html");
		return (-1);
	}
	servePage(_clientsMap[clientSocket], _clientsMap[clientSocket].getPath());
    // std::cout << "Received:\n" << _clientsMap[clientSocket].getBuffer() << std::endl;
    // SERVING the ./html/index.html file
    // std::ifstream file("./html/index.html");
    // std::stringstream bufferr;
    // bufferr << file.rdbuf();
    // std::string file_contents = bufferr.str();
    // const char *response_headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    // send(clientSocket, response_headers, strlen(response_headers), 0);
    // send(clientSocket, file_contents.c_str(), file_contents.size(), 0);
    close(clientSocket);
    std::cout << GREEN "Response sent" RESET << std::endl;

    return (0);
}