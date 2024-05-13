#include <fstream>
#include <iostream>
#include "VicParse.hpp"




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
    std::string         token;
    
    // Abre el archivo
    if (!file.is_open()) {
        std::cerr << RED << "Error: No se pudo abrir el archivo: " << RESET << filename << std::endl;
        return -1;
    }

    // Lee el archivo línea por línea y procesa la configuración
    while (std::getline(file, line)) 
    {
        std::istringstream  iss(line);

        std::cout << "DEGUB LINE" << line << std::endl;

        //Procesa cada token y carga la configuración en los atributos
        while (iss >> token)
        {
            this->test = token;
            //DEBUG DELETE 
            //std::cout << GREEN << token << RESET << std::endl;
            //std::cout << YELLOW << this->test << RESET << std::endl;
        }


        
        //std::string token;
        //while (iss >> token) {
            // Procesa cada token y carga la configuración en configData
        //}
        
        //Check if iter properly the file DELETE LATER
       //std::cout << line << std::endl;
       std::cout << "-----------NEXT LINE-------------" << std::endl;
       getchar();
    }

    // Cierra el archivo
    file.close();
    return 0;
}
