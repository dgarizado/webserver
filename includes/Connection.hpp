/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 20:08:17 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/26 20:55:16 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP
# include "webserv.hpp"
# include "VHost.hpp"
class VHost;

class Connection
{ 
    public:
        Connection();
        ~Connection();
        Connection(Connection const &src);
        Connection &operator=(Connection const &src);

        //GETTERS
        int getClientSocket() const;
        std::string getBuffer() const;

        //SETTERS
        void setClientData(int clientSocket, sockaddr_in clientAddr, socklen_t clientAddrSize, struct epoll_event ev);
        void setBuffer(std::string buffer);
        void setVhost(VHost vhost);

        //DETERMINATOR
        
    private:
        int _clientSocket;
        sockaddr_in _clientAddr;
        char _readBuffer[1024];
        socklen_t _clientAddrSize;
        struct epoll_event _ev;
        std::string _buffer; //maybe a char array is better
        //
        //METHOD TO PARSE THE REQUEST
        //
        VHost _vhost;
};

#endif