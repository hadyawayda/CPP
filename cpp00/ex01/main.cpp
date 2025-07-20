/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:09:52 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/20 17:09:52 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PhoneBook.hpp"

int main()
{
    PhoneBook pb;
    std::string cmd;

    while (true)
    {
        std::cout << "Enter command (ADD, SEARCH, EXIT): ";
        if (!std::getline(std::cin, cmd))
            break;  // EOF

        if (cmd == "ADD")
            pb.addContact();
        else if (cmd == "SEARCH")
            pb.searchContact();
        else if (cmd == "EXIT")
            break;
        // otherwise ignore and re-prompt
    }

    return 0;
}
