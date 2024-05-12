/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParse.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 12:13:57 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/12 13:06:50 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ConfParse.hpp"

ConfParse::ConfParse()
{
	std::cout << "Default config constructor done" << std::endl;
}

ConfParse::ConfParse(std::string const &config_file) : _config_file(config_file)
{
	std::cout << GREEN << "Analizing config file..." << RESET << std::endl;
}

ConfParse::~ConfParse()
{
}

ConfParse::ConfParse(ConfParse const &src)
{
	*this = src;
}

ConfParse &ConfParse::operator=(ConfParse const &src)
{
	if (this != &src)
	{
		this->_config_file = src._config_file;
	}
	return (*this);
}

bool ConfParse::readConfigFile() const
{
	std::ifstream file(this->_config_file.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file " << this->_config_file << std::endl;
		return (false);
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string parsed = buffer.str();
	//now print the parsed string
	std::cout << parsed << std::endl;
	file.close();
	return (true);
}