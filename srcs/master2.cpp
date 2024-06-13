/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   master2.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:35:35 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/13 20:55:08 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Master.hpp"

bool ends_with(const std::string& value, const std::string& ending) {
    if (ending.size() > value.size()) {
        return false;
    }
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::string getMimeType(const std::string &path)
{
	if (ends_with(path, ".html")) {
        return "text/html";
    } else if (ends_with(path, ".css")) {
        return "text/css";
    } else if (ends_with(path, ".jpg") || ends_with(path, ".jpeg")) {
        return "image/jpeg";
    } else if (ends_with(path, ".png")) {
        return "image/png";
    } else {
        // Default to application/octet-stream for unknown types
        return "application/octet-stream";
    }
}

int Master::servePage(Connection &connection, const std::string &path)
{
	std::ifstream file(path);
	std::stringstream bufferr;
	bufferr << file.rdbuf();
	std::string file_contents = bufferr.str();


	std::cout << "Status code: " << connection.getStatusCode() << std::endl;
	//NEED TO IMPROVE FOR OTHER STATUS CODES. FIX THIS.
	if (connection.getStatusCode() == 200)
	{
		std::cout << GREEN "Serving page" RESET << std::endl;
		std::string mime_type = getMimeType(path);
		std::string response_headers = "HTTP/1.1 200 OK\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(file_contents.size()) + "\r\n\r\n";
		send(connection.getClientSocket(), response_headers.c_str(), response_headers.size(), 0);
		send(connection.getClientSocket(), file_contents.c_str(), file_contents.size(), 0);

		return (0);
	}
	std::cout << RED "Error in servePage" RESET << std::endl;
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
    std::cout << MAGENTA "Processing request" << RESET << std::endl;
    if (_clientsMap[clientSocket].uriCheck(request) == 404)
	{
		std::cout << RED "Error in uriCheck" RESET << std::endl;
		servePage(_clientsMap[clientSocket], "./html/errorPages/404.html");
		return (-1);
	}
	servePage(_clientsMap[clientSocket], _clientsMap[clientSocket].getPath());
    close(clientSocket);
    std::cout << GREEN "Response sent" RESET << std::endl;

    return (0);
}