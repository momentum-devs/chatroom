#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#ifndef _ADDRESS_INCLUDED_
#define _ADDRESS_INCLUDED_

#include <odb/core.hxx>
#include <string>

#pragma db value
class Address
{
public:
    Address(const std::string& state, const std::string& city, const std::string& street, const std::string& zip,
            const std::string& type)
        : state(state), city(city), street(street), zip(zip), type(type)
    {
    }
    Address() {}

    const std::string& getState() const
    {
        return this->state;
    }
    const std::string& getCity() const
    {
        return this->city;
    }
    const std::string& getStreet() const
    {
        return this->street;
    }
    const std::string& getZip() const
    {
        return this->zip;
    }
    const std::string& getType() const
    {
        return this->type;
    }

    inline bool operator==(const Address& other) const
    {
        return this->state == other.state && this->city == other.city && this->street == other.street &&
               this->zip == other.zip && this->type == other.type;
    }

private:
    friend class odb::access;

    std::string state;
    std::string city;
    std::string street;
    std::string zip;
    std::string type;
};

#endif

#pragma clang diagnostic pop
