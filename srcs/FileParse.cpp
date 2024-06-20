#include "../includes/FileParse.hpp"


int ft_err(std::string str)
{
    std::cerr << RED << str << RESET << std::endl;
    return -1;
}

void ft_braces(int &braces, std::string &token)
{
    if (token == "{")
        braces++;
    else if (token == "}")
        braces--;
}

void setDefaultValuesLocation(t_location &ref)
{
    ref.autoIndex               = false;//default value of autoIndex
    ref.allowedMethods[GET]     = true;
    ref.allowedMethods[POST]    = true;
    ref.allowedMethods[PUT]     = true;
    ref.allowedMethods[DELETE]  = true;
}

void setNotAllowedMethod(t_location &ref, std::istringstream &iss)
{
	std::string token;

	while (iss >> token)
	{
		token.erase(std::remove(token.begin(), token.end(), ';'), token.end());

		if 		(token == "GET") 	ref.allowedMethods[GET] 	= false;
    	else if (token == "POST") 	ref.allowedMethods[POST] 	= false;
    	else if (token == "PUT") 	ref.allowedMethods[PUT] 	= false;
   		else if (token == "DELETE") ref.allowedMethods[DELETE] 	= false;
		else throw std::invalid_argument("not valid method " + token);
	}
}

void varLocation(FileParse *ref, std::istringstream &iss, int &nServer, int &nLocation, std::string &token)
{
    if (token == "root" )
    {
        iss >> token; 
        token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
        ref->getStruct().serverData[nServer].locations[nLocation].root = token;
    }
    else if (token == "index")
    {
        while (iss >> token)
        {
            token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
            ref->getStruct().serverData[nServer].locations[nLocation].index.push_back(token);
        }
    }
    else if (token == "autoindex")
    {
        iss >> token;
        token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
        if (token == "on" )
            ref->getStruct().serverData[nServer].locations[nLocation].autoIndex = true;
    }
	else if (token == "limit_except")
		setNotAllowedMethod(ref->getStruct().serverData[nServer].locations[nLocation], iss);
}

void insideLocation(FileParse *ref,std::ifstream &file, std::istringstream &iss, int &nServer, int &nLocation, std::string &token)
{
    int 	        locationBraces = 0;
    std::string     line;
    t_location      empty;

    if (token == "location")
    {
        //push a empty t_location into locations vector
        nLocation++;
        ref->getStruct().serverData[nServer].locations.push_back(empty);
        //register the location
        iss >> token;
        ref->getStruct().serverData[nServer].locations[nLocation].location = token;
        setDefaultValuesLocation(ref->getStruct().serverData[nServer].locations[nLocation]);
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
}

void varServer(FileParse *ref, std::ifstream &file, std::istringstream &iss, int &nServer, int &nLocation, std::string &token, bool *vars)
{
    int port;

    if (token == "listen" && (vars[LISTEN] = true))
    {
        iss >> token;
        token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
        port = std::stoi(token); //if error it throw exception
        ref->getStruct().ports.insert(port);
        ref->getStruct().serverData[nServer].listen = port;
    }
    else if (token == "server_name" && (vars[SERVER_NAME] = true))
    {
       while (iss >> token)
        {
            token.erase(std::remove(token.begin(), token.end(), ';'), token.end());
            ref->getStruct().serverData[nServer].server_name.push_back(token);
        }
    }
    else if (token == "location" && (vars[LOCATION] = true))
        insideLocation(ref, file, iss, nServer, nLocation, token);
}

int insideServer(FileParse *ref, std::ifstream &file, std::istringstream &iss, std::string &token)
{
    static int      nServer = -1;
    int             nLocation = -1;
    int 	        serverBraces = 0;
	int 		    encounter = -1;
    bool            vars[TOTAL_SERVER] = {false, false, false};//to markdown the vars to find
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
                varServer(ref, file, iss, nServer, nLocation, token, vars);
		    }
            if (serverBraces > 0 && std::getline(file, line))
            {
                iss.clear();    
                iss.str(line);
            }
        }while (serverBraces > 0);
        //Checks if vars are encounter inside of server block
        if (vars[LISTEN] == false || vars[SERVER_NAME] == false || vars[LOCATION] == false)
            throw std::runtime_error("Server block wrong");
    }
    return encounter;
}

/**
 * @brief iter if Http block till close the block. if http encounter start to process all 
 * line and its token / braces. if server is encounter it jump into it, if not encounter
 * throw a exception.
 * @return 1 if any http block is encounter. -1 if not.
 */
int insideHttp(FileParse *ref, std::ifstream &file, std::istringstream &iss, std::string &token)
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
                if (insideServer(ref, file, iss, token) == 1)//if server jump into it
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
 * @brief iter the tokens of the line, then call to function according to server block or not.
 */
int lineParser(FileParse *ref, std::ifstream &file, std::string &line)
{
    std::istringstream  iss(line);
    std::string         token;
   
    //Process the first toke of the line
    iss >> token;
    return (insideHttp(ref, file, iss, token));//jump into if "http"
}

/**
 * @brief try to open the file, then iter the characteres of all lines counting the '{' '}'
 * @return int 0 if {} are close proferly, -1 if doesnt match
 */
int checksBraces(std::string filename)
{
    std::ifstream       file(filename);
    std::string         line;
    int                 braces = 0;

    if (!file.is_open())
        throw std::runtime_error("braces: cannot open file");
    while (std::getline(file, line))
    {
        for (size_t i = 0; i < line.size(); ++i)
        {
            if (line[i] == '{')
                braces++;
            else if (line[i] == '}')
                braces--;
        }
    }
    file.close();
    if (braces != 0)
		throw std::runtime_error("braces: not match");
	// if (braces != 0)
    //     return -1;
    return 0;
}

/**
 * @brief Getter the reference of the structure atribute of FileParse 
 * @return reference to t_fileParse of instance 
 */
t_fileParse & FileParse::getStruct()
{
    return this->configData;
}

/**
 * @brief Method to load on ifstream object the full config file. Iter line per line parsing the file
 * @return int -1 cannot open file or any exception. 0 ok.
 */
int FileParse::loadConfigFromFile(const std::string filename)
{
    std::ifstream       file(filename);
    std::string         line;
    bool                http = false;
    
	try {
		checksBraces(filename);
	}catch(const std::exception& e) {
		std::cerr << RED << "Exception: " << e.what() << RESET << std::endl;
        return -1;
	} 

    if (!file.is_open())
        return ft_err("Error: cannasot open file. ");
    while (std::getline(file, line)) 
    {  
		try{
            if (lineParser(this, file, line) == 1)//Parse line per line
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

void FileParse::showConfig(void)
{

    std::cout << "/ports\t\t\t\t= " ;
    for (std::set<int>::iterator it = this->configData.ports.begin(); it != this->configData.ports.end(); ++it) {std::cout << *it << " ";} std::cout << std::endl;
    std::cout << "/serverData/ " << std::endl;
    for (std::vector<t_server>::iterator it = this->configData.serverData.begin(); it != this->configData.serverData.end(); ++it) {
        std::cout << "           /server_name\t\t= " ;
        for (std::vector<std::string>::iterator itVec = it->server_name.begin(); itVec != it->server_name.end(); ++itVec) {std::cout << *itVec << " ";} std::cout << std::endl;
        std::cout << "\t   /listen\t\t= " << it->listen << std::endl;
        for (std::vector<t_location>::iterator itLoc = it->locations.begin(); itLoc != it->locations.end(); ++itLoc) {
            std::cout << "\t   /location/" << std::endl;
            std::cout << "\t\t    /location\t= " << itLoc->location << std::endl;
            std::cout << "\t\t    /root\t= " << itLoc->root << std::endl;
            std::cout << "\t\t    /index\t= ";
            for (std::vector<std::string>::iterator itIndex = itLoc->index.begin(); itIndex != itLoc->index.end(); ++itIndex) {std::cout << *itIndex << " "; } std::cout << std::endl;
            std::cout << "\t\t    /autoindex\t= " << itLoc->autoIndex << std::endl;
            std::cout << "\t\t    /methods\t= " << "GET:" << itLoc->allowedMethods[GET] << " POST:" << itLoc->allowedMethods[POST] << " PUT:" << itLoc->allowedMethods[PUT] << " DELETE:" << itLoc->allowedMethods[DELETE] << std::endl;
        }
    std::cout << "-------------------------------------------------------------------------" << std::endl;
    }
}
