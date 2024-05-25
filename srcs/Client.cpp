/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 20:12:40 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/25 22:13:32 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client() : _clientSocket(0), _clientAddrSize(sizeof(_clientAddr))
{
}   

Client::~Client()
{
}

Client::Client(Client const &src)
{
    *this = src;
}

Client &Client::operator=(Client const &src)
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

int Client::getClientSocket() const
{
    return _clientSocket;
}

std::string Client::getBuffer() const
{
    return _buffer;
}


void Client::setClientData(int clientSocket, sockaddr_in clientAddr, socklen_t clientAddrSize, struct epoll_event ev)
{
    _clientSocket = clientSocket;
    _clientAddr = clientAddr;
    _clientAddrSize = clientAddrSize;
    _ev = ev;
}

void Client::setBuffer(std::string buffer)
{
    _buffer = buffer;
}

