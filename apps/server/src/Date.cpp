#include "Date.odb.hpp"
#include "Date.hpp"

#include <ctime>


int Date::getAge() const
{
	Date now = Date::now();
	int age = now.getYear() - this->getYear();
	if( now.getMonth() < this->getMonth() || ( now.getMonth() == this->getMonth() && now.getDay() < this->getDay() ) )
		age = age -1;
	return age;
}


Date Date::now()
{
	time_t t = std::time( nullptr );
	struct std::tm * now = std::localtime( &t );
	return Date( now->tm_mday, 1 + now->tm_mon, 1900 + now->tm_year );
}
