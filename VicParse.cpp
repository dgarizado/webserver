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
        server--;
}

// void setData(VicParse *ref, std::string &data)
// {
//     ref->getStruct().test = data;
// }

int isVarInsideServer(VicParse *ref, std::istringstream &iss, std::string &token, int &server, int &brackets)
{
    int encounter = 0;
    int valor_entero = 0;
    if (token == "listen")
    {
        iss >> token;
        token.erase(std::remove(token.begin(), token.end(), ';'), token.end());

        try {
        valor_entero = std::stoi(token); // Convertir el token a int
        std::cout << "Valor entero: " << valor_entero << std::endl;
        } catch (const std::invalid_argument& e) {
        std::cerr << "Error: El token no es un número válido." << std::endl;
        } catch (const std::out_of_range& e) {
        std::cerr << "Error: El número es grande o pequeño para ser representado como int." << std::endl;
        }
        std::cout << "antes" << std::endl;
        ref->getStruct().ports.insert(valor_entero);
        std::cout << "despues" << std::endl;
        encounter = 1;
    }
    
    while (iss >> token)
    {
        ft_brackets(server, token, brackets);
    }

    return encounter;

}

int isServer(std::istringstream &iss, std::string &token, int &server, int &brackets)
{
    int encounter = 0;

    if (token == "server")
    {
        server++;
        encounter++;
    }

    while (iss >> token)
    {
        ft_brackets(server, token, brackets);
    }
    return encounter;
}

int varServer(VicParse *ref, std::istringstream &iss, std::string &token, int &server, int &brackets)
{
    //si ya esta dentro de un bloque server
    if (server)
        return isVarInsideServer(ref, iss, token, server, brackets);
    else
    {
        //si todavia no ha entrado en un bloque server check, si empieza el bloque
        return (isServer(iss, token, server, brackets));
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
    
    std::cout << YELLOW << "/errorLogs= " << this->configData.errorLog << RESET << std::endl;
    std::cout << YELLOW << "/workerConnections= " << this->configData.workerConnections << RESET << std::endl;
     for (int elem : this->configData.workerConnections) {
        std::cout << YELLOW << "/VECTOR:port= " << elem << RESET << std::endl;
    }
    std::cout << YELLOW << "/VECTOR:port= " << this->configData.workerConnections << RESET << std::endl;
    // Cierra el archivo
    file.close();
    return 0;
}
