/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection_genResponse.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 13:30:21 by vcereced          #+#    #+#             */
/*   Updated: 2024/06/19 17:38:42 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Connection.hpp"

std::string Connection::genBodyHTTP(std::string filePath)
{
    std::ifstream       file(filePath);
    std::stringstream   buffer;
    std::string         file_content;

    if (!file){
        _statusCode = 404;
        throw std::runtime_error("genBodyHTTP: Cannot open file:" + filePath);
    }
    buffer << file.rdbuf();
    file_content = buffer.str();
    
    this->_statusCode = 200;
    return file_content;
    
}


std::string Connection::genResponsePage(std::string filePath)
{
    std::stringstream   buffer;
    std::string         mime_type;
    std::string         response_header;
    std::string         response_body;
    long                size;
    
    
    std::cout << GREEN "Serving page" RESET << std::endl;
    response_body = genBodyHTTP(filePath);
     std::cout << GREEN "AFTER Serving page" RESET << std::endl;
    mime_type = this->getMimeType(filePath);//para python ???????????????????
    size      = response_body.size();

   
    if (_statusCode == 200)
        response_header  = "HTTP/1.1 200 OK\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (_statusCode == 404)
        response_header  = "HTTP/1.1 404 Not Found\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (_statusCode == 405)
        response_header  = "HTTP/1.1 Method Not Allowed\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    
    std::cerr << GREEN << response_header << RESET << std::endl;
    std::cerr << GREEN << response_body << RESET << std::endl;
    
    return response_header + response_body;
}

std::string Connection::getValidDefaultIndex(void)
{
    std::string path = this->getPath();

    std::vector<std::string>::iterator it = this->_location.index.begin();
    for (; it != this->_location.index.end(); it++)
    {
        path = this->getPath() + *it;
        if (access(path.c_str(), F_OK)  != -1)
        {
            this->_statusCode = 200;
            return *it;
        }
    }
    this->_statusCode = 404;
    throw std::runtime_error("getValidDefaultIndex: not valid index" );
    return "";
}

std::string Connection::genResponseDefaultIndexPage(void)
{
    std::string                 responseHTTP;
    std::string                 defaultPath;
    
    defaultPath = this->getPath();
    
    defaultPath += getValidDefaultIndex();
    
    responseHTTP = genResponsePage(defaultPath);

    return responseHTTP;
}

std::string Connection::genResponse(void)
{
    std::vector<std::string>    defaultIndexs;
    std::string                 responseHTTP;
    std::string                 fileName;
    bool                        autoIndex;

    fileName = this->_fileName;
    defaultIndexs = this->getLocation().index;
    autoIndex = this->getLocation().autoIndex;
    
    if (fileName.empty() == false)
    {
        responseHTTP = genResponsePage(this->_path);
    }
    else if (defaultIndexs.empty() == false)
    {
        responseHTTP = genResponseDefaultIndexPage();
    }
    // else if (autoIndex == true)
    // {
        
    
    // }

    return responseHTTP;

    
}
