/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParse.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 12:12:00 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/16 18:27:01 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFPARSE_HPP
# define CONFPARSE_HPP
# include "webserv.hpp"
// # include "VHOST.hpp"

class ConfParse
{
	public:
		ConfParse();
		ConfParse(std::string const &config_file);
		~ConfParse();
		ConfParse(ConfParse const &src);
		ConfParse &operator=(ConfParse const &src);

		bool readConfigFile() const;
	private:
		std::string _config_file;
		// std::vector<VHOST> _vhosts;
		std::set<int> _ports;
		// int serverQty;
};

#endif