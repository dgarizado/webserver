/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 12:59:17 by vcereced          #+#    #+#             */
/*   Updated: 2024/05/19 19:03:26 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <iostream>
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#define RED "\033[31m"
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define PORT 85


// CONTENT-TYPE                        X
// CONTENT-LENGHT                      X
// REQUEST-METHOD.   X                 X
// SCRIPT-NAME       X                 X
// SERVER-PORT       X                 X
// SERVER-PROTOCOL.   X                X

typedef struct get_s{

	std::string REQUEST_METHOD;
	std::string QUERY_STRING;
	std::string SCRIPT_NAME;
}get_t;

typedef struct post_s{

	std::string REQUEST_METHOD;
	std::string CONTENT_TYPE;
	std::string SCRIPT_NAME;
	std::string CONTENT_LENGTH;

}post_t;


std::string readFileHtml(std::string filename)
{
	// Crear un flujo de entrada de archivo
    std::ifstream file(filename);
	// Verificar si el archivo se abrió correctamente
    if (!file.is_open()) {
        std::cerr << RED << "No se pudo abrir el archivo: " << RESET << filename << std::endl;
	}
	// Usar un stringstream para leer todo el contenido del archivo en un buffer
    std::stringstream buffer;
    buffer << file.rdbuf();
	// Obtener el contenido del buffer como una cadena
    std::string fileContents = buffer.str();
    // Cerrar el archivo
    file.close();
	return (fileContents);
}

std::string runCgi(void)
{
    FILE* pipe = popen("./cgi-bin/a.out", "r+");
    if (!pipe) {
        std::cerr << "Error al abrir el archivo CGI" << std::endl;
    }

    //fprintf(pipe, "%s", request.c_str());
    fflush(pipe); // Limpiar el buffer para asegurar que los datos se envíen al proceso

    // Leer la salida del archivo CGI línea por línea
    std::string pipeOutput;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        pipeOutput += buffer;
        std::cout << "reading pipe.." << std::endl;
    }

    // Cerrar el archivo CGI
    pclose(pipe);

     //std::cout << "INSIDE RUN CGI\n" << buffer << std::endl;

    return   pipeOutput;
}

std::string runCgiPOST(std::string request)
{
    FILE* pipe = popen("./cgi-bin/a.out", "r+");
    if (!pipe) {
        std::cerr << "Error al abrir el archivo CGI" << std::endl;
    }

    fprintf(pipe, "%s", request.c_str());
    fflush(pipe); // Limpiar el buffer para asegurar que los datos se envíen al proceso

    // Leer la salida del archivo CGI línea por línea
    std::string pipeOutput;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        pipeOutput += buffer;
        std::cout << "reading pipe.." << std::endl;
    }

    // Cerrar el archivo CGI
    pclose(pipe);

     //std::cout << "INSIDE RUN CGI\n" << buffer << std::endl;

    return   pipeOutput;
}

std::string readHttpRequest(int clientSocket)
{
	char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    // Leer la solicitud del cliente
    if (read(clientSocket, buffer, sizeof(buffer) - 1) == 0)
        std::cout << RED << "no HTTP READED!" << RESET << std::endl;
    std::cout << GREEN << "----------------------------\nSolicitud recibida:\n" << RESET << buffer << GREEN << "\n--------------------------------\n" << RESET << std::endl;
    return std::string(buffer);
}

void setEnvironmentVars(get_t t)
{
	std::string name = "REQUEST_METHOD";
	if (setenv(name.c_str(), t.REQUEST_METHOD.c_str(), 1) != 0) {
        std::cerr << "Error al establecer la variable de entorno " <<  t.REQUEST_METHOD << std::endl;
        exit(1);
    }
    name = "QUERY_STRING";
	if (setenv(name.c_str(), t.QUERY_STRING.c_str(), 1) != 0) {
        std::cerr << "Error al establecer la variable de entorno " <<  t.QUERY_STRING << std::endl;
        exit(1);
    }
    name = "SCRIPT_NAME";
	if (setenv(name.c_str(), t.SCRIPT_NAME.c_str(), 1) != 0) {
        std::cerr << "Error al establecer la variable de entorno " <<  t.SCRIPT_NAME << std::endl;
        exit(1);
    }
    std::cout << GREEN << "GET" << RESET << std::endl;
}

void setEnvironmentVars(post_t t)
{
	std::string name = "REQUEST_METHOD";
	if (setenv(name.c_str(), t.REQUEST_METHOD.c_str(), 1) != 0) {
        std::cerr << "Error al establecer la variable de entorno " <<  t.REQUEST_METHOD << std::endl;
        exit(1);
    }
    name = "CONTENT_TYPE";
	if (setenv(name.c_str(), t.CONTENT_TYPE.c_str(), 1) != 0) {
        std::cerr << "Error al establecer la variable de entorno " <<  t.CONTENT_TYPE << std::endl;
        exit(1);
    }
    name = "SCRIPT_NAME";
	if (setenv(name.c_str(), t.SCRIPT_NAME.c_str(), 1) != 0) {
        std::cerr << "Error al establecer la variable de entorno " <<  t.SCRIPT_NAME << std::endl;
        exit(1);
    }
    name = "CONTENT_LENGHT";
	if (setenv(name.c_str(), t.CONTENT_LENGTH.c_str(), 1) != 0) {
        std::cerr << "Error al establecer la variable de entorno " <<  t.CONTENT_LENGTH << std::endl;
        exit(1);
    }
    std::cout << GREEN << "POST" << RESET << std::endl;
}

// CONTENT-TYPE                        X
// CONTENT-LENGHT                      X
// REQUEST-METHOD.   X                 X
// SCRIPT-NAME       X                 X
// SERVER-PORT       X                 X
// SERVER-PROTOCOL.   X                X
// Función para manejar la conexión del cliente
std::string executeCGI(int clientSocket, std::string request) {

	//var extrated from request obj than contains the request parsed in map for example
	get_t HttpGetsimulation;
	HttpGetsimulation.REQUEST_METHOD = "GET";
	HttpGetsimulation.QUERY_STRING = "username=John";
	HttpGetsimulation.SCRIPT_NAME = "/cgi-bin/cgi";
	
	post_t HttpPostsimulation;
	HttpPostsimulation.REQUEST_METHOD = "POST";
	HttpPostsimulation.CONTENT_TYPE = "application/x-www-form-urlencoded";
	HttpPostsimulation.SCRIPT_NAME = "/cgi-bin/cgi";
	HttpPostsimulation.CONTENT_LENGTH = "29";

    std::string method;
    std::cout << "soy executeCGI, method?=" << std::endl;
    std::cin >> method;
	
    //Formating a response
    std::string httpLineStatus =  "HTTP/1.1 200 OK\r\n";
	std::string httpHeader =  "Content-Type: text/html\r\n\r\n";
    std::string httpBody;

	if (method == "GET")
	{
		setEnvironmentVars(HttpGetsimulation);
        std::cout << GREEN << "setted environments variables of GET" << RESET << std::endl;
        httpBody = runCgi();
	}
	else if (method == "POST")
	{
		setEnvironmentVars(HttpPostsimulation);
        std::cout << GREEN << "setted environments variables of POST" << RESET << std::endl;
        httpBody = runCgiPOST(request);
	}
    

	// if (analisis == 1)
    // {
	// 	//httpBody =  readFileHtml("./html.html");
	// 	httpBody =  runCgi(buffer);
    //     std::cout << "body generado de RUN CGI\n" << httpBody << std::endl;
    // }
	// else if (analisis == -1)
    //     std::cout << RED << "favicon requested i dont know what to do!" << RESET << std::endl;
    // else
	// 	httpBody =  readFileHtml("./html.html");


    // Respuesta HTTP simple
    std::string httpResponse = httpLineStatus + httpHeader + httpBody;
    // Enviar la respuesta al cliente
    return (httpResponse);

}

int initListenSocket(const int typeInet, const int typeStream, int flag)
{
	int listenSocket;

	listenSocket = socket(AF_INET, SOCK_STREAM, flag);
    if (listenSocket < 0)
	{
        std::cerr << "Error al crear el socket" << std::endl;
        return -1;
    }else
		return listenSocket;
}

int initBind(int &listenSocket)
{
    int port = PORT;
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Vincular el socket a la dirección y puerto especificados
    if (bind(listenSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error al vincular el socket" << std::endl;
        return -1;
    }
	return 0;
}

void sendHtml(int clientSocket)
{
    
	std::string httpLineStatus =  "HTTP/1.1 200 OK\r\n";
	std::string httpHeader =  "Content-Type: text/html\r\n\r\n";
	std::string httpBody = readFileHtml("./html.html");

    std::string httpResponse = httpLineStatus + httpHeader + httpBody;
    
    write(clientSocket, httpResponse.c_str(), httpResponse.length());
}

int main(int argc, char **argv) {
    int listenSocket;
	int clientSocket;

    // Crear el socket
	listenSocket = initListenSocket(AF_INET, SOCK_STREAM, 0);
	if (listenSocket == -1)
		return (1);
    // Configurar la dirección del servidor
	if (initBind(listenSocket) == -1)
		return (1);
    // Escuchar las conexiones entrantes
    if (listen(listenSocket, 5) < 0) {
        std::cerr << "Error al escuchar en el socket" << std::endl;
        return (1);
    }

    std::cout << GREEN << "Servidor escuchando en puerto 80..." << RESET << std::endl;
	
	//Loop of started server
    while (true)
	{
        std::cout << GREEN << "waiting conextions...." << RESET << std::endl;
        clientSocket = accept(listenSocket, nullptr, nullptr);
        if (clientSocket < 0) {
		    std::cerr << RED << "Error al aceptar la conexión" << RESET << std::endl;
            continue;
        }
        //just print the Request
		std::string request = readHttpRequest(clientSocket);//just for print the request message
        std::string action;
        std::cout << "send html or cgi?=" << std::endl;
        std::cin >> action;
        if (action == "cgi")
        {
            std::string httpResponse = executeCGI(clientSocket, request);
            //SEND TO CLIENT
            write(clientSocket, httpResponse.c_str(), httpResponse.length());
        }
        else if (action == "html")
        {
            std::cout << GREEN << "sending html" << RESET << std::endl;
            sendHtml(clientSocket);
            std::cout << GREEN << "sending already html" << RESET << std::endl;
        }
    	close(clientSocket);
    }
    close(listenSocket);

    return 0;
}


