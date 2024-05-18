/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Master.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgarizad <dgarizad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 16:11:34 by dgarizad          #+#    #+#             */
/*   Updated: 2024/05/19 00:35:45 by dgarizad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MASTER_HPP
# define MASTER_HPP
# define MAX_CLIENTS 100
#include "webserv.hpp"

class VHost;

class Master
{
	public:
		Master();
		~Master();
		Master(Master const &src);
		Master &operator=(Master const &src);

		int createVHosts(FileParse &config);
		int setSockets(std::vector<int> ports);
		int setEvents();
		int startEventLoop();
		
		
	private:

		std::vector<int> _ListenSockets;
		std::vector<VHost> _vhosts;
		std::map<std::string, VHost> _vhostMap;
		int _kq;
};


#endif