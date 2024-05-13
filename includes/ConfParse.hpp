/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfParse.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 12:12:00 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/13 17:27:18 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFPARSE_HPP
# define CONFPARSE_HPP
# include "webserv.hpp"

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
		int serverQty;
};

#endif