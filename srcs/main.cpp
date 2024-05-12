/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:41:19 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/11 21:50:46 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserver.hpp"

int main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cerr << "Usage: ./webserver [config_file]" << std::endl;
		return (1);
	}
	//first we will read the config file and store it in a string parsed.
	if (argc == 2)
	{
		std::string config_file = argv[1];
		std::ifstream file(config_file.c_str());
		if (!file.is_open())
		{
			std::cerr << "Error: could not open file " << config_file << std::endl;
			return (1);
		}
		std::stringstream buffer;
		buffer << file.rdbuf();
		std::string parsed = buffer.str();
		file.close();
	}
	return (0);
}