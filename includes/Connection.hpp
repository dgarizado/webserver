/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 20:08:17 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/16 13:31:22 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP
# include "webserv.hpp"
# include "VHost.hpp"
# include "RequestParser.hpp"
# include <sys/stat.h>
# include <exception>
# include <ctime>
# include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

class VHost;

class Connection
{ 
    public:
        Connection();
        ~Connection();
        Connection(Connection const &src);
        Connection &operator=(Connection const &src);

        //GETTERS
        int         getClientSocket() const;
        std::string getBuffer() const;
		std::string getPath() const;
		int         getStatusCode() const;
        bool        getAutoIndex() const;
        std::string getFileName() const;
        std::string getFinalPath() const;

        //SETTERS
        void        setClientData(int clientSocket, sockaddr_in clientAddr, socklen_t clientAddrSize, struct epoll_event ev);
        void        setBuffer(std::string buffer);
        void        setVhost(VHost vhost);

        //DETERMINATOR
        int         fixPath(std::string &path);
        int         uriCheck(RequestParser &request);
        bool        dirCheck(std::string directory);
		bool        fileCheck(std::string file);
        bool        methodCheck(RequestParser &request);
        int         requestCheck(RequestParser &request);
        bool        setIndex();
        int         setDefaultIndex(void);

        //SERVE PAGE
        bool        endsWith(const std::string &str, const std::string &ending);
        std::string getMimeType(const std::string &path);
        int         servePage(const std::string &path);
        void        serveErrorPage(void);  
        std::string genResponsePage(std::string path);

    
        std::string genAutoIndex(std::string route);
        std::string genRelativeRoute(std::string route);
        
    private:
        int                         _clientSocket;
        sockaddr_in                 _clientAddr;
        socklen_t                   _clientAddrSize;
        struct epoll_event          _ev;
        std::string                 _buffer; //maybe a char array is better
		std::string                 _requestedPath; //this is the full path, including the root and the file name.
        std::string                 _queryString;
        std::string                 _requestedPathNoFile; //this is the location sent by the client.
        std::string                 _requestedLocation;
        t_location                  _location;
        std::string                 _finalPath; //this is the path with the root without the file name..
        std::string                 _fileName; //this is the file name requested by the client. empty if the client requested a directory.
        std::string                 _root; //this is the root of the vhost.
		int                         _statusCode ;
        VHost _vhost;
};

#endif