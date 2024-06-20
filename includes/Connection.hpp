/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 20:08:17 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/20 11:39:37 by vcereced         ###   ########.fr       */
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

class Connection : public VHost
{ 
    public:
        Connection();
        ~Connection();
        Connection(Connection const &src);
        Connection &operator=(Connection const &src);
        Connection &operator=(VHost &src);

        //GETTERS
        int         getClientSocket() const;
        std::string getBuffer() const;
		std::string getPath() const;
		int         getStatusCode() const;
        t_location  getLocation() const;
        std::string getFileName() const;
        std::string getValidDefaultIndex(void);
        t_location  getLocationVHost(Connection *ref, std::string uriRequested);

        //SETTERS
        void        setClientData(int clientSocket, sockaddr_in clientAddr, socklen_t clientAddrSize, struct epoll_event ev);
        void        setBuffer(std::string buffer);


        //DETERMINATOR
        bool        methodCheck(std::string method);
        void        requestParse(RequestParser &request);
        void        requestCheck(RequestParser &request);
        int         setDefaultIndex(void);

        //SERVE PAGE
   
        std::string getMimeType(const std::string &path);
        void        serveErrorPage(void);  
        std::string genBodyCgi(std::string filePath, RequestParser &request);
        std::string genBodyFile(std::string filePath);
        std::string genBodyHTTP(std::string filePath, RequestParser &request);
        std::string genBodyAutoIndex(std::string route);
        std::string genHeaderHTTP(std::string bodyHTTP, std::string filePath);
        std::string genResponse(RequestParser &request);
        std::string genPathDefaultIndex(void);


        int setVarsEnviroment(RequestParser &request);

    

        
    private:
        int                         _clientSocket;
        sockaddr_in                 _clientAddr;
        socklen_t                   _clientAddrSize;
        struct epoll_event          _ev;
        std::string                 _buffer; //maybe a char array is better

        t_location                  _location;
        std::string                 _queryString;
        std::string                 _fileName;
        std::string                 _path;
		int                         _statusCode ;

};

#endif