/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Phonebook.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:09:34 by hawayda           #+#    #+#             */
/*   Updated: 2025/08/01 00:34:34 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Phonebook.hpp"
#include <iomanip>
#include <iostream>
#include <limits>

PhoneBook::PhoneBook()
{
}

int PhoneBook::counter = -1;

void PhoneBook::add(void)
{
	Contact	c;
	bool	isDigit;
	int		index;

	counter++;
	std::string firstName;
	std::string lastName;
	std::string nickname;
	std::string phone;
	std::string secret;
	std::cout << "Enter first name: ";
	if (!std::getline(std::cin, firstName))
		return ;
	std::cout << "Enter last name: ";
	if (!std::getline(std::cin, lastName))
		return ;
	std::cout << "Enter nickname: ";
	if (!std::getline(std::cin, nickname))
		return ;
	isDigit = true;
	while (true)
	{
		isDigit = true;
		std::cout << "Enter phone number: ";
		if (!std::getline(std::cin, phone))
			return ;
		for (int i = 0; i < (int)phone.size(); i++)
		{
			if (!isdigit(phone[i]))
			{
				isDigit = false;
				break ;
			}
		}
		if (isDigit)
			break ;
		else
			std::cout << "only numbers are allowed in phone" << std::endl;
	}
	std::cout << "Enter secret: ";
	if (!std::getline(std::cin, secret))
		return ;
	c.setFirstName(firstName);
	c.setLastName(lastName);
	c.setNickname(nickname);
	c.setPhone(phone);
	c.setSecret(secret);
	index = counter % 8;
	_contacts[index] = c;
}

std::string formatString(std::string str)
{
	if (str.length() > 10)
		return (str.substr(0, 9) + ".");
	return (std::string(10 - str.length(), ' ') + str);
}

void PhoneBook::display()
{
	int	total;

	if (counter == -1)
		return ;
	total = (counter < 8 ? counter + 1 : 8);
	std::cout << std::setw(10) << "Index"
				<< "|";
	std::cout << std::setw(10) << "First name"
				<< "|";
	std::cout << std::setw(10) << "Last name"
				<< "|";
	std::cout << std::setw(10) << "Nick name"
				<< "|";
	std::cout << std::endl;
	for (int i = 0; i < total; i++)
	{
		std::cout << std::setw(10) << i << "|" << formatString(_contacts[i].getFirstName()) << "|" << formatString(_contacts[i].getLastName()) << "|" << formatString(_contacts[i].getNickname()) << "|";
		std::cout << std::endl;
	}
}

void PhoneBook::search()
{
	int	index;
	int	total;

	if (counter == -1)
	{
		std::cout << "Phonebook is empty!" << std::endl;
		return ;
	}
	total = (counter < 8 ? counter + 1 : 8);
	std::cout << "Enter the index you want to search for: ";
	if (!(std::cin >> index))
	{
		// non-numeric input
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid index — please enter a number between 0 and 7" << std::endl;
		return ;
	}
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	if (index < 0 || index >= total)
	{
		std::cout << "Index not found" << std::endl;
		return ;
	}
	std::cout << "First name: " << _contacts[index].getFirstName() << std::endl;
	std::cout << "Last name: " << _contacts[index].getLastName() << std::endl;
	std::cout << "Nick name: " << _contacts[index].getNickname() << std::endl;
	std::cout << "Phone: " << _contacts[index].getPhone() << std::endl;
	std::cout << "Secret: " << _contacts[index].getSecret() << std::endl;
}
