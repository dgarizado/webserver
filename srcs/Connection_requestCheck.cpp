/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection_requestCheck.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 10:19:40 by vcereced          #+#    #+#             */
/*   Updated: 2024/06/20 11:37:25 by vcereced         ###   ########.fr       */
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
            this->_statusCode = 200;
            return *it;
        }
    }
    
    empty.location = "NULL";
    return empty;
}

std::string catEndSlashStr(std::string str)
{
    if (!str.empty() && str.back() != '/')
    {
        str += "/";
    }
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
    {
        path.replace(path.find(query) - 1, query.length() + 1, "");// start -1 because Query dont have the '?', length + 1 to count the '?'
    }
    return path;
}

void Connection::requestParse(RequestParser &request)
{
    std::string pathSwapedWithQuery;
    std::string uriRequested;
    
    uriRequested = request.get()["REQUEST_URI"];
    
    this->_location = this->getLocationVHost(this, uriRequested);

    if (this->_location.location == "NULL")
    {
        this->_statusCode = 404;
        throw std::runtime_error("requestParse: location requested wrong: " + uriRequested);
    }

    pathSwapedWithQuery = locationSwapedRoot(this, uriRequested);
    this->_fileName = extractFileNameStr(uriRequested);
    this->_queryString = extractQueryStr(uriRequested);
    this->_path = cleanPathQuery(pathSwapedWithQuery, this->_queryString);
   
    showParamsConsole(uriRequested, pathSwapedWithQuery, this->_path, this->_fileName, this->_queryString);
}
