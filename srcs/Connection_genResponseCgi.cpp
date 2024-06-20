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

void Connection::setVarsEnviroment(RequestParser &request)//mixing objects its not good but works 
{
    const char *value;
    if (request.get()["REQUEST_METHOD"] == "GET") //mixing objects its not good but works 
    {
        value = this->_queryString.c_str();
        if (setenv("QUERY_STRING", value, 1) != 0)
            throw std::runtime_error("setVarsEnviroment: QUERY_STRING error");
        
        value = request.get()["REQUEST_METHOD"].c_str();
        setenv("REQUEST_METHOD", value, 1);
        if (setenv("REQUEST_METHOD", value, 1) != 0)
            throw std::runtime_error("setVarsEnviroment: REQUEST_METHOD error");
    }
}

std::string Connection::genBodyCgi(std::string filePath, RequestParser &request)
{
    std::string response_body;
    long        size;
    
    try{
        setVarsEnviroment(request);
        response_body = readOutputCgi(filePath);
    }catch(std::exception &e) {
        _statusCode = 500;
        throw std::runtime_error("genBodyCgi: " + std::string(e.what()));
    }

    return response_body;
}