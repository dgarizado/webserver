#include <fstream>
#include <iostream>
#include "VicParse.hpp"


void ft_braces(int &braces, std::string &token)
{
    if (token == "{")
        braces++;
    else if (token == "}")
        braces--;
}


/**
 * @brief Getter the reference of the structure atribute of VicParse 
 * @return reference to t_fileParse of instance 
 */
t_fileParse & VicParse::getStruct()
{
    return this->configData;
}

int varServer(VicParse *ref, std::istringstream &iss, int &nServer, std::string &token)
{
    int encounter = 0;
    int port;

    if (token == "listen")
    {
        iss >> token;
        token.erase(std::remove(token.begin(), token.end(), ';'), token.end());

        try {
        port = std::stoi(token); // Convertir el token a int
        }catch(const std::exception& e) {
            std::cerr << RED << "Exception: " << e.what() << RESET << std::endl;}

        ref->getStruct().ports.insert(port);
        ref->getStruct().serverData[nServer].listen = port;
        std::cout << GREEN << "var: serverData[" << nServer << "].listen= " << port << RESET << std::endl;
        encounter = 1;
    }
    else if (token == "server_name")
    {
       while (iss >> token)
        {
            token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
            std::string name(token);
            ref->getStruct().serverData[nServer].server_name.push_back(name);
            std::cout << GREEN << "var: serverData[" << nServer << "].server_name= " << name << RESET << std::endl;
        }
        encounter = 1;
    }
    return encounter;
}

int varLocation(VicParse *ref, std::istringstream &iss, int &nServer, int &nLocation, std::string &token)
{
    int encounter = 0;

    if (token == "root")
    {
        iss >> token;
        token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
        ref->getStruct().serverData[nServer].locations[nLocation].root = token;
        std::cout << GREEN << "var: serverData[" << nServer << "].locations[" << nLocation << "].root= " << token << RESET << std::endl;
        encounter = 1;
    }
    else if (token == "index")
    {

        while (iss >> token)
        {

            token.erase(std::remove(token.begin(), token.end(), ';'), token.end());

            ref->getStruct().serverData[nServer].locations[nLocation].index.push_back(token);
            std::cout << GREEN << "var: serverData[" << nServer << "].locations[" << nLocation << "].index= " << token << RESET << std::endl;
        }
        encounter = 1;
    }
    return 0;
}


int insideLocation(VicParse *ref,std::ifstream &file, std::istringstream &iss, int &nServer, int &nLocation, std::string &token)
{
    //static int      nLocation = -1;
    int 	        locationBraces = 0;
	int 		    encounter = 0;
    std::string     line;
    t_location      empty;

    if (token == "location" && (encounter = 1))
    {
        nLocation++;
        ref->getStruct().serverData[nServer].locations.push_back(empty);
        std::cout << GREEN << "into location block! append locations[" << nLocation <<"] " RESET << std::endl;
        //register the uri
        iss >> token;
        ref->getStruct().serverData[nServer].locations[nLocation].uri = token;
        std::cout << GREEN << "var: serverData[" << nServer << "].locations[" << nLocation << "].uri= " << token << RESET << std::endl;

       do{
            std::cout << YELLOW << "LOCATION DO LINE= " << line <<  " locationBraces= " << locationBraces << RESET << std::endl;
            while(iss >> token)
		    {
                std::cout << YELLOW << "LOCATION DO TOKEN= " << token <<  " locationBraces= " << locationBraces << RESET << std::endl;
			    ft_braces(locationBraces, token);
                if (varLocation(ref, iss, nServer, nLocation, token))
                    std::cout << GREEN << "register OK"<< RESET << std::endl;
		    }
            if (locationBraces > 0 && std::getline(file, line))
            {
                iss.clear();    
                iss.str(line);
                std::cout << "-----------NEXT LINE-------------" << std::endl;
            }
            getchar();
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
        nServer++;
        ref->getStruct().serverData.push_back(empty);
        std::cout << GREEN << "into server block! append serverData[" << nServer <<"] " RESET << std::endl;
        //Append new empty t_server into t_fileParse

       do{
            std::cout << YELLOW << "SERVER DO LINE= " << line <<  " serverBraces= " << serverBraces << RESET << std::endl;
            while(iss >> token)
		    {
                std::cout << YELLOW << "SERVER DO TOKEN= " << token <<  " serverBraces= " << serverBraces << RESET << std::endl;
			    ft_braces(serverBraces, token);
                insideLocation(ref, file, iss, nServer, nLocation, token);
                if (varServer(ref, iss, nServer, token))
                    std::cout << GREEN << "register OK"<< RESET << std::endl;
		    }
            if (serverBraces > 0 && std::getline(file, line))
            {
                iss.clear();    
                iss.str(line);
                std::cout << "-----------NEXT LINE-------------" << std::endl;
            }
            getchar();
        }while (serverBraces > 0);

    }

    return encounter;
}



int insideHttp(VicParse *ref, std::ifstream &file, std::istringstream &iss, std::string &token)
{
	int 	        httpBraces = 0;
	int 		    encounter = 0;
    std::string     line;

    
    //DEBUG
    //std::cout << YELLOW << "insideHttp= " << line <<  " httpBraces= " << httpBraces << RESET << std::endl;
	if (token == "http" && (encounter = 1))
    {
        std::cout << GREEN << "into http block " << RESET << std::endl;
		//parseHttp(ref, file, iss, httpBrace);
        do{
            std::cout << YELLOW << "HTTP DO LINE= " << line <<  " httpBraces= " << httpBraces << RESET << std::endl;
            while(iss >> token)
		    {
                std::cout << YELLOW << "HTTP DO TOKEN= " << token <<  " httpBraces= " << httpBraces << RESET << std::endl;
			    ft_braces(httpBraces, token);
                insideServer(ref, file, iss, token);
		    }
            if (httpBraces > 0 && std::getline(file, line))
            {
                iss.clear();    
                iss.str(line);
                std::cout << "-----------NEXT LINE-------------" << std::endl;
            }
            getchar();
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
    if (token == "error_log")
    {
        encounter++;
        iss >> token;
        token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
        ref->getStruct().errorLog = token;
    }
    else if (token == "worker_connections")
    {
        encounter++;
        iss >> token;
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

    if (outsideHttp(ref, iss, token) || insideHttp(ref, file, iss, token))
	    std::cout << GREEN << "var register" << token << RESET << std::endl;

    return 0;
}

/**
 * @brief Method to load on ifstream object the full config file. Iter line per line parsing the file
 * @return int -1 cannot open file. 0 ok.
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
        
        std::cout << "FROMFILE LINE " << line << std::endl;
		try{
            lineParser(this, file, line);
		}catch(const std::exception& e) {
			std::cerr << RED << "Exception: " << e.what() << RESET << std::endl;}

        //Check if iter properly the file DELETE LATER
       std::cout << "-----------NEXT LINE-------------" << std::endl;
       getchar();
    }

	
    // ONLY FOR DEBUG, PRINT THE WHOLE STRUCTURE!!!!

    //  std::cout << "----------- STRUCT -------------" << std::endl;
    // std::cout << GREEN << "/errorLogs= " << this->configData.errorLog << RESET << std::endl;
    // std::cout << GREEN << "/workerConnections= " << this->configData.workerConnections << RESET << std::endl;
    // std::cout << GREEN << "/VECTOR:port= " ;
    // for (std::set<int>::iterator it = this->configData.ports.begin(); it != this->configData.ports.end(); ++it) {
    //     std::cout << *it <<", ";
    // }
    // std::cout << RESET << std::endl;
    // std::cout << "--------vector<t_server>[0]----------" << std::endl;
    // std::cout << GREEN "/serverData[0].listen= " << this->configData.serverData[0].listen << RESET << std::endl;
    // std::cout << GREEN << "/serverData[0].server_name= " ;
    // for (std::vector<std::string>::iterator it = this->configData.serverData[0].server_name.begin(); it != this->configData.serverData[0].server_name.end(); ++it) {
    //     std::cout << *it <<", ";
    // }
    // std::cout << RESET << std::endl;

    // std::cout << "--------vector<t_server>[1]----------" << std::endl;
    // std::cout << GREEN "/serverData[1].listen= " << this->configData.serverData[1].listen << std::endl;
    // std::cout << GREEN << "/serverData[1].server_name= " ;
    // for (std::vector<std::string>::iterator it = this->configData.serverData[1].server_name.begin(); it != this->configData.serverData[1].server_name.end(); ++it) {
    //     std::cout << *it <<", ";
    // }
    // std::cout << RESET << std::endl;
    // Cierra el archivo
    file.close();
    return 0;
}