#ifndef CONTACT_HPP
# define CONTACT_HPP

# include <iomanip>
# include <iostream>
# include <limits>
# include <string>

class Contact
{
  public:
	Contact();

	std::string getFirstName();
	void setFirstName(std::string firstName);

	std::string getLastName();
	void setLastName(std::string lastName);

	std::string getNickname();
	void setNickname(std::string nickname);

	std::string getPhone();
	void setPhone(std::string phone);

	std::string getSecret();
	void setSecret(std::string secret);

  private:
	std::string _firstName;
	std::string _lastName;
	std::string _nickname;
	std::string _phone;
	std::string _secret;
};

#endif
