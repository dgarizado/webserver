/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection_genResponsePage.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 13:30:21 by vcereced          #+#    #+#             */
/*   Updated: 2024/06/19 14:31:29 by vcereced         ###   ########.fr       */
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
    
    if (!file){
        _statusCode = 404;
        std::cout << RED << "genResponsePage: Cannot open file: " << filePath <<  RESET << std::endl;
    }

    buffer << file.rdbuf();
    file_content = buffer.str();
    mime_type = this->getMimeType(filePath);
    size      = file_content.size();

    std::cout << GREEN "Serving page" RESET << std::endl;
    
    if (_statusCode == 200)
        response  = "HTTP/1.1 200 OK\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (_statusCode == 404)
        response  = "HTTP/1.1 404 Not Found\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (_statusCode == 405)
        response  = "HTTP/1.1 Method Not Allowed\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";

    response += file_content;
    std::cerr << std::endl << GREEN << response << RESET << std::endl;
    return response;
}

std::string Connection::genResponseDefaultIndexPage(void)
{
    
}

std::string Connection::genResponse(void)
{
    std::vector<std::string>    defaultIndex;
    std::string                 response;
    std::string                 file;
    bool                        autoIndex;

    file = this->_file;
    defaultIndex = this->getLocation()->index;
    autoIndex = this->getLocation()->autoIndex;
    
    if (file.empty() == false)
    {
        response = genResponsePage(this->_path);
    }
    else if (defaultIndex.empty() == false)
    {
        response = genResponseDefaultIndexPage();
    }
    // else if (autoIndex == true)
    // {
        
    // }
    // else
    // {
    //     this->_statusCode = 404;//ni idea del error aqui!!!!!!!!!!!!!!
    //     this->serveErrorPage();
    // }
    

    
}
