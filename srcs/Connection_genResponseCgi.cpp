// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   Connection_genResponseCgi.cpp                      :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/05/13 14:44:26 by dgarizad          #+#    #+#             */
// /*   Updated: 2024/06/20 09:49:07 by vcereced         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

 #include "../includes/Connection.hpp"

int Connection::setVarsEnviroment(RequestParser &request)//mixing objects its not good but works 
{
    const char *value;
    if (request.get()["REQUEST_METHOD"] == "GET") //mixing objects its not good but works 
    {
        value = this->_queryString.c_str();
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

std::string Connection::genBodyCgi(std::string filePath, RequestParser &request)
{
    std::cerr << YELLOW << "DEBUG filePath= " << filePath << RESET << std::endl;
  //  std::string response_header;
    std::string response_body;
    long        size;
    
    if (setVarsEnviroment(request) == -1)
    {
        _statusCode = 500;
        this->serveErrorPage();
    }

    response_body = readOutputCgi(filePath);
  //  size            = response_body.size();
   // response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    
    return response_body;
    
}