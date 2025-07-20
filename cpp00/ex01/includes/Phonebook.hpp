/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Phonebook.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:09:06 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/20 17:09:06 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHONEBOOK_HPP
#define PHONEBOOK_HPP

#include "Contact.hpp"
#include <iostream>
#include <iomanip>
#include <limits>

class PhoneBook {
public:
    PhoneBook();

    // Prompt for a new contact and store it (overwrite oldest if full)
    void addContact();

    // Show a table of saved contacts, then detailed view by index
    void searchContact() const;

private:
    Contact _contacts[8];
    int     _nextSlot;    // next index to write (0–7)
    int     _nbContacts;  // how many are filled (≤8)
};

#endif
