/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcereced <vcereced@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 17:28:48 by vcereced          #+#    #+#             */
/*   Updated: 2024/05/25 20:54:28 by vcereced         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

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

class Request {
public:
	//Constructors
	Request(void) {}
	Request(std::string);
	Request(Request const &src);
	Request &operator=(Request const &src);

	//destructor
	~Request() {};

	int loadConfigFromRequest(const std::string );
	void showConfig(void);

	//setter
	void set(std::string, std::string);
	
	//getter
	std::map<std::string, std::string> &get(void);

private:
    std::map<std::string, std::string> _requestData;
};

#endif  // RequestParse_HPP 