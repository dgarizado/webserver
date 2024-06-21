/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection_delete.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 23:38:25 by vcereced          #+#    #+#             */
/*   Updated: 2024/06/22 00:22:24 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Connection.hpp"
#include <iostream>
#include <filesystem>
#include <cstdio>
#include <iostream>

static void openFile(std::string filePath)
{
    if (access(filePath.c_str(), F_OK) == -1)
        throw ServerException("openFile: not found: " + filePath, 404);
    if (access(filePath.c_str(), R_OK) == -1)
        throw ServerException("openFile: Permision not allowed: " + filePath, 403);
    
    std::ifstream       file(filePath);
    if (!file)
        throw ServerException("openFile: internal error: " + filePath, 500);
    
    
}



std::string Connection::genResponseDELETE(RequestParser &request)
{
    std::string responseHTTP_header;
    std::string responseHTTP_body;

    if (this->_fileName.empty() == true)
        throw ServerException("genResponseDELETE: no filename to delete ", 400);
    if (this->_fileName.empty() == false)
    {
        try{
            openFile(this->_path);
        } catch (const ServerException &e) {
            throw ServerException("genResponseDELETE: " + std::string(e.what()), e.getCode());
        }
        if (std::remove(this->_path.c_str()) == 0)
        {
            this->_statusCode = 204;
            responseHTTP_body = this->_fileName + "deleted succesfully, try rm -rf /* ja";
            responseHTTP_header = genHeaderHTTP(responseHTTP_body, this->_path);
            return responseHTTP_header + responseHTTP_body;
        } else {
            throw ServerException("genResponseDELETE: internal error: " + this->_path, 500);
        }
        
    }
}