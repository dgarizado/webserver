#include "VicParse.hpp"

void ft_braces(int &braces, std::string &token)
{
    if (token == "{")
        braces++;
    else if (token == "}")
        braces--;
}

int varServer(VicParse *ref, std::istringstream &iss, int &nServer, std::string &token)
{
    int encounter = 0;
    int port;

    if (token == "listen" && (encounter = 1))
    {
        iss >> token;
        token.erase(std::remove(token.begin(), token.end(), ';'), token.end());

        try {
        port = std::stoi(token); // Convertir el token a int
        }catch(const std::exception& e) {
            std::cerr << RED << "Exception: " << e.what() << RESET << std::endl;}

        ref->getStruct().ports.insert(port);
        ref->getStruct().serverData[nServer].listen = port;
    }
    else if (token == "server_name" && (encounter = 1))
    {
       while (iss >> token)
        {
            token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
            ref->getStruct().serverData[nServer].server_name.push_back(token);
        }
    }
    return encounter;
}

int varLocation(VicParse *ref, std::istringstream &iss, int &nServer, int &nLocation, std::string &token)
{
    int encounter = 0;

    if (token == "root" && (encounter = 1))
    {
        iss >> token; 
        token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
        ref->getStruct().serverData[nServer].locations[nLocation].root = token;
    }
    else if (token == "index" && (encounter = 1))
    {
        while (iss >> token)
        {
            token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
            ref->getStruct().serverData[nServer].locations[nLocation].index.push_back(token);
        }
    }
    return 0;
}

int insideLocation(VicParse *ref,std::ifstream &file, std::istringstream &iss, int &nServer, int &nLocation, std::string &token)
{
    int 	        locationBraces = 0;
	int 		    encounter = 0;
    std::string     line;
    t_location      empty;

    if (token == "location" && (encounter = 1))
    {
        //push a empty t_location into locations vector
        nLocation++;
        ref->getStruct().serverData[nServer].locations.push_back(empty);
        //register the uri
        iss >> token;
        ref->getStruct().serverData[nServer].locations[nLocation].uri = token;
       do{
            while(iss >> token)
		    {
			    ft_braces(locationBraces, token);
                varLocation(ref, iss, nServer, nLocation, token);
		    }
            if (locationBraces > 0 && std::getline(file, line))
            {
                iss.clear();    
                iss.str(line);
            }
        }while (locationBraces > 0);
    }
    return encounter;
}

int insideServer(VicParse *ref,std::ifstream &file, std::istringstream &iss, std::string &token)
{
    static int      nServer = -1;
    int             nLocation = -1;
    int 	        serverBraces = 0;
	int 		    encounter = 0;
    std::string     line;
    t_server        empty;

    if (token == "server" && (encounter = 1))
    {
        //push a empty t_server in server vector
        nServer++;
        ref->getStruct().serverData.push_back(empty);
       do{
            while(iss >> token)
		    {
			    ft_braces(serverBraces, token);
                insideLocation(ref, file, iss, nServer, nLocation, token);
                varServer(ref, iss, nServer, token);
		    }
            if (serverBraces > 0 && std::getline(file, line))
            {
                iss.clear();    
                iss.str(line);
            }
        }while (serverBraces > 0);
    }
    return encounter;
}

/**
 * @brief iter if Http block till close the block 
 */
int insideHttp(VicParse *ref, std::ifstream &file, std::istringstream &iss, std::string &token)
{
	int 	        httpBraces = 0;
	int 		    encounter = 0;
    std::string     line;

	if (token == "http" && (encounter = 1))
    {
        do{
            while(iss >> token)
		    {
			    ft_braces(httpBraces, token);
                insideServer(ref, file, iss, token);
		    }
            if (httpBraces > 0 && std::getline(file, line))
            {
                iss.clear();    
                iss.str(line);
            }
        }while (httpBraces > 0);
    }
	return encounter;
}

/**
 * @brief set the variables outside of HTTP block
 */
int outsideHttp(VicParse *ref, std::istringstream &iss, std::string &token)
{
    int encounter = 0;

    if (token == "error_log" && (encounter = 1) && iss >> token)
    {
        token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
        ref->getStruct().errorLog = token;
    }
    else if (token == "worker_connections" && (encounter = 1) && iss >> token)
    {
        token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
        ref->getStruct().workerConnections = token;
    }
    return encounter;
}

/**
 * @brief iter the tokens of the line, then call to function according to server block or not.
 */
int lineParser(VicParse *ref, std::ifstream &file, std::string &line)
{
    std::istringstream  iss(line);
    std::string         token;
   
    //Procesa el primer token
    iss >> token;
    return (outsideHttp(ref, iss, token) || insideHttp(ref, file, iss, token));
}

/**
 * @brief Getter the reference of the structure atribute of VicParse 
 * @return reference to t_fileParse of instance 
 */
t_fileParse & VicParse::getStruct()
{
    return this->configData;
}

/**
 * @brief Method to load on ifstream object the full config file. Iter line per line parsing the file
 * @return int -1 cannot open file or any exception. 0 ok.
 */
int VicParse::loadConfigFromFile(const std::string& filename)
{
    std::ifstream       file(filename);
    std::string         line;
    
    // Checks is file open
    if (!file.is_open()) {
        std::cerr << RED << "Error: cannot open file. " << RESET << filename << std::endl;
        return -1;
    }

    // Lee el archivo línea por línea y procesa la configuración
    while (std::getline(file, line)) 
    {  
		try{
            lineParser(this, file, line);
		}catch(const std::exception& e) {
			std::cerr << RED << "Exception: " << e.what() << RESET << std::endl;
            return -1;    
        }
    }
    file.close();
    return 0;
}

void VicParse::showConfig(void)
{
    std::cout << "/errorLogs\t\t\t= " << this->getStruct().errorLog << std::endl; //DELETEEE!!
    std::cout << "/workerConnections\t\t= " << this->getStruct().workerConnections << std::endl; //DELETEEE!!
    std::cout << "/ports\t\t\t\t= " ;
    for (std::set<int>::iterator it = this->configData.ports.begin(); it != this->configData.ports.end(); ++it) {std::cout << *it << " ";} std::cout << std::endl;
    std::cout << "/serverData/ " << std::endl;
    for (std::vector<t_server>::iterator it = this->configData.serverData.begin(); it != this->configData.serverData.end(); ++it) {
        std::cout << "           /server_name\t\t= " ;
        for (std::vector<std::string>::iterator itVec = it->server_name.begin(); itVec != it->server_name.end(); ++itVec) {std::cout << *itVec << " ";} std::cout << std::endl;
        std::cout << "\t   /listen\t\t= " << it->listen << std::endl;
        for (std::vector<t_location>::iterator itLoc = it->locations.begin(); itLoc != it->locations.end(); ++itLoc) {
            std::cout << "\t   /location/" << std::endl;
            std::cout << "\t\t    /uri\t= " << itLoc->uri << std::endl;
            std::cout << "\t\t    /root\t= " << itLoc->root << std::endl;
            std::cout << "\t\t    /index\t= ";
            for (std::vector<std::string>::iterator itIndex = itLoc->index.begin(); itIndex != itLoc->index.end(); ++itIndex) {std::cout << *itIndex << " "; } std::cout << std::endl;
        }
    std::cout << "-------------------------------------------------------------------------" << std::endl;
    }
}
