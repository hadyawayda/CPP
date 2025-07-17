/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contact.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 20:26:57 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/17 20:26:57 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Contact.hpp"

Contact::Contact() {}

std::string Contact::_readNonEmpty(const std::string& prompt)
{
    std::string input;
    do {
        std::cout << prompt;
        if (!std::getline(std::cin, input))
            return std::string(); // EOF
    } while (input.empty());
    return input;
}

void Contact::fill(void)
{
    _firstName = _readNonEmpty("First name      : ");
    _lastName  = _readNonEmpty("Last name       : ");
    _nickname  = _readNonEmpty("Nickname        : ");
    _phone     = _readNonEmpty("Phone number    : ");
    _secret    = _readNonEmpty("Darkest secret  : ");
}

bool Contact::isEmpty(void) const { return _firstName.empty(); }

std::string Contact::_formatField(const std::string& str)
{
    if (str.length() > 10)
        return str.substr(0, 9) + ".";
    return str;
}

void Contact::displayInline(int idx) const
{
    std::cout << std::setw(10) << idx << '|'
              << std::setw(10) << _formatField(_firstName) << '|'
              << std::setw(10) << _formatField(_lastName)  << '|'
              << std::setw(10) << _formatField(_nickname)  << std::endl;
}

void Contact::displayFull(void) const
{
    std::cout << "First name     : " << _firstName << std::endl;
    std::cout << "Last name      : " << _lastName  << std::endl;
    std::cout << "Nickname       : " << _nickname  << std::endl;
    std::cout << "Phone number   : " << _phone     << std::endl;
    std::cout << "Darkest secret : " << _secret    << std::endl;
}
