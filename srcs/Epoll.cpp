#include "../includes/Master.hpp"
#include <sys/epoll.h>
#include <fcntl.h>
#include <iostream>

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

int Master::startEventLoop()
{
    const int MAX_EVENTS = 64;
    struct epoll_event events[MAX_EVENTS];

    while (true)
    {
        int nev = epoll_wait(_epoll_fd, events, MAX_EVENTS, -1);
        if (nev == -1)
            ft_error("Error in epoll_wait");

        for (int i = 0; i < nev; ++i)
        {
            int socketToAccept = events[i].data.fd;

            bool isListenSocket = false;
            std::vector<int>::iterator it = _ListenSockets.begin();
            for (; it != _ListenSockets.end(); it++)
            {
                if (socketToAccept == *it)
                {
                    isListenSocket = true;
                    break;
                }
            }
            if (isListenSocket)
            {
                sockaddr_in clientAddr;
                socklen_t clientAddrSize = sizeof(clientAddr);
                int clientSocket = accept(socketToAccept, (struct sockaddr *)&clientAddr, &clientAddrSize);
                if (clientSocket < 0)
                    ft_error("Error accepting connection");
                std::cout << GREEN << "Accepted connection on socket " << socketToAccept << " from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << RESET << std::endl;

                // // Set client socket to non-blocking
                // fcntl(clientSocket, F_SETFL, O_NONBLOCK);

                struct epoll_event ev;
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = clientSocket;
                if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, clientSocket, &ev) == -1)
                    ft_error("Error adding client socket to epoll");
            }
            else
            {
                char buffer[1024];
                int bytesRead = read(socketToAccept, buffer, 1024);

                //HERE COMES TEST WITH REQUEST PARSER !!!!!!!!!!!!!!!!!!!!!
                
                Request request(buffer);
                request.showConfig();

                if (bytesRead < 0)
                    ft_error("Error reading from socket");
                if (bytesRead == 0)
                    ft_error("Client disconnected");
                else
                {
                    buffer[bytesRead] = '\0';
                    std::cout << "Received: " << buffer << std::endl;

                    // SERVING the ./html/index.html file
                    std::ifstream file("./html/index.html");
                    std::stringstream bufferr;
                    bufferr << file.rdbuf();
                    std::string file_contents = bufferr.str();
                    const char *response_headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
                    send(socketToAccept, response_headers, strlen(response_headers), 0);
                    send(socketToAccept, file_contents.c_str(), file_contents.size(), 0);
                    close(socketToAccept);
                    std::cout << GREEN "Response sent" RESET << std::endl;
                }
            }
        }
    }
    return (0);
}
