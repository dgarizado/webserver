/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:41:19 by dgarizad          #+#    #+#             */
/*   Updated: 2024/06/21 12:27:47 by vcereced         ###   ########.fr       */
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
	std::string path;
	
	if (argc > 2)
		return (ft_error("Bad arguments, use: ./webserv [config_file]"));
		
	try{
		if (argc == 2 )
			path = argv[1];
		else
			path = "./config/default.com";
			
		std::cout << GREEN << "Parsing configuration file: " << path << RESET<< std::endl;
		fileParser.loadConfigFromFile(path);
	} catch (std::exception &e){
		return ft_error("Exception: main: "+ std::string(e.what()));
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