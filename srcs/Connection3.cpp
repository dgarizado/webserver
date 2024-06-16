/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection3.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 13:30:21 by vcereced          #+#    #+#             */
/*   Updated: 2024/06/16 21:36:19 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Connection.hpp"
#include <cstdio>
#include <iostream>

int setVarsEnviroment(Connection *ref, RequestParser &request)//mixing objects its not good but works 
{
    const char *value;
    if (request.get()["REQUEST_METHOD"] == "GET") //mixing objects its not good but works 
    {
        value = ref->getQueryString().c_str();
        if (setenv("QUERY_STRING", value, 1) != 0)
        {
            std::cerr << "Error al establecer la variable de entorno QUERY_STRING\n";
            return -1;
        }
        
        value = request.get()["REQUEST_METHOD"].c_str();
        setenv("REQUEST_METHOD", value, 1);
        if (setenv("REQUEST_METHOD", value, 1) != 0)
        {
            std::cerr << "Error al establecer la variable de entorno REQUEST_METHOD\n";
            return -1;
        }
    }
    return 0;   
}

std::string responseCgi(std::string filePath)
{
    FILE* pipe = popen(filePath.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error al abrir el pipe para ejecutar el comando\n";
        return "ERROR";
    }

    // Buffer para almacenar la salida del comando
    char buffer[1024];
    std::string result = "";

    // Leer la salida del comando línea por línea
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    // Cerrar el pipe
    pclose(pipe);

    // Imprimir la salida del comando
    std::cout << "Resultado del comando:\n" << result << "\n";
    return result;
}


std::string Connection::genResponseCgi(std::string filePath, RequestParser &request)
{
    std::cerr << YELLOW << "DEBUG filePath= " << filePath << RESET << std::endl;
    std::string response_header;
    std::string response_body;
    long        size;
    
    if (setVarsEnviroment(this, request) == -1)
    {
        _statusCode = 500;
        this->serveErrorPage();
    }

    response_body = responseCgi(filePath);
    size            = response_body.size();
    response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    
    return response_header + response_body;
    
}


std::string Connection::genResponsePage(std::string filePath)
{
    std::stringstream   buffer;
    std::ifstream       file(filePath);
    std::string         file_content;
    std::string         mime_type;
    std::string         response;
    long                size;
    
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filePath);
        _statusCode = 404;
    }

    buffer << file.rdbuf();
    file_content = buffer.str();
    mime_type = this->getMimeType(filePath);
    size      = file_content.size();

    std::cout << GREEN "Serving page" RESET << std::endl;
    
    if (_statusCode == 200)
        response  = "HTTP/1.1 200 OK\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (_statusCode == 404)
        response  = "HTTP/1.1 404 Not Found\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (_statusCode == 405)
        response  = "HTTP/1.1 Method Not Allowed\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";

    response += file_content;
    std::cerr << std::endl << GREEN << response << RESET << std::endl;
    return response;
}



