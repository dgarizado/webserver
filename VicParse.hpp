#ifndef VICPARSE_HPP
# define VICPARSE_HPP

# include <fstream>
# include <set>
# include <vector>
# include <string>
# include <sstream>
# include <iostream>
# include <unistd.h>

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define RESET "\033[0m"

enum nums {
    LISTEN = 0,
    SERVER_NAME = 1,
    LOCATION = 2,
    ROOT = 0,
    INDEX = 1
};

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
    VicParse(void) {}

    int loadConfigFromFile(const std::string& filename);
    void showConfig(void);

    //getter
    t_fileParse &getStruct(void);

private:
	t_fileParse configData;

};

#endif  // VICPARSE_HPP 



