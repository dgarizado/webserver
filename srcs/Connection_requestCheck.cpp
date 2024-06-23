/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection_requestCheck.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 10:19:40 by vcereced          #+#    #+#             */
/*   Updated: 2024/06/23 16:47:57 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Connection.hpp"

t_location Connection::getLocationVHost(Connection *ref, std::string uriRequested)
{
    std::vector<t_location> locations;
    std::string             locationName;
    t_location              empty;
    
    locations = this->getServerStruct().locations;
    locationName = extractLocationUriStr(uriRequested);

    for (std::vector<t_location>::iterator it = locations.begin(); it != locations.end(); it++)
    {
        if (it->location == locationName)
        {
            this->_statusCode = OK;
            return *it;
        }
    }
    
    empty.location = "NULL";
    return empty;
}

std::string catEndSlashStr(std::string str)
{
    if (!str.empty() && str.back() != '/')
        str += "/";
    return str;
}

std::string locationSwapedRoot(Connection *ref, std::string uriRequested)
{
    std::string location;
    std::string root;

    location = extractLocationUriStr(uriRequested);
    location = catEndSlashStr(location);

    root = ref->getLocation().root;
    if (root.empty())
        return "";
    root = catEndSlashStr(root);
        
    uriRequested.replace(0, location.length(), root);
   
    return (uriRequested);
}

std::string cleanPathQuery(std::string path, std::string query)
{
    if (query.empty() == false)
        path.replace(path.find(query) - 1, query.length() + 1, "");// start -1 because Query dont have the '?', length + 1 to count the '?'
    
    else if (endsWith(path, "?"))
        path.pop_back();
        
    return path;
}

void Connection::requestParse(RequestParser &request)
{
    std::string pathSwapedWithQuery;
    std::string uriRequested;
    
    uriRequested = request.get()["REQUEST_URI"];
    
    this->_location = this->getLocationVHost(this, uriRequested);

    if (this->_location.location == "NULL")
        throw ServerException("requestParse: location requested wrong: " + uriRequested, NOT_FOUND);
        
    try {         
        this->_queryString  = extractQueryStr(uriRequested);
        
        this->_path         = cleanPathQuery(uriRequested, this->_queryString);
        
        this->_pathInfo     = extractPathInfo(this->_path);
        
        this->_path         = cleanPathInfo(this->_path, this->_pathInfo);
        
        this->_fileName     = extractFileNameStr(this->_path);
        
        this->_format       = extractExtension(this->_fileName);
        
        this->_path         = locationSwapedRoot(this,this->_path);
        
        showParamsConsole(uriRequested, this->_path, this->_fileName, this->_pathInfo, this->_queryString, this->_format);   
    } catch(const ServerException &e) {
        throw ServerException("requestParse: " + std::string(e.what()), INTERNAL_SERVER_ERROR);
    }
}
    //std::cout << "querystring " << this->_queryString << std::endl;
    //std::cout << "path " <<this->_path << std::endl;
    //std::cout << "pathinfo " <<this->_pathInfo<< std::endl;
    //std::cout << "filename " <<this->_fileName<< std::endl;
    //std::cout << "path " <<this->_path<< std::endl;
    //std::cout << "format " <<this->_format<< std::endl;
    //std::cout << "path " <<this->_path<< std::endl;
    //    getchar();
