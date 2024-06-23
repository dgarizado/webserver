/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection_genResponse.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 13:30:21 by vcereced          #+#    #+#             */
/*   Updated: 2024/06/23 18:03:37 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Connection.hpp"

std::string Connection::genHeaderHTTP(std::string bodyHTTP, std::string filePath)
{
    std::stringstream   buffer;
    std::string         mime_type;
    std::string         response_header;
    long                size;
    long                statusCode;

    mime_type = this->getMimeType(filePath);//para python ???????????????????
    statusCode = this->getStatusCode();
    size = bodyHTTP.size();

    if (statusCode == 200)
        response_header  = "HTTP/1.1 200 OK\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (statusCode == 400)
        response_header  = "HTTP/1.1 400 Bad Request\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (statusCode == 403)
        response_header  = "HTTP/1.1 403 Forbidden\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (statusCode == 404)
        response_header  = "HTTP/1.1 404 Not Found<\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (statusCode == 405)
        response_header  = "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";
    else if (statusCode == 500)
        response_header  = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: " + mime_type + "\r\nContent-Length: " + std::to_string(size) + "\r\n\r\n";

    return response_header;
}

std::string Connection::getValidDefaultIndex(void)
{
    std::string path = this->getPath();

    std::vector<std::string>::iterator it = this->_location.index.begin();
    for (; it != this->_location.index.end(); it++)
    {
        path = this->getPath() + *it;
        if (access(path.c_str(), F_OK)  != -1)
        {
            this->_statusCode = 200;
            return *it;
        }
    }
    //this->_statusCode = 404;
    throw ServerException("getValidDefaultIndex: not any index found ", 404);
   // throw std::runtime_error("getValidDefaultIndex: not any index found");
    return "";
}

std::string Connection::genPathDefaultIndex(void)
{
    std::string                 responseHTTP;
    std::string                 defaultPath;

    defaultPath = this->getPath();

    defaultPath += getValidDefaultIndex();

    return defaultPath;
}

std::ifstream openFile(std::string filePath)
{
    if (access(filePath.c_str(), F_OK) == -1)
        throw ServerException("openFile: not found: " + filePath, 404);
    if (access(filePath.c_str(), R_OK) == -1)
        throw ServerException("openFile: Permision not allowed: " + filePath, 403);

    std::ifstream       file(filePath);
    if (!file)
        throw ServerException("openFile: internal error: " + filePath, 500);

    return file;
}


std::string Connection::genBodyFile(std::string filePath)
{
    std::string         responseHTTP_body;
    std::ifstream       file;
    std::stringstream   buffer;

    try{
       file = openFile(filePath);
    } catch (const ServerException &e) {
        throw ServerException("genBodyFile: " + std::string(e.what()), e.getCode());
    }
    // if (!file){
    //    // this->_statusCode = 404;
    //   //  throw std::runtime_error("genBodyFile: Cannot open file:" + filePath);
    //     throw ServerException("genBodyFile: " + std::string(strerror(errno) + filePath), 404);
    // }

    buffer << file.rdbuf();
    responseHTTP_body = buffer.str();

    this->_statusCode = 200;

    return responseHTTP_body;
}

std::string Connection::genBodyHTTP(std::string filePath, RequestParser &request)
{
    std::string         responseHTTP_body;

    if (endsWith(filePath, "py"))
        responseHTTP_body = genBodyCgi(filePath, request);
    else
        responseHTTP_body = genBodyFile(filePath);

    return responseHTTP_body;
}

std::string Connection::genResponseGET(RequestParser &request)
{
    std::vector<std::string>    defaultIndexs;
    std::string                 responseHTTP_header;
    std::string                 responseHTTP_body;
    bool                        autoIndex;

    defaultIndexs = this->getLocation().index;
    autoIndex = this->getLocation().autoIndex;

    if (this->_fileName.empty() == false)
    {
        responseHTTP_body = genBodyHTTP(this->_path, request);
    }
    else if (defaultIndexs.empty() == false)
    {
        this->_path = genPathDefaultIndex();
        responseHTTP_body = genBodyHTTP(this->_path, request);
    }
    else if (autoIndex == true)
    {
       responseHTTP_body = genBodyAutoIndex(this->_path);
    }
    else
        throw ServerException("genResponse: not default index and autoindex defined ", 404);

    responseHTTP_header = genHeaderHTTP(responseHTTP_body, this->_path);

    return responseHTTP_header + responseHTTP_body;
}

std::string Connection::genResponsePOST(RequestParser &request)
{
    return "POST";
}


/**
 * @brief This function will parse the body of a POST request
 * it will extract the boundary and the headers in the body
 * @param body 
 * @return std::string 
 */
std::string Connection::parseBody(std::string &body)
{
    std::string     headers;
    std::string     bodyContent;
    bool            contentStart = false;

    _boundary = _requestConnection.getBoundary();
    _postFileName = _requestConnection.getPostFileName();
    std::cout << BBLUE "PARSING body: "  RESET<< body << std::endl;
    std::istringstream iss(body);
    std::string line;
    while (std::getline(iss, line))
    {
        line += "\n";
        if (contentStart == true)
        {
            bodyContent += line;
        }
        if (line == "\r\n")
        {
            contentStart = true;
        }
        if (contentStart == false)
            headers += line;
    }
    std::cout << BBLUE "HEADERS PARSED FROM BODY: "  RESET<< headers << std::endl;
    std::cout << BRED "SIZE OF HEADERS: "  RESET<< headers.size() << std::endl;
    std::cout << BYELLOW "BOUNDARY IS:" RESET << _boundary << std::endl;
    std::cout << BYELLOW "BOUNDARY end should be: " RESET << _boundary + "--" << std::endl;
    
    std::cout << BYELLOW "FILENAME IS:" RESET << _postFileName << std::endl;
    std::cout << BBLUE "BODY PARSED FROM BODY: "  RESET<< bodyContent << std::endl;
    std::cout << BRED "SIZE OF BODY: "  RESET<< body.size() << std::endl;
    std::cout << BRED "SIZE OF BUFFER2: " RESET << _buffer2.size() << std::endl;
    //extract first header.size bytes from _buffer2 as chars for printing
    std::string headerContent(_buffer2.begin(), _buffer2.begin() + headers.size());
    std::cout << BBLUE "HEADERS FROM BUFFER2: "  RESET<< headerContent << std::endl;
    //extract the body from the buffer2 as binary data for not damaging the data
    std::vector<unsigned char> bodyContent2(_buffer2.begin() + headers.size(), _buffer2.end());
    std::cout << BYELLOW "BODY FROM BUFFER2: "  RESET<< bodyContent2.data() << std::endl;
    std::cout << BRED "SIZE OF BODY FROM BUFFER2: "  RESET<< bodyContent2.size() << std::endl;
    //get last line of the bodycontent2
    return bodyContent;
}

//TESTES
//Function to search for the boundary within the buffer
// size_t findBoundary(const std::vector<unsigned char>& buffer, const std::vector<unsigned char>& boundary, size_t start) {
//     auto it = std::search(buffer.begin() + start, buffer.end(), boundary.begin(), boundary.end());
//     return (it == buffer.end()) ? buffer.size() : std::distance(buffer.begin(), it);
// }

size_t findBoundary(const std::vector<unsigned char>& buffer, const std::vector<unsigned char>& boundary, size_t start) {
    size_t bufferSize = buffer.size();
    size_t boundarySize = boundary.size();
    
    for (size_t i = start; i <= bufferSize - boundarySize; ++i) {
        bool found = true;
        for (size_t j = 0; j < boundarySize; ++j) {
            if (buffer[i + j] != boundary[j]) {
                found = false;
                break;
            }
        }
        if (found) {
            return i;
        }
    }
    return bufferSize; // Return the size of the buffer if the boundary is not found
}

// Function to extract a subvector from the buffer
std::vector<unsigned char> extractSubVector(const std::vector<unsigned char>& buffer, size_t start, size_t end) {
    if (start >= buffer.size() || end > buffer.size() || start >= end) 
    return {};
    return std::vector<unsigned char>(buffer.begin() + start, buffer.begin() + end);
}

// Function to find the end of headers (double CRLF)
// size_t findHeadersEnd(const std::vector<unsigned char>& buffer, size_t start) {
//     const std::string doubleCRLF = "\r\n\r\n";
//     auto it = std::search(buffer.begin() + start, buffer.end(), doubleCRLF.begin(), doubleCRLF.end());
//     return (it == buffer.end()) ? buffer.size() : std::distance(buffer.begin(), it) + doubleCRLF.size();
// }

// Function to find the end of headers (double CRLF)
size_t findHeadersEnd(const std::vector<unsigned char>& buffer, size_t start) {
    const char* doubleCRLF = "\r\n\r\n";
    size_t crlfSize = strlen(doubleCRLF);
    size_t bufferSize = buffer.size();
    
    for (size_t i = start; i <= bufferSize - crlfSize; ++i) {
        bool found = true;
        for (size_t j = 0; j < crlfSize; ++j) {
            if (buffer[i + j] != static_cast<unsigned char>(doubleCRLF[j])) {
                found = false;
                break;
            }
        }
        if (found) {
            return i + crlfSize;
        }
    }
    return bufferSize; // Return the size of the buffer if the headers end is not found
}

//END TESTES

void Connection::processPost()
{
    std::string     fileName;
    std::string     boundary;
    std::string     body;


    body = _requestConnection.getRequestBody();
    body = parseBody(body);
    boundary = _requestConnection.getBoundary();
    fileName = _requestConnection.getPostFileName();
    std::cout << BBLUE "boundary: "  RESET<< boundary << std::endl;
    std::cout << BBLUE "fileName: "  RESET<< fileName << std::endl;

    // create a file with the name of fileName and store the body of the request

    //TESTEES

    std::vector<unsigned char> boundaryVect(boundary.begin(), boundary.end());
    // Find the start of the first part
    size_t part_start = findBoundary(_buffer2, boundaryVect, 0);
    if (part_start == _buffer2.size()) {
        std::cerr << "BoundaryVect not found" << std::endl;
        return ;
    }

    // Move past the boundaryVect to start parsing headers
    part_start += boundaryVect.size() + 2; // +2 for the CRLF following the boundaryVect

    // Find the end of the first part (next boundaryVect)
    size_t part_end = findBoundary(_buffer2, boundaryVect, part_start);

    // Extract the part between boundaries
    std::vector<unsigned char> part = extractSubVector(_buffer2, part_start, part_end);

    size_t headers_end = findHeadersEnd(part, 0);
    if (headers_end == part.size()) {
        std::cerr << "Headers end not found" << std::endl;
        return ;
    }

    // Extract the binary data (after headers and CRLF)
    std::vector<unsigned char> binary_data = extractSubVector(part, headers_end, part.size() - 2); // -2 to exclude trailing CRLF

    // Write binary data to a file
    std::ofstream file(fileName, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(binary_data.data()), binary_data.size());
        file.close();
        std::cout << "File saved as " << fileName << std::endl;
    } else {
        std::cerr << "Failed to open file for writing" << std::endl;
    }
    //END TESTES
    // std::ofstream file(fileName);
    // file << body;
    // file.close();
    // std::cout << BBLUE "file created: "  RESET<< fileName << std::endl;
}


std::string Connection::genResponse(RequestParser &request)
{
    std::string method;
    std::string response;

    method = request.get()["REQUEST_METHOD"];
    if (method == "GET")
        return genResponseGET(request);
    else if (method == "POST")
    {
        _keepAlive = true;

        if (request.getBodyWatchDog() == 1)
            this->processPost();
        // std::string body = genBodyFile("./html/index.html");
        // std::string header = genHeaderHTTP(body, "./html/index.html");
        //send http 100 continue
        std::string continueRequest = "HTTP/1.1 100 Continue\r\n\r\n";
        return continueRequest;
    }
    else
        throw ServerException("genResponse: method not configured: " + method, 400);
}