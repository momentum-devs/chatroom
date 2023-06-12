#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"

#ifndef _EMPLOYEE_INCLUDED_
#define _EMPLOYEE_INCLUDED_

#include <odb/core.hxx>
#include <string>

#include "Person.hpp"

class Branch;

#pragma db object
class Employee : public Person
{
public:
    Employee(const std::string& name, const Date& dateOfBirth) : Person(name, dateOfBirth) {}

    int getSalary() const
    {
        return this->salary;
    }
    void setSalary(int salaryInit)
    {
        this->salary = salaryInit;
    }

    const std::shared_ptr<Employee>& getSupervisor() const
    {
        return this->supervisor;
    }
    void setSupervisor(std::shared_ptr<Employee>& supervisorInit)
    {
        this->supervisor = supervisorInit;
    }

    const std::shared_ptr<Branch>& getBranch() const
    {
        return this->branch;
    }

private:
    friend class odb::access;
    friend class Branch;
    Employee() : Person() {}

    int salary = 0;

    std::shared_ptr<Employee> supervisor;

    std::shared_ptr<Branch> branch;
};

#include "Branch.hpp"

#endif

#pragma clang diagnostic pop
