#include "../includes/Master.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctime>
#include <sys/stat.h>
#include <exception>


int Master::setSockets(std::set<int> &ports)
{
	std::set<int>::iterator it = ports.begin();
	for (; it != ports.end(); ++it)
	{
		int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSocket < 0)
			ft_error("Error creating socket");

		// Set socket to non-blocking
		fcntl(serverSocket, F_SETFL, O_NONBLOCK);

		struct sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(*it);
		serverAddr.sin_addr.s_addr = INADDR_ANY;

		if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
			ft_error("Error binding socket");

		if (listen(serverSocket, 10) < 0)
			ft_error("Error listening on socket");

		_ListenSockets.push_back(serverSocket);
		std::cout << GREEN << "Socket " << serverSocket << " listening on port " << *it << RESET << std::endl;
	}
	return (0);
}

/**
 * @brief Creates the epoll set and adds the server sockets to it.
 * epoll_create1(0) creates a new epoll instance.
 * @return int 
 */
int Master::setEvents()
{
    _epoll_fd = epoll_create1(0);
    if (_epoll_fd == -1)
        ft_error("Error creating epoll");

    std::vector<int>::iterator it = _ListenSockets.begin();
    for (; it != _ListenSockets.end(); it++)
    {
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLET; // Edge-triggered mode
        ev.data.fd = *it;
        if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, *it, &ev) == -1)
            ft_error("Error adding socket to epoll");
    }
    return (0);
}

/**
 * @brief Accepts a connection on a server socket and adds the client socket to the epoll set.
 * Creates a Client object and adds it to the _clientsMap.
 * 
 * @param socketToAccept 
 * @return int 
 */
int Master::clientAccept(int socketToAccept)
{
    Connection client;
	sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    
    int clientSocket = accept(socketToAccept, (struct sockaddr *)&clientAddr, &clientAddrSize);
    if (clientSocket < 0)
       return(ft_error("Error accepting connection"));
    std::cout << GREEN << "Accepted connection on socket " << socketToAccept << " from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << RESET << std::endl;

    // Set client socket to non-blocking
    fcntl(clientSocket, F_SETFL, O_NONBLOCK);
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = clientSocket;
    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, clientSocket, &ev) == -1)
        return(ft_error("Error adding client socket to epoll"));

    _clientSockets.push_back(clientSocket);
    client.setClientData(clientSocket, clientAddr, clientAddrSize, ev);
    _clientsMap[clientSocket] = client;
    return (0);
}

/**
 * @brief Here we read from the client socket and store the buffer in the Client object.
 * 
 * @param clientSocket 
 * @return int 
 */
int Master::clientRead(int clientSocket)
{
    char buffer[1024];
    int bytesRead = read(clientSocket, buffer, 1024);
    if (bytesRead < 0)
        return(ft_error("Error reading from socket")); //REMOVE CLIENT SOCKET FROM EPOLL SET AND CLOSE SOCKET!
    if (bytesRead == 0)
    {
        //Remove the client socket from the epoll set, close the socket and remove it from the clientSockets vector
        epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, clientSocket, NULL);
        close(clientSocket);
        _clientSockets.erase(std::remove(_clientSockets.begin(), _clientSockets.end(), clientSocket), _clientSockets.end());
        // ft_error("Client disconnected"); //Is this an error? or just a message? ... maybe a message. HANDLE IT!!!
        std::cout << RED << "Client disconnected" << RESET << std::endl;
        return (1); // This is not an error, it's just a message HANDLE THIS!!!
    }
    else //store the buffer in the Client object
    {
        buffer[bytesRead] = '\0';
        _clientsMap[clientSocket].setBuffer(buffer);
    }
    return (0);
}

/*******************************************************************************/


std::string genRowName(struct dirent *entry)
{
    std::string html;
    
    if (entry->d_type == DT_DIR)
        html = html + "<a href=\"" + entry->d_name + "\\\">" + entry->d_name + "\\" + "</a>";
    else if (entry->d_type == DT_REG)
        html = html + "<a href=" + "\"" + entry->d_name + "\">" + entry->d_name + "</a>";
    html += "</td>";

    return html;
}


std::string genDescription(struct stat fileStat)
{
    std::string html;

    html += (fileStat.st_mode & S_IRUSR) ? "r" : "-";
    html += (fileStat.st_mode & S_IWUSR) ? "w" : "-";
    html += (fileStat.st_mode & S_IXUSR) ? "x" : "-";
    html += (fileStat.st_mode & S_IRGRP) ? "r" : "-";
    html += (fileStat.st_mode & S_IWGRP) ? "w" : "-";
    html += (fileStat.st_mode & S_IXGRP) ? "x" : "-";
    html += (fileStat.st_mode & S_IROTH) ? "r" : "-";
    html += (fileStat.st_mode & S_IWOTH) ? "w" : "-";
    html += (fileStat.st_mode & S_IXOTH) ? "x" : "-";

    return html;
}

std::string genRowLastMod(struct stat fileStat)
{
    std::string html;
    std::time_t modTime;
    struct tm   *timeinfo;
    char        buffer[30];

    modTime = fileStat.st_mtime;
    timeinfo = localtime(&modTime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    html = buffer;

    return html;
}

std::string genRowSize(struct stat fileStat)
{
    std::string html;
    off_t       fileSize;

    fileSize = fileStat.st_size;
    html = std::to_string(fileSize) + " B";

    return html;
}

std::string genRowAutoIndex(struct dirent *entry, std::string filePath)
{
    std::string html;
    struct stat fileStat;

    if (stat(filePath.c_str(), &fileStat) < 0)
        throw std::runtime_error(filePath);

    html += "<tr>";
    html += "<td>";
    html += genRowName(entry);
    html += "</td><td>";
    html += genRowLastMod(fileStat);
    html += "</td><td>";
    html += genRowSize(fileStat);
    html += "</td><td>";
    html += genDescription(fileStat);
    html += "</tr>";

    return html;
}

std::string genRowsAutoIndex(std::string path)
{
    std::string     html;
    struct dirent   *entry;
    DIR             *dp = opendir(path.c_str());

    if (dp == NULL) {
        perror("opendir");
        return "fallo estupido humano";
    }

    while ((entry = readdir(dp)))
    {
        try {
            html += genRowAutoIndex(entry, path + entry->d_name);
        } catch (const std::exception &e) {
            std::cerr << "Excepción: stats of file: " << e.what() << '\n';
        }
    }
    closedir(dp);

    return html;
}

std::string genAutoIndex(std::string path)
{
    std::string response_header;
    std::string response_body_begin;
    std::string response_body_middle;
    std::string response_body_end;

    response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    response_body_begin = R"(
    <!DOCTYPE html>
    <html>
    <head>
        <title>Index of /directorio-listado/</title>
        <style>
            body {
                font-family: Arial, sans-serif;
            }
            table {
                width: 100%;
                border-collapse: collapse;
            }
            th, td {
                padding: 8px;
                text-align: left;
                border-bottom: 1px solid #ddd;
            }
            th {
                background-color: #f2f2f2;
            }
            a {
                color: #0066cc;
                text-decoration: none;
            }
            a:hover {
                text-decoration: underline;
            }
        </style>
    </head>
    <body>
        <h1>Index of /directorio-listado/</h1>
        <table>
            <tr>
                <th>Name</th>
                <th>Last modified</th>
                <th>Size</th>
                <th>Permisions</th>
            </tr>)";
    response_body_middle = genRowsAutoIndex(path);
    response_body_end = R"(
            </table>
        </body>
    </html>)";

    return response_header + response_body_begin + response_body_middle + response_body_end;
}

std::string genRelativeRoute(std::string uri)
{
    return "." + uri;
}

/***************************************************************************************************/

int Master::manageConnection(int connectionSocket)
{
    RequestParser request;
    //read from the socket
    if (clientRead(connectionSocket) < 0)
        return ft_error("Error reading from client");
    //request = _clientsMap[connectionSocket].getBuffer();
    //parse the request
    if (request.loadConfigFromRequest(_clientsMap[connectionSocket].getBuffer()) < 0)
        return ft_error("Error loading config from request");
    request.showConfig();
    VHost tmp = assignVHost(request.get()["HTTP_HOST"]);
    _clientsMap[connectionSocket].setVhost(tmp);
    //if (processRequest(connectionSocket, request) < 0)
    //    return ft_error("Error processing request");

    /***************************************************************************************************************/

    std::string route = genRelativeRoute(request.get()["REQUEST_URI"]);
    std::cout << YELLOW << route << RESET << std::endl;

    std::string msg = genAutoIndex(route);
    send(connectionSocket, msg.c_str(), msg.size(), 0);
    close(connectionSocket);

    /*****************************************************************************************************************/
    //create a response
    //send the response
    //close the connection
    return 0;
}
/**
 * @brief Main event loop. If a server socket receives a connection, it will call clientAccept. 
 * If a client socket is ready to read, it will call clientRead.
 * 
 * @return int 
 */
int Master::startEventLoop()
{
    const int MAX_EVENTS = 64;
    struct epoll_event events[MAX_EVENTS];

    while (true)
    {
        int nev = epoll_wait(_epoll_fd, events, MAX_EVENTS, 300); 
        if (nev == -1)
            ft_error("Error in epoll_wait");

        for (int i = 0; i < nev; ++i)
        {
            int socketToAccept = events[i].data.fd;

            if (std::find(_ListenSockets.begin(), _ListenSockets.end(), socketToAccept) != _ListenSockets.end())
            {
                if (clientAccept(socketToAccept) < 0)
                    ft_error("Error accepting client");
            } else //  A client socket is ready to read
            {
                try {
                    manageConnection(socketToAccept);
                } catch (std::exception &e) {
                    std::cerr << e.what() << std::endl;
                    close(socketToAccept);
                    _clientSockets.erase(std::remove(_clientSockets.begin(), _clientSockets.end(), socketToAccept), _clientSockets.end());
                }
            }
            // check for a write event
            // if (events[i].events & EPOLLOUT)
            // {
            //     // write to the client
            // }
        }
    }
    return (0);
}
