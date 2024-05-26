/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 17:28:48 by vcereced          #+#    #+#             */
/*   Updated: 2024/05/26 17:01:00 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

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
	RequestParser(std::string);
	RequestParser(RequestParser const &src);
	RequestParser &operator=(RequestParser const &src);

	//destructor
	~RequestParser() {};

	int loadConfigFromRequest(const std::string );
	void showConfig(void);

	//setter
	void set(std::string, std::string);
	
	//getter
	std::map<std::string, std::string> &get(void);

private:
    std::map<std::string, std::string> _requestData;
	// std::string _URI;
	// std::string _method;
};

#endif  // RequestParse_HPP 