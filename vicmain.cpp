#include <iostream>
#include "VicParse.hpp"

int main() {
    // Crea una instancia de VicParse
    VicParse FileParser;

    // Carga la configuración desde un archivo
    if (FileParser.loadConfigFromFile("./conf/server.conf") == -1) {
		perror("parser error load from file");
		return(errno);
	}else
		std::cout << GREEN << "parser load file ok" << RESET << std::endl;

        // // Obtiene y muestra algunos valores de configuración
        // auto value = parser.getValue("port");
        // if (std::holds_alternative<int>(value)) {
        //     std::cout << "Puerto: " << std::get<int>(value) << std::endl;
        // } else {
        //     std::cout << "El valor del puerto no es un entero.\n";
        // }

        // value = parser.getValue("error_logs");
        // if (std::holds_alternative<std::string>(value)) {
        //     std::cout << "Logs de error: " << std::get<std::string>(value) << std::endl;
        // } else {
        //     std::cout << "El valor de los logs de error no es una cadena.\n";
        // }

        // value = parser.getValue("worker_connections");
        // if (std::holds_alternative<std::string>(value)) {
        //     std::cout << "Conexiones de trabajador: " << std::get<std::string>(value) << std::endl;
        // } else {
        //     std::cout << "El valor de las conexiones de trabajador no es una cadena.\n";
        // }
    

    return 0;
}
