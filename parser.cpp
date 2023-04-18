/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 15:36:34 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/18 16:41:43 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

std::string	ft_itoa( int number )
{
	std::ostringstream	oss;
	std::string			converted;

	oss << number;
	converted = oss.str();
	return (converted);
}

std::vector<std::string> splitStr( std::string str, char sep )
{
	size_t					  begin;
	size_t					  end = 0;
	std::vector<std::string>	ret;

	begin = str.find_first_not_of(sep, end);
	while (begin != std::string::npos)
	{
		end = str.find(sep, begin);
		std::string newStr = str.substr(begin, end - begin);
		ret.push_back(newStr);
		begin = str.find_first_not_of(sep, end);
	}
	return (ret);
}

