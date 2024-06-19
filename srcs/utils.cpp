/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 16:41:58 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/19 18:34:15 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

int ft_error(std::string msg)
{
	std::cerr << YELLOW << msg << RESET << std::endl;
	return (-1);
}

bool endsWith(const std::string& str, const std::string& ending) {
    if (ending.size() > str.size()) {
        return false;
    }
    return std::equal(ending.rbegin(), ending.rend(), str.rbegin());
}

std::string extractFileNameStr(std::string uri)
{
	std::string fileName;
    size_t      slashPos;
    size_t      questionMarkPos;
    size_t      dotPos;

    slashPos = uri.rfind('/');
    questionMarkPos = uri.find('?');
    dotPos = uri.find('.', slashPos);

    if (dotPos != std::string::npos && questionMarkPos != std::string::npos)
        fileName = uri.substr(slashPos + 1, questionMarkPos - slashPos - 1);
    else if (dotPos != std::string::npos)
        fileName = uri.substr(slashPos + 1);
    return fileName;
}

std::string extractQueryStr(std::string uri)
{
	std::string queryString;
    size_t      questionMarkPos;

    questionMarkPos = uri.find('?');

    if (questionMarkPos != std::string::npos)
        queryString = uri.substr(questionMarkPos + 1);
    else
        queryString = "";
    return queryString;
}

std::string extractLocationUriStr(std::string uri)
{
    std::string prefix;
    size_t slashPos;
    size_t dotPos;
    
    slashPos = uri.find('/', 1);         //Find the position of "/" after first position like "/folder/file.format"
    dotPos = uri.find('.', 1);
                              
    if (slashPos != std::string::npos)                   // if encounter "/" then extract
        prefix = uri.substr(0, slashPos);
    else if (dotPos != std::string::npos) //if encounter ".format" then means its / location
        prefix = "/";  
    else
        prefix = uri;               // En caso de que no se encuentre otra barra y un ".format", asumir que todo es el prefijo
 
    return prefix;

}