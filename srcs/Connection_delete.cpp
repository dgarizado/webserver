/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection_delete.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 23:38:25 by vcereced          #+#    #+#             */
/*   Updated: 2024/06/23 18:43:20 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Connection.hpp"
#include <iostream>
#include <filesystem>
#include <cstdio>



std::string responseDELETE(Connection *ref, RequestParser &request)
{
    std::string responseHTTP_header;
    std::string responseHTTP_body;
    
    if (std::remove(ref->getPath().c_str()) == 0)
    {
        ref->setStatusCode(NO_CONTENT);
        
        responseHTTP_body = ref->getFileName() + "deleted succesfully, try rm -rf /* ja";
        responseHTTP_header = ref->genHeaderHTTP(responseHTTP_body, ref->getPath());
        
        return responseHTTP_header + responseHTTP_body;
    } 
    else
        throw ServerException("genResponseDELETE: internal error: " + ref->getPath(), INTERNAL_SERVER_ERROR);
}


std::string Connection::genResponseDELETE(RequestParser &request)
{
    std::string responseHTTP;

    try{
        
        if (this->_fileName.empty() == true)
            throw ServerException("no filename to delete ", BAD_REQUEST);
            
        openFile(this->getPath());
        
        if (this->_fileName.empty() == false)
            responseHTTP = responseDELETE(this, request);
            
    }catch (const ServerException &e) {
        throw ServerException("genResponseDELETE: " + std::string(e.what()), e.getCode());
    }
}