/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 20:12:40 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/26 20:50:57 by vcereced         ###   ########.fr       */
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
