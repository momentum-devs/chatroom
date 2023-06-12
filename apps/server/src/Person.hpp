#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"

#ifndef _PERSON_INCLUDED_
#define _PERSON_INCLUDED_

#include <odb/core.hxx>
#include <string>

#include "Customer.hpp"
#include "Date.hpp"

#pragma db object
class Person : public Customer
{
public:
    Person(const std::string& name, const Date& dateOfBirth) : Customer(name), dateOfBirth(dateOfBirth) {}

    Date getDateOfBirth() const
    {
        return dateOfBirth;
    }
    int getAge() const
    {
        return this->dateOfBirth.getAge();
    }

    void setDateOfBirth(const Date& dateOfBirthInit)
    {
        this->dateOfBirth = dateOfBirthInit;
    }

protected:
    Person() : Customer() {}

private:
    friend class odb::access;

    Date dateOfBirth;
};

#endif

#pragma clang diagnostic pop
