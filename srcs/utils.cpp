/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 16:41:58 by dgarizad          #+#    #+#             */
/*   Updated: 2024/07/11 10:43:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

int ft_error(std::string msg)
{
	std::cerr << YELLOW << msg << RESET << std::endl;
	return (-1);
}

void setNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        std::cerr << "Error getting socket flags\n";
        exit(EXIT_FAILURE);
    }
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
        std::cerr << "Error setting non-blocking\n";
        exit(EXIT_FAILURE);
    }
}

void    openFile(std::string filePath, std::ifstream& file)
{
    if (access(filePath.c_str(), F_OK) == -1)
        throw ServerException("openFile: not found: " + filePath, NOT_FOUND);
    if (access(filePath.c_str(), R_OK) == -1)
        throw ServerException("openFile: Permision not allowed: " + filePath, FORBIDDEN);
    
    file.open(filePath.c_str());
    if (!file)
        throw ServerException("openFile: internal error: " + filePath, INTERNAL_SERVER_ERROR);
    
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

char** convertToCharArray(const std::vector<std::string>& strList) {

    char** charArray = new char*[strList.size() + 1];
    
    for (size_t i = 0; i < strList.size(); ++i) {
        charArray[i] = new char[strList[i].size() + 1];
        std::strcpy(charArray[i], strList[i].c_str());
    }
    
    charArray[strList.size()] = NULL;
    
    return charArray;
}

std::string extractPathInfo(std::string uri)
{
    std::string pathInfo;
    size_t      slashPos;
    size_t      dotPos;
    size_t      lenght;

    lenght = uri.length();
    dotPos = uri.find('.', 0);
    slashPos = uri.find('/', dotPos);
    
    if (dotPos != std::string::npos && slashPos != std::string::npos)
        pathInfo = uri.substr(slashPos + 1 , lenght - slashPos);
    else
        pathInfo = "";
    return pathInfo;
}


std::string extractFileNameStr(std::string uri)
{
	std::string fileName;
    size_t      slashPos;
    // size_t      length;
    size_t      dotPos;

    // length = uri.length();
    slashPos = uri.rfind('/');
    dotPos = uri.find('.');

    if (dotPos != std::string::npos)
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

std::string cleanPathInfo(std::string path, std::string query)
{
    if (query.empty() == false)
    {
        path.replace(path.find(query) - 1, query.length() + 1, "");// start -1 because Query dont have the '/', length + 1 to count the '/'
    }
    return path;
}

std::string extractExtension(std::string fileName)
{
    size_t dosPos;
    
    dosPos = fileName.rfind('.');
    if (dosPos != std::string::npos)
        return fileName.substr(dosPos);
    else
        return "";
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

void showParamsConsole(std::string &uriRequested, std::string &path, std::string &fileName, std::string &pathInfo, std::string &queryString, std::string &format)
{
    const int colWidth = 20;
    
    std::cout   << MAGENTA << std::left     // print headers of columns
                << std::setw(colWidth) << "Uri Requested"
                << std::setw(colWidth) << "_Path"
                << std::setw(colWidth) << "_FileName" <<"\n";


    std::cout   << std::setw(colWidth) << uriRequested        // print values
                << std::setw(colWidth) << path
                << std::setw(colWidth) << fileName << std::endl;

    std::cout   << std::string(colWidth * 2, '-') << "\n";  // print separator

    std::cout   << std::left     // print headers of columns
                << std::setw(colWidth) << "_format"
                << std::setw(colWidth) << "_PathInfo"
                << std::setw(colWidth) << "_queryString" << "\n";

    std::cout   << std::left  
                << std::setw(colWidth) << format
                << std::setw(colWidth) << pathInfo
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
