/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Contact.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:08:54 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/31 21:13:44 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Contact.hpp"

Contact::Contact() {}

std::string Contact::getFirstName()
{
	return (this->_firstName);
}

void Contact::setFirstName(std::string firstName)
{
	this->_firstName = firstName;
}

std::string Contact::getLastName()
{
	return (this->_lastName);
}

void Contact::setLastName(std::string lastName)
{
	this->_lastName = lastName;
}

std::string Contact::getNickname()
{
	return (this->_nickname);
}

void Contact::setNickname(std::string nicknam)
{
	this->_nickname = nicknam;
}

std::string Contact::getPhone()
{
	return (this->_phone);
}

void Contact::setPhone(std::string phone)
{
	this->_phone = phone;
}

std::string Contact::getSecret()
{
	return (this->_secret);
}

void Contact::setSecret(std::string secret)
{
	this->_secret = secret;
}
