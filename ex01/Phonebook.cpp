/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phonebook.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 20:28:47 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/17 20:28:47 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Phonebook.hpp"

PhoneBook::PhoneBook() : _nextSlot(0), _nbContacts(0) {}

void PhoneBook::addContact(void)
{
    Contact &c = _contacts[_nextSlot];
    c.fill();
    if (!c.isEmpty())
    {
        _nextSlot = (_nextSlot + 1) % 8;
        if (_nbContacts < 8)
            ++_nbContacts;
        std::cout << "✔ Contact saved!" << std::endl;
    }
}

void PhoneBook::searchContact(void) const
{
    if (_nbContacts == 0)
    {
        std::cout << "PhoneBook is empty." << std::endl;
        return;
    }
    std::cout << std::setw(10) << "Index" << '|'
              << std::setw(10) << "First name" << '|'
              << std::setw(10) << "Last name"  << '|'
              << std::setw(10) << "Nickname"   << std::endl;
    for (int i = 0; i < _nbContacts; ++i)
        _contacts[i].displayInline(i);

    std::cout << "Index to display > ";
    int idx = -1;
    if (!(std::cin >> idx))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input!" << std::endl;
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (idx < 0 || idx >= _nbContacts)
        std::cout << "Index out of range." << std::endl;
    else
        _contacts[idx].displayFull();
}
