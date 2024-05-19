#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib> // Para getenv
#define RED "\033[31m"
#define RESET "\033[0m"
#define GREEN "\033[32m"


int main(void)
{
    std::cout << GREEN << "soy CGI !" << RESET << std::endl;
    const char *ptr = std::getenv("REQUEST_METHOD");
    std::string method;
    std::string query;
    std::string length;

    if (ptr != nullptr)
        method = ptr;

    ptr = std::getenv("QUERY_STRING");
    if (ptr != nullptr)
        query = ptr;

     ptr = std::getenv("CONTENT_LENGHT");
    if (ptr != nullptr)
        length = ptr;

    std::cout << "ENV!  QUERY= "<< query << "lengght= " << length << std::endl;

    std::string value;

    if (method == "GET")
    {
        int start = query.find('=');
        int end = query.find('&');
        value = query.substr(start + 1, end - start-1);
    }

   if (method == "POST") {
        // Obtener la longitud del contenido
            int contentLength = std::stoi(length);

            // Leer el cuerpo de la solicitud
            std::string postData;
            postData.resize(contentLength);
            std::cin.read(&postData[0], contentLength);

            value = postData;

            std::cerr << "valor de POSDATA EN CGI= " << value << std::endl;
        } else {
            std::cerr << "Error: CONTENT_LENGTH no está definida." << std::endl;
        }
    

// //std::cout << GREEN << "detected REQUEST_METHOD parsed = POST" << RESET << std::endl;




//     // Variables para almacenar la URI
//     //std::string request;
//     // Leer la request de STDIN
//     //std::getline(std::cin, request);
	

	std::cout << "<!DOCTYPE html>\n";
	std::cout << "<html>\n<head>\n<title>Success Page</title>\n</head>\n<body>\n";

    // Format the username with <strong> and <span> for larger text
    std::cout << "<p><strong><span style='font-size: 40px;'>"; // Empieza el párrafo con un span y strong con un tamaño de fuente grande
    std::cout << value << "</span></strong> Congratulations !!</p>\n"; // Muestra el nombre de usuario y un mensaje de felicitaciones

    // Print the second line on a new line
    std::cout << "<p>You have successfully sent data from an HTML form to a CGI C++ script.</p>\n"; // Muestra un mensaje indicando que los datos del formulario se han procesado correctamente

    // Close the HTML tags
    std::cout << "</body>\n</html>\n";

    return 0;
}
