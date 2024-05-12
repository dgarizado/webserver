/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:41:19 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/12 13:06:21 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"
#include "../includes/ConfParse.hpp"

int main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cerr << RED << "Usage: ./webserver [config_file]" << RESET <<std::endl;
		return (1);
	}
	if (argc == 2)
	{
		
		std::string config_file(argv[1]);
		ConfParse conf(config_file);
		if (!conf.readConfigFile())
		{
			return (1);
		}
		return (0);
	} 
	else
	{
		std::cout << GREEN << "Usign Default config file: ./config/default.conf"<< RESET<< std::endl;
		//USE DEFAULT CONFIG FILE
	}
	return (0);
}