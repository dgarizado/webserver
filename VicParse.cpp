#include <fstream>
#include <iostream>
#include "VicParse.hpp"


/**
 * @brief Getter the reference of the structure atribute of VicParse 
 * @return reference to t_fileParse of instance 
 */
 t_fileParse & VicParse::getStruct()
 {
    return this->configData;
 }


void ft_brackets(int &server, std::string &token, int &brackets)
{
    if (token == "{")
        brackets++;
    else if (token == "}")
        brackets--;
    
    if (brackets == 0)
        server = 0;
}


int isVarInsideServer(VicParse *ref, int &nServer, std::istringstream &iss, std::string &token, int &server, int &brackets)
{
    int encounter = 0;
    int valor_entero = 0;
    if (token == "listen")
    {
        iss >> token;
        token.erase(std::remove(token.begin(), token.end(), ';'), token.end());

        try {
        valor_entero = std::stoi(token); // Convertir el token a int
        } catch (const std::invalid_argument& e) {
        std::cerr << "Error: not valid num" << std::endl;
        } catch (const std::out_of_range& e) {
        std::cerr << "Error: num out of range" << std::endl;
        }
        ref->getStruct().ports.insert(valor_entero);
        ref->getStruct().serverData[nServer].listen = valor_entero;
        std::cout << GREEN << "var: serverData[" << nServer << "].listen= " << valor_entero << RESET << std::endl;
        encounter = 1;
    }
    else if (token == "server_name")
    {
        while (iss >> token)
        {
            token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
            std::string name(token);
            ref->getStruct().serverData[nServer].server_name.push_back(name);
        }
    }
    
    
    if (server)
            ft_brackets(server, token, brackets);
    while (iss >> token)
    {
        if (server)
            ft_brackets(server, token, brackets);
    }


    return encounter;

}

int isServer(VicParse *ref, std::istringstream &iss, int &nServer, std::string &token, int &server, int &brackets)
{
    int encounter = 0;
    std::cout << YELLOW << "IS SERVER token= " <<token << " status server= " << server << " N server= " << nServer << " brackets= " << brackets << RESET << std::endl;
    if (token == "server")
    {
        t_server empty;
        server++;
        encounter++;
        nServer++;
        ref->getStruct().serverData.push_back(empty);
        std::cout << GREEN << "push_back de una struct t_server !!!" << RESET << std::endl;
    }

    while (iss >> token)
    {
        if (server)
            ft_brackets(server, token, brackets);
    }
    return encounter;
}

int varServer(VicParse *ref, std::istringstream &iss, std::string &token, int &server, int &brackets)
{
    //index of vhost(server)
    static int          nServer = -1;


    std::cout << YELLOW << "VAR SERVER token= " << token << " status server= " << server << " N server= " << nServer << " brackets= " << brackets << RESET << std::endl;
    //si ya esta dentro de un bloque server
    if (server)
        return isVarInsideServer(ref, nServer, iss, token, server, brackets);
    else
    {
        //si todavia no ha entrado en un bloque server check, si empieza el bloque
        return (isServer(ref, iss, nServer, token, server, brackets));
    }

}


int VarOutsideServer(VicParse *ref, std::istringstream &iss, std::string &token)
{
    int encounter = 0;
    if (token == "error_log")
    {
        iss >> token;
        token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
        ref->getStruct().errorLog = token;
        encounter = 1;
    }
    else if (token == "worker_connections")
    {
        iss >> token;
        token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
        ref->getStruct().workerConnections = token;
        encounter = 1;
    }

    return encounter;
}


int lineParser(VicParse *ref, std::string &line)
{
    std::istringstream  iss(line);
    std::string         token;
    static int          server = 0;
   
    static int          brackets = 0;

   
    //Procesa el primer token
    iss >> token;
    //index of token

    if (VarOutsideServer(ref, iss, token) || varServer(ref, iss, token, server, brackets))
    {
            std::cout << GREEN << "register var= " << token << RESET << std::endl;
    }

    //std::cout << GREEN << token << RESET << std::endl;
     

    return 0;
}


/**
 * @brief Method to load on ifstream object the full config file. It's try to open the file.
 * Iter line per line parsing the file
 * 
 * @param filename string route of the file
 * @return int -1 err. 0 ok.
 */
int VicParse::loadConfigFromFile(const std::string& filename)
{
    std::ifstream       file(filename);
    std::string         line;
    
    // Abre el archivo
    if (!file.is_open()) {
        std::cerr << RED << "Error: No se pudo abrir el archivo: " << RESET << filename << std::endl;
        return -1;
    }

    // Lee el archivo línea por línea y procesa la configuración
    while (std::getline(file, line)) 
    {
        
        std::cout << "DEGUB LINE " << line << std::endl;

        lineParser(this, line);

        
        //Check if iter properly the file DELETE LATER
       std::cout << "-----------NEXT LINE-------------" << std::endl;
       getchar();
    }


    //PRINT THE WHOLE STRUCTURE!!!!
     std::cout << "----------- STRUCT -------------" << std::endl;
    std::cout << GREEN << "/errorLogs= " << this->configData.errorLog << RESET << std::endl;
    std::cout << GREEN << "/workerConnections= " << this->configData.workerConnections << RESET << std::endl;
    std::cout << GREEN << "/VECTOR:port= " ;
    for (std::set<int>::iterator it = this->configData.ports.begin(); it != this->configData.ports.end(); ++it) {
        std::cout << *it <<", ";
    }
    std::cout << RESET << std::endl;
    std::cout << "--------vector<t_server>[0]----------" << std::endl;
    std::cout << GREEN "/serverData[0].listen= " << this->configData.serverData[0].listen << RESET << std::endl;
    std::cout << GREEN << "/serverData[0].server_name= " ;
    for (std::vector<std::string>::iterator it = this->configData.serverData[0].server_name.begin(); it != this->configData.serverData[0].server_name.end(); ++it) {
        std::cout << *it <<", ";
    }
    std::cout << RESET << std::endl;

    std::cout << "--------vector<t_server>[1]----------" << std::endl;
    std::cout << GREEN "/serverData[1].listen= " << this->configData.serverData[1].listen << std::endl;
    std::cout << GREEN << "/serverData[1].server_name= " ;
    for (std::vector<std::string>::iterator it = this->configData.serverData[1].server_name.begin(); it != this->configData.serverData[1].server_name.end(); ++it) {
        std::cout << *it <<", ";
    }
    std::cout << RESET << std::endl;
    // Cierra el archivo
    file.close();
    return 0;
}
