/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Phonebook.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:09:34 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/20 17:09:34 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PhoneBook.hpp"
#include <iostream>
#include <iomanip>
#include <limits>

PhoneBook::PhoneBook()
    : _contacts(), _nextSlot(0), _nbContacts(0)
{}

void PhoneBook::addContact()
{
    Contact &c = _contacts[_nextSlot];
    c.fill();
    if (!c.isEmpty()) {
        // advance nextSlot in circular buffer
        _nextSlot = (_nextSlot + 1) % 8;
        if (_nbContacts < 8)
            ++_nbContacts;
        std::cout << "✔ Contact saved!\n";
    }
}

void PhoneBook::searchContact() const
{
    if (_nbContacts == 0) {
        std::cout << "PhoneBook is empty.\n";
        return;
    }

    // Header
    std::cout
        << std::setw(10) << "Index"      << '|'
        << std::setw(10) << "First name" << '|'
        << std::setw(10) << "Last name"  << '|'
        << std::setw(10) << "Nickname"   << '\n';

    // Rows
    for (int i = 0; i < _nbContacts; ++i)
        _contacts[i].displayInline(i);

    // Select one
    std::cout << "Index to display > ";
    int idx;
    if (!(std::cin >> idx)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input!\n";
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (idx < 0 || idx >= _nbContacts)
        std::cout << "Index out of range.\n";
    else
        _contacts[idx].displayFull();
}
