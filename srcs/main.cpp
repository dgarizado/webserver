/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:41:19 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/19 18:21:46 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

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
	FileParse fileParser;
	Master master;
	
	if (argc > 2)
		return (ft_error("Bad arguments, use: ./webserv [config_file]"));
	if (argc == 2 )
	{
		std::cout << GREEN << "Parsing configuration file" << RESET << std::endl;
		if (fileParser.loadConfigFromFile(argv[1]) == -1)
			return (ft_error("Error while parsing the configuration file"));
	} 
	else
	{
		std::cout << GREEN << "Usign Default config file: ./config/default.conf"<< RESET<< std::endl;
		if (fileParser.loadConfigFromFile("./config/default.com") == -1)
			return (ft_error("Error while parsing the configuration file"));
	}
	//1. Parse the configuration file
	//QUESTIONS: 
	// 			 Q1:WHAT HAPPENS IF A SERVERBLOCK HAS MULTIPLE SERVER NAMES?
	//			 Q2:WHAT HAPPENS IF TWO SERVER BLOCKS HAVE THE SAME SERVER NAME?:
	// 			 A2: THE LAST ONE WILL BE THE ONE USED
	//			 - WHAT HAPPENS IF A SERVER BLOCK HAS NO SERVER NAME?
	//2. Create Vhost objects based on the configuration. check
	//3. Create Master object and pass the Vhost objects to it check
		
	std::vector<int> ports;
	createPorts(ports);
	fileParser.showConfig();
	master.createVHosts(fileParser);
	// master.printServerNames();:
	VHost &vhost = master.getVHost("www.domain2.com");
	std::cout << "VHOST SERVER NAME: " << vhost.getServerStruct().listen << std::endl;
	master.setSockets(ports);
	master.setEvents();
	master.startEventLoop();
	return (0);
}