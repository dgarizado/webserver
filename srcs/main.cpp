/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:41:19 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/16 19:06:11 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"
#include "../includes/ConfParse.hpp"
#include "../includes/Master.hpp"

int createPorts(std::vector<int> &ports)
{
	ports.push_back(8084);
	ports.push_back(8083);
	ports.push_back(8082);
	ports.push_back(8081);
	ports.push_back(8085);
	return (0);
}

int main(int argc, char **argv)
{
	// argv = NULL;
	if (argc > 2)
		ft_error("Bad arguments, use: ./webserv [config_file]");
	if (argc == 2)
	{
		//HERE COMES PARSING AND LEXING BY VICTOR
	} 
	else
	{
		// ConfParse();
		std::cout << GREEN << "Usign Default config file: ./config/default.conf"<< RESET<< std::endl;
	}
	// 1. Parse the configuration file
		std::string config_file(argv[1]);
		std::cout << GREEN << "Parsing configuration file" << RESET << std::endl;
		ConfParse conf(config_file);
		if (!conf.readConfigFile())
		 	ft_error("Error reading config file");
	//2. Create Vhost objects based on the configuration.
	//3. Create Master object and pass the Vhost objects to it
		
		std::vector<int> ports;
		createPorts(ports);
		Master master;
		master.setSockets(ports);
		master.setEvents();
		master.startEventLoop();
	return (0);
}