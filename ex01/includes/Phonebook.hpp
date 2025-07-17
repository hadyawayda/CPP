/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phonebook.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 20:19:33 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/17 20:19:33 by hawayda          ###   ########.fr       */
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

    void addContact();

    void searchContact() const;

private:
    Contact _contacts[8];
    int     _nextSlot;
    int     _nbContacts;
};

#endif
