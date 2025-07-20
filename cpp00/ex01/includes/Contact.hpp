/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Contact.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:06:52 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/20 17:06:52 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <string>
#include <limits>
#include <iostream>
#include <iomanip>

class Contact {
public:
    Contact();

    // Prompt user until a non-empty line is read for each field
    void        fill();

    // True if this contact was never filled
    bool        isEmpty() const;

    // Print one line (idx | 10-char first | 10-char last | 10-char nick)
    void        displayInline(int idx) const;

    // Print all five fields, one per line
    void        displayFull() const;

private:
    std::string _firstName;
    std::string _lastName;
    std::string _nickname;
    std::string _phone;
    std::string _secret;

    static std::string _readNonEmpty(const std::string& prompt);
    static std::string _formatField(const std::string& str);
};

#endif
