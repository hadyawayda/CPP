/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:09:52 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/31 21:38:49 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Phonebook.hpp"

int	main(void)
{
	PhoneBook	pb;
	std::string cmd;
	while (true)
	{
		std::cout << "Enter command (ADD, SEARCH, EXIT): "<<std::endl;
		if (!std::getline(std::cin, cmd))
			break ;
		if (cmd == "ADD")
			pb.add();
		else if (cmd == "SEARCH")
        {
            pb.display();
			pb.search();
        }
		else if (cmd == "EXIT")
			break ;
	}
	return (0);
}
