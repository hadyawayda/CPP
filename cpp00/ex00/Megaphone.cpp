/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   megaphone.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hawayda <hawayda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 12:49:16 by hawayda           #+#    #+#             */
/*   Updated: 2025/07/17 12:49:16 by hawayda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Megaphone.hpp"

void shout(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *";
        return;
    }
    for (int i = 1; i < argc; ++i)
    {
        for (char *p = argv[i]; *p; ++p)
            std::cout << static_cast<char>(std::toupper(*p));
        if (i + 1 < argc)
            std::cout << ' ';
    }
}

int main(int argc, char **argv)
{
    shout(argc, argv);
    std::cout << std::endl;
    return 0;
}
