#include <iostream>
#include "VicParse.hpp"

int main() {
    // Crea una instancia de VicParse
    VicParse FileParser;

    // Carga la configuración desde un archivo
    if (FileParser.loadConfigFromFile("./conf/server.conf") == -1)
    {
		  perror("parser error load from file");
		  return(errno);
	}else
		std::cout << GREEN << "parser load file ok" << RESET << std::endl;
    FileParser.showConfig();

    return 0;
}
