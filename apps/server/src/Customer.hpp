#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"

#ifndef _CUSTOMER_INCLUDED_
#define _CUSTOMER_INCLUDED_

#include <memory>
#include <odb/core.hxx>
#include <string>
#include <unordered_set>

#include "Date.hpp"
#include "LicenseClass.hpp"

#pragma db object
class Customer
{
public:
    Customer(const std::string& name) : name(name) {}

    const std::string& getName() const
    {
        return this->name;
    }
    const std::string& getEmail() const
    {
        return this->email;
    }
    const Date& getRegistrationDate() const
    {
        return this->registrationDate;
    }
    const std::unordered_set<std::shared_ptr<LicenseClass>>& getLicenseClasses() const
    {
        return licenseClasses;
    }

    void setEmail(const std::string& emailInit)
    {
        this->email = emailInit;
    }
    bool addLicenseClass(std::shared_ptr<LicenseClass> l)
    {
        return licenseClasses.insert(l).second;
    }
    bool removeLicenseClass(std::shared_ptr<LicenseClass> l)
    {
        return licenseClasses.erase(l) == 1;
    }

protected:
    Customer() {}

private:
    friend class odb::access;

#pragma db id
    std::string name;

    std::string email;

#pragma db value_not_null unordered
    std::unordered_set<std::shared_ptr<LicenseClass>> licenseClasses;

    Date registrationDate;
};

#endif

#pragma clang diagnostic pop
