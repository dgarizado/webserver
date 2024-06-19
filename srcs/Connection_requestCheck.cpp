/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection_requestCheck.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 10:19:40 by vcereced          #+#    #+#             */
/*   Updated: 2024/06/19 17:06:27 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Connection.hpp"


t_location getLocationVHost(Connection *ref, std::string uriRequested)
{
    std::vector<t_location> locations;
    std::string             locationName;
    t_location              empty;
    
    locations = ref->getServerStruct().locations;
    locationName = extractLocationUriStr(uriRequested);

    for (std::vector<t_location>::iterator it = locations.begin(); it != locations.end(); it++)
    {
        if (it->location == locationName)
            return *it;
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
    
    std::cout << "SWAPPING  uri SWAPPED= " << uriRequested << "  root= " << root << "  prefix= "<< location <<  "  uri: " << uriRequested <<   std::endl;   
    return (uriRequested);
}

// void Connection::setFlags(void)
// {
//     if (this->getLocation()->autoIndex == true)
//         this->_autoIndex = true;
//     else
//         this->_autoIndex = false;
// }

void Connection::requestParse(RequestParser &request)
{
    std::string uriRequested;
    std::string method;
    
    uriRequested = request.get()["REQUEST_URI"];
    
    this->_location = getLocationVHost(this, uriRequested);

    if (this->_location.location == "NULL" && (_statusCode = 404))
    {
		this->serveErrorPage();
        throw std::runtime_error("location requested wrong: " + uriRequested);
    }

    this->_path = locationSwapedRoot(this, uriRequested);
    
	if (this->_path.empty() && (_statusCode = 404))
	{
		this->serveErrorPage();
        throw std::runtime_error("location requested have not root defined: " + uriRequested);
	}
    
    method = request.get()["REQUEST_METHOD"];
    
    if (this->methodCheck(method) == false && (_statusCode =  405))
	{
		this->serveErrorPage();
        throw std::runtime_error("location requested method not allowed: " + method);
	}
    
    this->_fileName = extractFileNameStr(uriRequested);
    this->_queryString = extractQueryStr(uriRequested);
   
    //this->setFlags();
}