/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Phonebook.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 17:09:06 by hawayda           #+#    #+#             */
/*   Updated: 2025/08/01 00:32:05 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHONEBOOK_HPP
# define PHONEBOOK_HPP

# include "Contact.hpp"
# include <cstring>
# include <iomanip>
# include <iostream>
# include <limits>
# include <sstream>

class PhoneBook
{
  public:
	PhoneBook();
	void add();
	void search();
	void display();

	int index;

  private:
	static int counter;
	Contact _contacts[8];
};

#endif
