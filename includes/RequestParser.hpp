/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 17:28:48 by vcereced          #+#    #+#             */
/*   Updated: 2024/06/21 10:19:19 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP
# include "webserv.hpp"
# include <string>
# include <iostream>
# include <sstream>
# include <exception>
# include <map>

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define RESET "\033[0m"

class RequestParser {
public:
	//Constructors
	RequestParser(void) {}
	RequestParser(RequestParser const &src);
	RequestParser &operator=(RequestParser const &src);

	//destructor
	~RequestParser() {};

	void loadConfigFromRequest(const std::string );
	void showConfig(void);

	//setter
	void set(std::string, std::string);
	
	//getter
	std::map<std::string, std::string> &get(void);

private:
    std::map<std::string, std::string> _requestData;
};

#endif  // RequestParse_HPP 