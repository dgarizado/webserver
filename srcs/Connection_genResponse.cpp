/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection_genResponse.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 13:30:21 by vcereced          #+#    #+#             */
/*   Updated: 2024/06/19 21:21:15 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Connection.hpp"

std::string Connection::genBodyHTTP(std::string filePath)
{
    std::ifstream       file(filePath);
    std::stringstream   buffer;
    std::string         file_content;
    
    if (endsWith(filePath, "py"))
        
    
    if (!file){
        _statusCode = 404;
        throw std::runtime_error("genBodyHTTP: Cannot open file:" + filePath);
    }
    buffer << file.rdbuf();
    file_content = buffer.str();
    
    this->_statusCode = 200;
    return file_content;
    
}


std::string Connection::genHeaderHTTP(std::string bodyHTTP, std::string filePath)
{
    std::stringstream   buffer;
    std::string         mime_type;
    std::string         response_header;
    long                size;
    
    
    mime_type = this->getMimeType(filePath);//para python ???????????????????
    size      = bodyHTTP.size();

    std::cout << "generando cabezera: _stausCode" <<_statusCode << std::endl;

    if (_statusCode == 200)
        response_header  = "HTTP/1.1 200 OK\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (_statusCode == 404)
        response_header  = "HTTP/1.1 404 Not Found\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (_statusCode == 405)
        response_header  = "HTTP/1.1 Method Not Allowed\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";

     std::cout << "generando cabezera: response header: " <<response_header << std::endl;
    return response_header;
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

std::string Connection::genPathDefaultIndex(void)
{
    std::string                 responseHTTP;
    std::string                 defaultPath;
    
    defaultPath = this->getPath();
    
    defaultPath += getValidDefaultIndex();
    
    //this->_path = defaultPath;
    
   // responseHTTP = genBodyHTTP(defaultPath);

    return defaultPath;
}

std::string Connection::genResponse(void)
{
    std::vector<std::string>    defaultIndexs;
    std::string                 responseHTTP_header;
    std::string                 responseHTTP_body;
    std::string                 fileName;
    bool                        autoIndex;

    fileName = this->_fileName;
    defaultIndexs = this->getLocation().index;
    autoIndex = this->getLocation().autoIndex;

    
    if (fileName.empty() == false)
    {
        responseHTTP_body = genBodyHTTP(this->_path);
    }  
    else if (defaultIndexs.empty() == false)
    {
        this->_path = genPathDefaultIndex();
        responseHTTP_body = genBodyHTTP(this->_path);
    }
    else if (autoIndex == true)
    {
       responseHTTP_body = genBodyAutoIndex(this->_path);
    }
    
    std::cout << "path que entra a generar  GEN RESPONSE HTTP: " << this->_path << std::endl;
    responseHTTP_header = genHeaderHTTP(responseHTTP_body, this->_path);
     std::cout << "header HTTP creado: " << responseHTTP_header << std::endl;
    return responseHTTP_header + responseHTTP_body;
}
