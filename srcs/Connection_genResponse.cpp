/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection_genResponse.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 13:30:21 by vcereced          #+#    #+#             */
/*   Updated: 2024/06/21 14:43:13 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Connection.hpp"

std::string Connection::genHeaderHTTP(std::string bodyHTTP, std::string filePath)
{
    std::stringstream   buffer;
    std::string         mime_type;
    std::string         response_header;
    long                size;
    long                statusCode;
    
    mime_type = this->getMimeType(filePath);//para python ???????????????????
    statusCode = this->getStatusCode();
    size = bodyHTTP.size();

    if (statusCode == 200)
        response_header  = "HTTP/1.1 200 OK\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (statusCode == 400)
        response_header  = "HTTP/1.1 400 Bad Request\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (statusCode == 403)
        response_header  = "HTTP/1.1 403 Forbidden\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (statusCode == 404)
        response_header  = "HTTP/1.1 404 Not Found<\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (statusCode == 405)
        response_header  = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (statusCode == 500)
        response_header  = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";

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
    //this->_statusCode = 404;
    throw ServerException("getValidDefaultIndex: not any index found ", 404);
   // throw std::runtime_error("getValidDefaultIndex: not any index found");
    return "";
}

std::string Connection::genPathDefaultIndex(void)
{
    std::string                 responseHTTP;
    std::string                 defaultPath;
    
    defaultPath = this->getPath();
    
    defaultPath += getValidDefaultIndex();
    
    return defaultPath;
}

std::ifstream openFile(std::string filePath)
{
    if (access(filePath.c_str(), F_OK) == -1)
        throw ServerException("openFile: not found: " + filePath, 404);
    if (access(filePath.c_str(), R_OK) == -1)
        throw ServerException("openFile: Permision not allowed: " + filePath, 403);
    
    std::ifstream       file(filePath);
    if (!file)
        throw ServerException("openFile: internal error: " + filePath, 500);
    
    return file;
}


std::string Connection::genBodyFile(std::string filePath)
{
    std::string         responseHTTP_body;
    std::ifstream       file;
    std::stringstream   buffer;

    try{
       file = openFile(filePath);
    } catch (const ServerException &e) {
        throw ServerException("genBodyFile: " + std::string(e.what()), e.getCode());
    }
    // if (!file){
    //    // this->_statusCode = 404;
    //   //  throw std::runtime_error("genBodyFile: Cannot open file:" + filePath);
    //     throw ServerException("genBodyFile: " + std::string(strerror(errno) + filePath), 404);
    // }
    
    buffer << file.rdbuf();
    responseHTTP_body = buffer.str();
    
    this->_statusCode = 200;
    
    return responseHTTP_body;
}

std::string Connection::genBodyHTTP(std::string filePath, RequestParser &request)
{
    std::string         responseHTTP_body;
    
    if (endsWith(filePath, "py"))
        responseHTTP_body = genBodyCgi(filePath, request);
    else
        responseHTTP_body = genBodyFile(filePath);
    
    return responseHTTP_body;
}

std::string Connection::genResponseGET(RequestParser &request)
{
    std::vector<std::string>    defaultIndexs;
    std::string                 responseHTTP_header;
    std::string                 responseHTTP_body;
    bool                        autoIndex;

    defaultIndexs = this->getLocation().index;
    autoIndex = this->getLocation().autoIndex;

    if (this->_fileName.empty() == false)
    {
        responseHTTP_body = genBodyHTTP(this->_path, request);
    }  
    else if (defaultIndexs.empty() == false)
    {
        this->_path = genPathDefaultIndex();
        responseHTTP_body = genBodyHTTP(this->_path, request);
    }
    else if (autoIndex == true)
    {
       responseHTTP_body = genBodyAutoIndex(this->_path);
    }
    else
        throw ServerException("genResponse: not default index and autoindex defined ", 404);
        
    responseHTTP_header = genHeaderHTTP(responseHTTP_body, this->_path);

    return responseHTTP_header + responseHTTP_body;
}

std::string Connection::genResponse(RequestParser &request)
{
    std::string method;
    std::string response;
    
    method = request.get()["REQUEST_METHOD"];
    if (method == "GET")
        return genResponseGET(request);
   // else if (method == "POST")
    //    response = genResponsePOST(request);
    else
        throw ServerException("genResponse: method not configured: " + method, 400);
}