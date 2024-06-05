/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 20:12:40 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/05 20:35:54 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Connection.hpp"

Connection::Connection() : _clientSocket(0), _clientAddrSize(sizeof(_clientAddr))
{
}   

Connection::~Connection()
{
}

Connection::Connection(Connection const &src)
{
    *this = src;
}

Connection &Connection::operator=(Connection const &src)
{
    if (this != &src)
    {
        _clientSocket = src._clientSocket;
        _clientAddr = src._clientAddr;
        _clientAddrSize = src._clientAddrSize;
        _ev = src._ev;
        _buffer = src._buffer;
        // _vhost = src._vhost;
    }
    return (*this);
}

int Connection::getClientSocket() const
{
    return _clientSocket;
}

std::string Connection::getBuffer() const
{
    return _buffer;
}


void Connection::setClientData(int clientSocket, sockaddr_in clientAddr, socklen_t clientAddrSize, struct epoll_event ev)
{
    _clientSocket = clientSocket;
    _clientAddr = clientAddr;
    _clientAddrSize = clientAddrSize;
    _ev = ev;
}

void Connection::setBuffer(std::string buffer)
{
    _buffer = buffer;
}

void Connection::setVhost(VHost vhost)
{
    _vhost = vhost;
}

/**
 * @brief this function checks if the uri is a defined location in the vhost configuration.
 * 
 * @param directory 
 * @return int 
 */
int Connection::dirCheck(std::string directory)
{
    std::vector<t_location> locations = _vhost.getServerStruct().locations;
    for (std::vector<t_location>::iterator it = locations.begin(); it != locations.end(); it++)
    {
        if (it->uri == directory)
        {
            std::cout << GREEN "Location found:" RESET <<  it->uri << std::endl;
            return (1);
        }
    }
    std::cout << RED "Location not found:" RESET << directory << std::endl;
    return (0);
}

/**
 * @brief This is part of parsing the request. It checks if the uri is a directory or a file.
 * 
 * @param request 
 * @return int 
 */
int Connection::uriCheck(RequestParser &request)
{
    std::string uri = request.get()["REQUEST_URI"];
    std::string::size_type pos;

    //Separate the uri in tokens with ? as delimiter
    std::stringstream   iss(uri);
    std::string         path;
    std::string         queryString;
    std::getline(iss, path, '?');
    std::getline(iss, queryString, '?');
	
    std::cout << BLUE "Path: " RESET << path << std::endl;
    std::cout << BLUE "Query: " RESET << queryString << std::endl;

    pos = path.rfind('/');
    std::string directory = path.substr(0, pos);
    std::string filename = pos == std::string::npos ? path : path.substr(pos + 1);
    //if the filename does not have an extension, it is a directory. recomplete the directory path
    if (filename.find('.') == std::string::npos)
    {
        directory = path;
        filename = "";
    }
    
    std::cout << BLUE "Directory: " RESET << directory << std::endl;
    //get just the first part of the directory
    pos = directory.find('/', 1);
    directory = directory.substr(0, pos);
    std::cout << BLUE "DirectoryParsed: " RESET << directory << std::endl;
    std::cout << BLUE "Filename: " RESET << filename << std::endl;
    dirCheck(directory);
    //check if the directory exists in the vhost locations
    
    return (0);
}