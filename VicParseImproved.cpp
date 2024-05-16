#include "VicParse.hpp"

int ft_err(std::string str)
{
    std::cerr  << str << std::endl;
    return -1;
}

void ft_braces(int &braces, std::string &token)
{
    if (token == "{")
        braces++;
    else if (token == "}")
        braces--;
}


int varLocation(VicParse *ref, std::istringstream &iss, int &nServer, int &nLocation, std::string &token, int *varsCheck)
{
    //int encounter = 0;

    if (token == "root" && (varsCheck[ROOT] = 1))
    {
        iss >> token; 
        token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
        ref->getStruct().serverData[nServer].locations[nLocation].root = token;
    }
    else if (token == "index" && (varsCheck[INDEX] = 1))
    {
        while (iss >> token)
        {
            token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
            ref->getStruct().serverData[nServer].locations[nLocation].index.push_back(token);
        }
    }
    return 0;
}


void insideLocation(VicParse *ref,std::ifstream &file, std::istringstream &iss, int &nServer, int &nLocation, std::string &token)
{
    int 	        locationBraces = 0;

    int             v[2] = {0, 0};//to markdown the vars to find
    std::string     line;
    t_location      empty;

    if (token == "location")
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
                varLocation(ref, iss, nServer, nLocation, token, v);
		    }
            if (locationBraces > 0 && std::getline(file, line))
            {
                iss.clear();    
                iss.str(line);
            }
        }while (locationBraces > 0);

        if (v[ROOT] == 0 || v[INDEX] == 0)
            throw std::runtime_error("location block wrong");
    }

}

void varServer(VicParse *ref, std::ifstream &file, std::istringstream &iss, int &nServer, int &nLocation, std::string &token, int *varsCheck)
{
    int port;

    if (token == "listen" && (varsCheck[LISTEN] = 1))
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
    else if (token == "server_name" && (varsCheck[SERVER_NAME] = 1))
    {
       while (iss >> token)
        {
            token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
            ref->getStruct().serverData[nServer].server_name.push_back(token);
        }
    }
    else if (token == "location" && (varsCheck[LOCATION] = 1))
        insideLocation(ref, file, iss, nServer, nLocation, token);
}

int insideServer(VicParse *ref, std::ifstream &file, std::istringstream &iss, std::string &token)
{
    static int      nServer = -1;
    int             nLocation = -1;
    int 	        serverBraces = 0;
	int 		    encounter = -1;
    int             v[3] = {0, 0, 0};//to markdown the vars to find
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
                varServer(ref, file, iss, nServer, nLocation, token, v);
		    }
            if (serverBraces > 0 && std::getline(file, line))
            {
                iss.clear();    
                iss.str(line);
            }
        }while (serverBraces > 0);

        if (v[LISTEN] == 0 || v[SERVER_NAME] == 0 || v[LOCATION] == 0)
            throw std::runtime_error("Server block wrong");
    }
    return encounter;
}

/**
 * @brief iter if Http block till close the block 
 */
int insideHttp(VicParse *ref, std::ifstream &file, std::istringstream &iss, std::string &token)
{
	int 	        httpBraces = 0;
	int 		    encounter = -1;
    bool            server = false;
    std::string     line;

	if (token == "http" && (encounter = 1))
    {
        do{
            while(iss >> token)
		    {
			    ft_braces(httpBraces, token);
                if (insideServer(ref, file, iss, token) == 1)
                    server = true;
            }   
            if (httpBraces > 0 && std::getline(file, line))
            {
                iss.clear();    
                iss.str(line);
            }
        }while (httpBraces > 0);
        
        if (!server)
            throw std::runtime_error("Http block wrong");
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
    outsideHttp(ref, iss, token);//MAY DELETE CUZ DONT NEED VARS OUTSIDE OF HTTP SERVERS

    return (insideHttp(ref, file, iss, token));//jump into if "http"
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
    bool                http = false;
    
    // Checks is file open
    if (!file.is_open()) {
        std::cerr << RED << "Error: cannot open file. " << RESET << filename << std::endl;
        return -1;
    }

    // Lee el archivo línea por línea y procesa la configuración
    while (std::getline(file, line)) 
    {  
		try{

            if (lineParser(this, file, line) == 1)
                http = true; 
		
        }catch(const std::exception& e) {
			std::cerr << RED << "Exception: " << e.what() << RESET << std::endl;
            return -1;    
        }
    }
    file.close();

    if (http == false)
        return ft_err("error: not found http block");
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
