/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 20:08:17 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/08 15:06:42 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP
# include "webserv.hpp"
# include "VHost.hpp"
# include "RequestParser.hpp"
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
		std::string getPath() const;

        //SETTERS
        void setClientData(int clientSocket, sockaddr_in clientAddr, socklen_t clientAddrSize, struct epoll_event ev);
        void setBuffer(std::string buffer);
        void setVhost(VHost vhost);

        //DETERMINATOR
        int fixPath(std::string &path);
        int uriCheck(RequestParser &request);
        bool dirCheck(std::string directory);
		bool fileCheck(std::string file);
        
        
    private:
        int _clientSocket;
        sockaddr_in _clientAddr;
        socklen_t _clientAddrSize;
        struct epoll_event _ev;
        std::string _buffer; //maybe a char array is better
        char _readBuffer[1024];
        std::string _directory; //this is the location sent by the client.
        std::string _finalPath; //this is the path with the root without the file name..
		std::string _path; //this is the full path, including the root and the file name.
        std::string _fileName; //this is the file name requested by the client. empty if the client requested a directory.
        std::string _queryString;
        std::string _root; //this is the root of the vhost.
        VHost _vhost;
};

#endif