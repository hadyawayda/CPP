/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Contact.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:08:54 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/20 17:08:54 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Contact.hpp"

Contact::Contact()
    : _firstName(), _lastName(), _nickname(), _phone(), _secret()
{}

std::string Contact::_readNonEmpty(const std::string& prompt)
{
    std::string input;
    do {
        std::cout << prompt;
        if (!std::getline(std::cin, input))
            return std::string();  // EOF
    } while (input.empty());
    return input;
}

void Contact::fill()
{
    _firstName = _readNonEmpty("First name      : ");
    _lastName  = _readNonEmpty("Last name       : ");
    _nickname  = _readNonEmpty("Nickname        : ");
    _phone     = _readNonEmpty("Phone number    : ");
    _secret    = _readNonEmpty("Darkest secret  : ");
}

bool Contact::isEmpty() const
{
    return _firstName.empty();
}

std::string Contact::_formatField(const std::string& str)
{
    if (str.length() > 10)
        return str.substr(0, 9) + ".";
    return str;
}

void Contact::displayInline(int idx) const
{
    std::cout
        << std::setw(10) << idx << '|'
        << std::setw(10) << _formatField(_firstName) << '|'
        << std::setw(10) << _formatField(_lastName)  << '|'
        << std::setw(10) << _formatField(_nickname)  << '\n';
}

void Contact::displayFull() const
{
    std::cout
        << "First name     : " << _firstName << '\n'
        << "Last name      : " << _lastName  << '\n'
        << "Nickname       : " << _nickname  << '\n'
        << "Phone number   : " << _phone     << '\n'
        << "Darkest secret : " << _secret    << '\n';
}
