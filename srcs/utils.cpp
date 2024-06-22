/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 16:41:58 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/21 22:27:34 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

int ft_error(std::string msg)
{
	std::cerr << YELLOW << msg << RESET << std::endl;
	return (-1);
}

void printWaitConsole(void)
{
    static int  step = 0;
    std::string str;
    
    str =  "waiting connection";
    for (int i = 0; i < step; i++)
        str += ".";
    step++;
    if(step > 3)
        step = 0; 
    

    std::cout << "\033[2K";  // Borra toda la línea
    std::cout << GREEN  << str << "\r"<<  RESET ;
    std::cout.flush(); 
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

void showParamsConsole(std::string &uriRequested, std::string &pathSwapedWithQuery, std::string &path, std::string &fileName, std::string &queryString)
{
    const int colWidth = 20;
    
    std::cout << MAGENTA << std::left     // print headers of columns
                         << std::setw(colWidth) << "Uri Requested"
                         << std::setw(colWidth) << "Path Swaped With Query"
                         << std::setw(colWidth) << "_Path"
                         << std::setw(colWidth) << "_FileName"
                         << std::setw(colWidth) << "_queryString" << "\n";

    std::cout << std::string(colWidth * 5, '-') << "\n";  // print separator

    std::cout << std::setw(colWidth) << uriRequested        // print values
              << std::setw(colWidth) << pathSwapedWithQuery
              << std::setw(colWidth) << path
              << std::setw(colWidth) << fileName
              << std::setw(colWidth) << queryString << RESET << std::endl;
}

void showParamsConsoleHTTP(std::string responseStr, size_t sizeResponse, int clientSocket, int statusCode, bool error)
{
     // Definir el ancho para cada columna
    const int colWidth = 10;

    std::istringstream stream(responseStr);
    std::string line;
    std::getline(stream, line);
    std::istringstream  iss(line);
    std::string         token;
    std::string         code;
    std::string         mime;
    
    iss >> token;
    iss >> token;
    code = token;
    std::getline(stream, line);
    std::istringstream  iss2(line);
    iss2 >> token;
    iss2 >> token;
    mime = token;
    
    if (!error)
        std::cout  <<  "\n               SENDING RESPONSE ------------->" << std::endl;
    else
        std::cout  <<  "\n               SENDING RESPONSE " << RED << "ERROR --------> " << RESET  << std::endl;
        
    std::cout << std::left  // Alinear el texto a la izquierda
              << std::setw(colWidth) << "Code Server"
              << std::setw(colWidth) << "Code Response"
              << std::setw(colWidth) << "Client Socket"
              << std::setw(colWidth) << "Mime type"
              << std::setw(colWidth) << "Size ResponseHTTP" << "\n";

    std::cout << std::string(colWidth * 5, '-') << "\n";
    std::cout << std::setw(colWidth) << statusCode
              << std::setw(colWidth) << code
              << std::setw(colWidth) << clientSocket
              << std::setw(colWidth) << mime
              << std::setw(colWidth) << sizeResponse << "\n\n";
}
