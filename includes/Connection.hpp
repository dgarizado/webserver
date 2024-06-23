/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 20:08:17 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/23 16:59:09 by dgarizad         ###   ########.fr       */
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

        void readFromSocket();

        //GETTERS
        int         getClientSocket() const;
        std::string getBuffer() const;
		std::string getPath() const;
		int         getStatusCode() const;
        t_location  getLocation() const;
        std::string getFileName() const;
        std::string getValidDefaultIndex(void);
        t_location  getLocationVHost(Connection *ref, std::string uriRequested);
        bool        getKeepAlive(void) const;
        RequestParser& getRequest(void);

        //SETTERS
        void        setClientData(int clientSocket, sockaddr_in clientAddr, socklen_t clientAddrSize, struct epoll_event ev);
        void        setBuffer(std::string buffer);
        void        setStatusCode(int statusCode);


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
        std::string genResponseGET(RequestParser &request);
        std::string genResponsePOST(RequestParser &request);
        void        setVarsEnviroment(RequestParser &request);

        //POST
        void        processPost();
        std::string parseBody(std::string &body);


        void processRequest(RequestParser &request);


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
		int                         _statusCode;
        bool                        _keepAlive;
        RequestParser               _requestConnection;

        //POST
        std::string                 _headerPost;
        std::string                 _body;
        std::string                 _boundary;
        std::string                 _postFileName;
        std::vector<unsigned char>           _buffer2;
        std::vector<unsigned char>           _buffer3;
};

#endif