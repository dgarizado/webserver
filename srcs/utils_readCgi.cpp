/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_readCgi.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 19:19:13 by vcereced          #+#    #+#             */
/*   Updated: 2024/09/25 20:27:05 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"
#include <vector>

pid_t   pid;

void dupAndCloseFd(int *pipefd)
{
    close(pipefd[0]);
    
    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
        std::cerr << "dup2 failed: " << std::endl;
        close(pipefd[1]);
        _exit(EXIT_FAILURE);
    }
    close(pipefd[1]);
}

void handle_alarm(int sig) {
    std::cout << "Signal SIGALRM received:"<< sig <<"kill proccess: " << pid << " by signal " << SIGTERM << std::endl;
    kill(pid, SIGTERM);
}


std::string readOutputCgi(std::string cgi, std::string filePath, std::string file)//pendiente
{
    // std::array<char, CGI_BUFFER_SIZE>  buffer;
    std::vector<char>        buffer(CGI_BUFFER_SIZE);
    std::string             result;
    ssize_t                 bytesRead;
    int                     pipefd[2];
    int                     status;
    
    signal(SIGALRM, handle_alarm);

    if (pipe(pipefd) == -1)
        throw ServerException("readOutputCgi: cannot create pipe:  " + std::string(strerror(errno)), INTERNAL_SERVER_ERROR);

    pid = fork();
    if (pid == -1)
    {
        close(pipefd[1]);
        close(pipefd[0]);
        throw ServerException("readOutputCgi: fork failed:  " + std::string(strerror(errno)), INTERNAL_SERVER_ERROR); 
    }
    
    if (pid == 0)// Child process
    { 
        std::vector<std::string> args;  // arguments to execve
        args.push_back(file);
        args.push_back(filePath);
        char** argv = convertToCharArray(args);

        dupAndCloseFd(pipefd);
        execve(cgi.c_str(), argv, environ);
        
        _exit(EXIT_FAILURE);
    }
    else // Father process
    { 
        alarm(1); // Establecer un tiempo de espera de 2 segundos
        int block =  waitpid(pid, &status, 0);
        block = block + 0;
        alarm(0);
        
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0 ) {
            std::cout << "Cgi in subprocess finished: exit status: " << WEXITSTATUS(status) << std::endl;

            close(pipefd[1]);
            while ((bytesRead = read(pipefd[0], &buffer[0], buffer.size())) > 0)
                result.append(&buffer[0], bytesRead);
            // while ((bytesRead = read(pipefd[0], buffer.data(), buffer.size())) > 0)
            //     result.append(buffer.data(), bytesRead);
            close(pipefd[0]);
            
        } else if (WIFEXITED(status) && WEXITSTATUS(status) != 0 ) {
            waitpid(pid, &status, 0); // Esperar a que termine después de matarlo
            throw ServerException("readOutputCgi: Cgi in subprocess finished suddenly: unexpected error", INTERNAL_SERVER_ERROR);
        } else if (WIFSIGNALED(status)) {
            waitpid(pid, &status, 0);
            std::stringstream ss;
            ss << WTERMSIG(status);
            throw ServerException("readOutputCgi: Cgi in subprocess finished by signal: " + ss.str(), INTERNAL_SERVER_ERROR);
        } 
            // throw ServerException("readOutputCgi: Cgi in subprocess finished by signal: " + std::to_string(WTERMSIG(status)), INTERNAL_SERVER_ERROR);}
        

        return result;
    }
}
