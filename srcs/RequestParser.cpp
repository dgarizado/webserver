/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 18:04:58 by vcereced          #+#    #+#             */
/*   Updated: 2024/06/20 21:02:02 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RequestParser.hpp"

/**
 * @brief just parsing the line of Request starting by "User-Agent:" 
 */
void userAgentLineParse(RequestParser *ref, std::istringstream &iss)
{
    std::string     token;
    std::string     tokenconcated;

    while (iss >> token)
        tokenconcated += token;
    ref->set("HTTP_USER_AGENT", tokenconcated);
}

/**
 * @brief just parsing the line of Request starting by a POST/PUT/DELETE/GET" 
 */
void receivedLineParse(RequestParser *ref, std::istringstream &iss, std::string &token)
{
    ref->set("REQUEST_METHOD", token);      //first token was extracted in previous funcion
    iss >> token;                           //extract next token from line in istringstream
    ref->set("REQUEST_URI", token); //HERE WE FIXED THE NAME FROM QUERY_STRING TO REQUEST_URI
    iss >> token;
    ref->set("SERVER_PROTOCOL", token);
}

/**
 * @brief iter the tokens of the line. it extract the first token to know
 * wich variable to set in Request's map.
 */
void lineParser(RequestParser *ref, std::string &requestLine)
{
    std::istringstream  iss(requestLine);
    std::string         token;
    
    //Process the first toke of the line
    iss >> token;
    
    if(token == "GET" || token == "POST" || token == "DELETE" || token == "PUT" || token == "HEAD" )
        receivedLineParse(ref, iss, token);
    else if (token == "User-Agent:")
        userAgentLineParse(ref, iss);
    else if (token == "Host:" && iss >> token)
        ref->set("HTTP_HOST", token);
    else if (token == "Accept:" && iss >> token)
        ref->set("HTTP_ACCEPT", token);
    else if (token == "Accept-Language:" && iss >> token)
        ref->set("HTTP_ACCEPT_LANGUAGE", token);
    else if (token == "Accept-Encoding:" && iss >> token)
        ref->set("HTTP_ACCEPT_ENCODING", token);    
    else if (token == "Content-Type:" && iss >> token)
        ref->set("CONTENT_TYPE", token);
    else if (token == "Content-Length:" && iss >> token)
        ref->set("CONTENT_LENGTH", token);
    else if (token == "DNT:" && iss >> token)
        ref->set("HTTP_DNT", token);
    else if (token == "Connection:" && iss >> token)
        ref->set("HTTP_CONNECTION", token);
    else if (token == "Upgrade-Insecure-Requests:" && iss >> token)
        ref->set("HTTP_UPGRADE_INSECURE_REQUESTS", token);
    else if (token == "Sec-Fetch-Dest:" && iss >> token)
        ref->set("HTTP_SEC_FETCH_DEST", token);
    else if (token == "Sec-Fetch-Mode:" && iss >> token)
        ref->set("HTTP_SEC_FETCH_MODE", token);  
    else if (token == "Sec-Fetch-Site:" && iss >> token)
        ref->set("HTTP_SEC_FETCH_SITE", token);
    else if (token == "Priority:" && iss >> token)
        ref->set("HTTP_PRIORITY", token);
    else if (token == "Pragma:" && iss >> token)
        ref->set("HTTP_PRAGMA", token);
    else if (token == "Cache-Control:" && iss >> token)
        ref->set("HTTP_CACHE_CONTROL", token);
}

/**
 * @brief Method to load on istringstream object the full message of request (client) 
 * This message had read before on the conexion's FD. Iter line per line parsing the message.
 * @return int -1 any exception. 0 ok.
 */
void RequestParser::loadConfigFromRequest(const std::string requestMessage)
{
    std::istringstream  stream(requestMessage);     // Create a string stream from a string message
    std::string         line;                       // string to store a line
 
    if ( requestMessage.empty() )
        throw std::runtime_error("loadConfigFromRequest: is empty");
        
    while (std::getline(stream, line))              //Extract line per line till reach EOF
    {  
		try{
            lineParser(this, line);                 //Parse line per line 
        }catch (const std::exception& e) {
            throw std::runtime_error("loadConfigFromRequest: lineParser: " + std::string(e.what()));}
    }
}

/**
 * @brief setter of the private Request's map.  
 */
void RequestParser::set(std::string key, std::string value)
{
    _requestData[key] = value;
}

/**
 * @brief return a reference of the private Request's map then can be manipulate
 * @return std::map<std::string, std::string>& 
 */
std::map<std::string, std::string> &RequestParser::get(void)
{
    return(_requestData);
}

/**
 * @brief write on terminal all the content of the Request's map for debugging.
 */
void RequestParser::showConfig(void)
{
    typedef std::map<std::string, std::string>::iterator it_t;
    
    std::cout << GREEN <<  "\n  <----------- HTTP REQUEST RECEIVED" << RESET << std::endl;
    for (it_t it = this->get().begin(); it != this->get().end(); ++it)
        std::cout << GREEN << it->first << RESET << " = " << YELLOW << it->second << RESET << std::endl;
     std::cout << GREEN <<  "-------------------------\n" << RESET << std::endl;
}

RequestParser::RequestParser(RequestParser const &src)
{
    *this = src;
}

RequestParser &RequestParser::operator=(RequestParser const &src)
{
    if (this != &src)
    {
        this->_requestData = src._requestData;
    }
    return *this;
}
