/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection3.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 13:30:21 by vcereced          #+#    #+#             */
/*   Updated: 2024/06/16 17:45:38 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Connection.hpp"

std::string Connection::genResponsePage(std::string filePath)
{
    std::stringstream   buffer;
    std::ifstream       file(filePath);
    std::string         file_content;
    std::string         mime_type;
    std::string         response;
    long                size;
    
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filePath);
        _statusCode = 404;
    }

    buffer << file.rdbuf();
    file_content = buffer.str();
    mime_type = this->getMimeType(filePath);
    size      = file_content.size();

    std::cout << GREEN "Serving page" RESET << std::endl;
    
    if (_statusCode == 200)
        response  = "HTTP/1.1 200 OK\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (_statusCode == 404)
    {
        std::cout << RED "ERRORR WUAAAAAAAAAAAA!" RESET << std::endl;
        response  = "HTTP/1.1 404 Not Found\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    }
    else if (_statusCode == 405)
        response  = "HTTP/1.1 Method Not Allowed\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";

    response += file_content;
    return response;
}

