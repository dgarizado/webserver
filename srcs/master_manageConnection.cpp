/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   master_manageConnection.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:35:35 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/21 14:08:35 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Master.hpp"

void Connection::requestCheck(RequestParser &request)
{
    std::string method;
    std::string uriRequested;
    
    uriRequested = request.get()["REQUEST_URI"];
    
    if (this->_path.empty())
	{
        throw ServerException("requestParse: location requested have not root defined: " + uriRequested, 500);
		//this->_statusCode = 404;
        //throw std::runtime_error("requestParse: location requested have not root defined: " + uriRequested);
	}
    
    method = request.get()["REQUEST_METHOD"];
    
    if (this->methodCheck(method) == false)
	{
        throw ServerException("requestParse: location requested method not allowed: " + uriRequested, 405);
		//this->_statusCode = 405;
        //throw std::runtime_error("requestParse: location requested method not allowed: " + method);
        
	}
}

/**
 * @brief get the buffer from the client object and process the request. 
 * 
 * @param clientSocket 
 * @return int 
 */
void Master::processRequest(Connection &connection, RequestParser &request)
{
    std::string response;

    std::cout << MAGENTA "PROCESSING REQUEST..." << RESET << std::endl;
    try{
        connection.requestParse(request);
        connection.requestCheck(request);
        response = connection.genResponse(request);
    }catch(const ServerException &e) {
        throw ServerException("processRequest: " + std::string(e.what()), e.getCode());
    }
    send(connection.getClientSocket(), response.c_str(), response.size(), 0);
    //close(connection.getClientSocket());// IMPORTANT TO DEFINE!! ???????????????????????????????????????????????????? CREO QUE NO: ES AL SALIR DE MANAGE CONNECTION PUES FINALIZO AUNQUE ES LO MISMO HAY QUE AGRUPAR las acciones de LIMPIAR LOS VECTOR FDS

    showParamsConsoleHTTP(response, response.size(), connection.getClientSocket(), connection.getStatusCode(), false);
}


void Connection::readFromSocket(void)
{
    char    buffer[BUFFER];//????????????????????????????????????????????limitante para el upload???????????????
    int     bytesRead;
    int     clientSocket;

    clientSocket = this->getClientSocket();
    bytesRead = read(clientSocket, buffer, 2048);
    
    if (bytesRead < 0)
        throw std::runtime_error("readFromSocket: Error reading from client socket " + std::to_string(clientSocket));  //REMOVE CLIENT SOCKET FROM EPOLL SET AND CLOSE SOCKET!

    if (bytesRead == 0)
        throw std::runtime_error("readFromSocket: 0 bytes readed from client socket " + std::to_string(clientSocket));
    else //store the buffer in the connection object which represent the client
    {
        buffer[bytesRead] = '\0';
        this->setBuffer(buffer);
    } 
}

void Master::manageConnection(Connection &connection)
{
    RequestParser   request;
    VHost           VHostAssigned;
    std::string     buffer;

    try{
        connection.readFromSocket();

        buffer = connection.getBuffer();
        request.loadConfigFromRequest(buffer);

        request.showConfig();

        VHostAssigned = this->assignVHost(request.get()["HTTP_HOST"]);
        
        connection = VHostAssigned;         //post-herencia, clase BASE de Connection con la clase Base asignada
        
        this->processRequest(connection, request);
    }
    catch (const ServerException &e) {
        throw ServerException("manageConnection: " + std::string(e.what()), e.getCode());
    }
    catch (const std::exception &e) {
        throw std::runtime_error("manageConnection: " + std::string(e.what()));
    }

}
