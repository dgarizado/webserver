/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   master_manageConnection.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 19:35:35 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/20 12:06:14 by vcereced         ###   ########.fr       */
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
		this->_statusCode = 404;
        throw std::runtime_error("requestParse: location requested have not root defined: " + uriRequested);
	}
    
    method = request.get()["REQUEST_METHOD"];
    
    if (this->methodCheck(method) == false)
	{
		this->_statusCode = 405;
        throw std::runtime_error("requestParse: location requested method not allowed: " + method);
	}
}

void showParamsConsoleHTTP(std::string responseStr, size_t sizeResponse, int clientSocket, int statusCode)
{
     // Definir el ancho para cada columna
    const int colWidth = 20;

    // Imprimir los encabezados de las columnas
    std::cout << std::left  // Alinear el texto a la izquierda
              << std::setw(colWidth) << "Status Code"
              << std::setw(colWidth) << "Client Socket"
              << std::setw(colWidth) << "Size ResponseHTTP" << "\n";

    // Imprimir una línea separadora
    std::cout << std::string(colWidth * 3, '-') << "\n";

    // Imprimir los valores
    std::cout << std::setw(colWidth) << statusCode
              << std::setw(colWidth) << clientSocket
              << std::setw(colWidth) << sizeResponse << "\n";

    // Imprimir el mensaje de respuesta HTTP
    std::cout << "\nResponse Message:\n" << responseStr << "\n";

    // Imprimir información adicional de la conexión
    std::cout << "\nConnection Information:\n" << "Client Socket: " << clientSocket << "\n";
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
    }catch(std::exception &e) {
        throw std::runtime_error("processRequest: " + std::string(e.what()));
    }
    
    send(connection.getClientSocket(), response.c_str(), response.size(), 0);
    //close(connection.getClientSocket());// IMPORTANT TO DEFINE!! ????????????????????????????????????????????????????
    std::cout << GREEN <<  "MSG HTTP SENT TO--> " << connection.getClientSocket() << " client Sock\n----------------------------\n" RESET << std::endl;
    showParamsConsoleHTTP(response, response.size(), connection.getClientSocket(), connection.getStatusCode());
}

void Master::manageConnection(Connection &connection)
{
    RequestParser   request;
    VHost           VHostAssigned;
    std::string     buffer;

    try{
        
        this->clientRead(connection.getClientSocket()); //read from the socket
        
        buffer = connection.getBuffer();
        request.loadConfigFromRequest(buffer);

        request.showConfig();

        VHostAssigned = this->assignVHost(request.get()["HTTP_HOST"]);
        
        connection = VHostAssigned;         //post-herencia, clase BASE de Connection con la clase Base asignada
        
        this->processRequest(connection, request);
        
    }catch (std::exception &e) {
        connection.serveErrorPage();
        throw std::runtime_error("manageConnection: " + std::string(e.what()));
    }
}