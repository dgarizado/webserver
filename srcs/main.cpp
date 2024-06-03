/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:41:19 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/03 19:40:47 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"
#include "../includes/Master.hpp"
#include "../includes/FileParse.hpp"
#include "../includes/VHost.hpp"


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
	fileParser.showConfig();
	master.createVHosts(fileParser);
	// master.printServerNames();:
	// VHost &vhost = master.getVHost("www.domain2.com");
	// std::cout << "VHOST SERVER NAME: " << vhost.getServerStruct().listen << std::endl;
	master.setSockets(fileParser.getStruct().ports);
	master.setEvents();
	master.startEventLoop();
	return (0);
}