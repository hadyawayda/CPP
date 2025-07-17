/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contact.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 20:17:24 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/17 20:17:24 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <string>
#include <iostream>
#include <iomanip>

class Contact {
public:
    Contact();

    void        fill();

    bool        isEmpty() const;

    void        displayInline(int index) const;

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
