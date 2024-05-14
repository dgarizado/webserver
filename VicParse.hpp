#ifndef VICPARSE_HPP
#define VICPARSE_HPP

#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <variant>
#include <sstream>
#include <cstring>

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define RESET "\033[0m"

typedef struct s_location {
    std::string                 uri;
    std::string 			    root;
    std::vector<std::string>    index;
}t_location;

typedef struct s_server {
    std::vector<std::string>    server_name;
    int 					    listen;
    std::string 			    errorLogs;
    std::string 			    workerConnections;
	std::vector<t_location>	    locations;
}t_server;

typedef struct s_fileParse {
    int                         test;
	std::set<int>               ports;
	std::string                 errorLog;
	std::string                 workerConnections;
    std::vector<t_server>       serverData;
}t_fileParse;

class VicParse {
public:
	
    // // Constructor por defecto
    VicParse(void)
    {
    //   memset(&this->configData, 0, sizeof(t_fileParse));
    }

    // // Destructor
    // ~VicParse() = default;

    // Método para cargar la configuración desde un archivo
    int loadConfigFromFile(const std::string& filename);

    // Método para obtener un valor de la configuración
    //std::variant<int, std::string, std::map<std::string, std::string>> getValue(const std::string& key) const;

    //getter
    t_fileParse & getStruct(void);

private:
	t_fileParse configData;

};

#endif  // VICPARSE_HPP 



