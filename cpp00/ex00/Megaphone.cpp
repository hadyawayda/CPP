/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Megaphone.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42beirut.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 20:27:28 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/19 20:27:28 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <string>
#include <cctype>

class Megaphone {
public:
    // Constructor: builds the heap-allocated vector of messages
    Megaphone(int ac, char **av)
    {
		messages_ = new std::vector<std::string>();
        shout(ac, av);
    }

    // Destructor: invoked by 'delete horn;'
    // - First calls each member's destructor (none need special handling)
    // - Then frees the heap memory for messages_
    ~Megaphone()
    {
        delete messages_;
        messages_ = 0;
    }

    // Print all stored, already-uppercased messages separated by spaces, then newline
    void announce() const
    {
        for (size_t i = 0; i < messages_->size(); ++i)
        {
            std::cout << (*messages_)[i];
            if (i + 1 < messages_->size())
                std::cout << ' ';
        }
        std::cout << '\n';
    }

private:
    std::vector<std::string>* messages_;  // heap-allocated container

    // Uppercase converter (instance method)
    std::string toUpper(const std::string &s)
    {
        std::string out;
        out.reserve(s.size());
        for (size_t i = 0; i < s.size(); ++i)
        {
            unsigned char c = static_cast<unsigned char>(s[i]);
            out.push_back(static_cast<char>(std::toupper(c)));
        }
        return out;
    }

    void shout(int ac, char **av)
    {
        if (ac < 2)
        {
            messages_->push_back(toUpper("* LOUD AND UNBEARABLE FEEDBACK NOISE *"));
        }
        else
        {
            for (int i = 1; i < ac; ++i)
                messages_->push_back(toUpper(av[i]));
        }
    }
};

int main(int ac, char **av)
{
    // Allocate on heap
    Megaphone *horn = new Megaphone(ac, av);

    horn->announce();

    // Properly destroy:
    // 1) Calls ~Megaphone(), which delete’s the internal vector
    // 2) Frees horn’s memory
    delete horn;
    return 0;
}
