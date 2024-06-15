/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 20:12:40 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/15 15:08:12 by dgarizad         ###   ########.fr       */
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
        _directory = src._directory;
        _finalPath = src._finalPath;
        _path = src._path;
        _fileName = src._fileName;
        _queryString = src._queryString;
        _root = src._root;
        _statusCode = src._statusCode;
        _vhost = src._vhost;
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

std::string Connection::getPath() const
{
	return _path;
}

int Connection::getStatusCode() const
{
	return _statusCode;
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


bool Connection::fileCheck(std::string file)
{
	if (_fileName.empty() == true)
	{
		std::cout << YELLOW "No file requested, serving index or autoindex" RESET << std::endl;
		_path += "/index.html"; //THIS IS HARDCODED! MANAGE THIS with the autoindex or index directive.
		return (true);
	}
	std::cout << BLUE "File requested: " RESET << file << std::endl;
	std::ifstream f(file.c_str());
	if (f.good())
	{
		std::cout << GREEN "File found: 200!" RESET << std::endl;
		_statusCode = 200;
		return (true);
	}
	_statusCode = 404;
	std::cout << RED "File not found: 404!" RESET << std::endl;
	return (false);
}

/**
 * @brief this function checks if the uri is a defined location in the vhost configuration.
 * 
 * @param directory 
 * @return int 
 */
bool Connection::dirCheck(std::string directory)
{
    std::vector<t_location> locations = _vhost.getServerStruct().locations;
    for (std::vector<t_location>::iterator it = locations.begin(); it != locations.end(); it++)
    {
        if (it->location == directory)
        {
            std::cout << MAGENTA "Location found:" RESET <<  it->uri << std::endl;
            _location = *it;
            _root = it->root;
			_statusCode = 200;
            return (true);
        }
    }
    // std::cout << RED "Location not found: 404!" RESET << directory << std::endl;
    _statusCode = 404;
	return (false);
}

/**
 * @brief This function fixes the path of the request.
 * It replaces the directory with the root of the location.
 * 
 * @param path 
 * @return int 
 */
int Connection::fixPath(std::string &path)
{
    size_t found = path.find(_directory);
    if (path == "/" && _fileName.empty() == false)
	{
		_root += "/";
	}
    if (found != std::string::npos)
    {
        path.replace(found, _directory.length(), _root);
    }
    else
    { //MAYBE THIS IS NOT NEEDED. IF THE DIRECTORY IS NOT FOUND, THE PATH WILL BE THE ROOT?
        std::cout << RED "Path not found: 404!" RESET << std::endl;
        _statusCode = 404;
		return (404);
    }
    return (0);
}

/**
 * @brief This function checks if the method sent by the client is 
 * allowed in the serverblock.
 * @param request 
 * @return true 
 * @return false 
 */
bool Connection::methodCheck(RequestParser &request)
{
    std::string method = request.get()["REQUEST_METHOD"];
    return (true);
    /*
    if (_location.allowedMethods[method] == true)
    {
        std::cout << MAGENTA "Method" << method << "allowed!, continue" << std::endl;
                
    } else {
        std::cout << RED "Method " << method << "not allowed 405!" << std::endl;
    }
    */
}

/**
 * @brief This is part of parsing the request. 
 * It checks if the uri is a directory or a file.
 * and sets the path of the file based on the root of the location.
 * @param request 
 * @return int 
 */
int Connection::uriCheck(RequestParser &request)
{
    std::string uri = request.get()["REQUEST_URI"];
    std::string::size_type pos;

    std::stringstream   iss(uri);
    std::getline(iss, _path, '?');
    std::getline(iss, _queryString, '?');
	
    std::cout << BLUE "_Path: " RESET << _path << std::endl;
    std::cout << BLUE "Query: " RESET << _queryString << std::endl;

    pos = _path.rfind('/');
    _directory = _path.substr(0, pos);
    _fileName = pos == std::string::npos ? _path : _path.substr(pos + 1);
    //if the _filename does not have an extension, it is a directory. recomplete the directory path
    if (_fileName.find('.') == std::string::npos)
    {
        _directory = _path;
        _fileName = "";
    }
	if (_directory == "")
		_directory = "/";
    std::cout << BLUE "Directory: " RESET << _directory << std::endl;
    //get just the first part of the _directory
    pos = _directory.find('/', 1);
    _finalPath = _directory;
    _directory = _directory.substr(0, pos);
    std::cout << BLUE "Location is: " RESET << _directory << std::endl;
    std::cout << BLUE "_Filename: " RESET << _fileName << std::endl;
    if (dirCheck(_directory) == true)
        std::cout << GREEN "Directory found: 200!" RESET << std::endl;
    else
    {
        std::cout << RED "Directory not found: 404!" RESET << std::endl;
        return (404);
    }
    //print the root of the location
    std::cout << BLUE "Root: " RESET << _root << std::endl;
    if (fixPath(_finalPath) == 404 || fixPath(_path) == 404)
		return (404);
    std::cout << BLUE "Path fixed: " RESET << _finalPath << std::endl;
	std::cout << BLUE "full Path fixed: " RESET << _path << std::endl;
	if (fileCheck(_path) == false)
		return (404);
    //fix the path with the root of the location.
    return (0);
}

bool Connection::endsWith(const std::string& str, const std::string& ending) {
    if (ending.size() > str.size()) {
        return false;
    }
    return std::equal(ending.rbegin(), ending.rend(), str.rbegin());
}

std::string Connection::getMimeType(const std::string &path)
{
	if (endsWith(path, ".html")) {
        return "text/html";
    } else if (endsWith(path, ".css")) {
        return "text/css";
    } else if (endsWith(path, ".jpg") || endsWith(path, ".jpeg")) {
        return "image/jpeg";
    } else if (endsWith(path, ".png")) {
        return "image/png";
    } else {
        // Default to application/octet-stream for unknown types
        return "application/octet-stream";
    }
}

int Connection::servePage(const std::string &path)
{
    std::ifstream file(path);
    std::stringstream bufferr;
    bufferr << file.rdbuf();
    std::string file_contents = bufferr.str();

    std::cout << "Status code: " << _statusCode << std::endl;
    //NEED TO IMPROVE FOR OTHER STATUS CODES. FIX THIS.
    if (_statusCode == 200)
    {
        std::cout << GREEN "Serving page" RESET << std::endl;
        std::string mime_type = getMimeType(path);
        std::string response_headers = "HTTP/1.1 200 OK\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(file_contents.size()) + "\r\n\r\n";
        send(_clientSocket, response_headers.c_str(), response_headers.size(), 0);
        send(_clientSocket, file_contents.c_str(), file_contents.size(), 0);

        return (0);
    }
    std::cout << RED "Error in servePage" RESET << std::endl;
    const char *response_headers = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n";
    send(_clientSocket, response_headers, strlen(response_headers), 0);
    send(_clientSocket, file_contents.c_str(), file_contents.size(), 0);
    return (0);
}