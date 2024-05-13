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
    std::ifstream file(filename);
    std::string line;
    
    // Abre el archivo
    if (!file.is_open()) {
        std::cerr << RED << "Error: No se pudo abrir el archivo: " << RESET << filename << std::endl;
        return -1;
    }

    // Lee el archivo línea por línea y procesa la configuración
    while (std::getline(file, line)) {
        // Aquí puedes implementar la lógica para procesar cada línea del archivo
        // Por ejemplo, puedes dividir la línea en tokens utilizando la función std::istringstream y cargar la configuración en configData
        // Aquí un ejemplo simple de cómo podrías dividir la línea en tokens:
        
        std::istringstream iss(line);
        //std::string token;
        //while (iss >> token) {
            // Procesa cada token y carga la configuración en configData
        //}
        
        //Check if iter properly the file DELETE LATER
       //std::cout << line << std::endl;
    }

    // Cierra el archivo
    file.close();
    return 0;
}
