#include "Branch.hpp"

#include "Branch.odb.hpp"

std::vector<odb::lazy_weak_ptr<Employee>>::iterator Branch::findEmployee(const std::shared_ptr<Employee>& e)
{
    std::vector<odb::lazy_weak_ptr<Employee>>::iterator pos = employees.begin();
    for (; pos != employees.end(); ++pos)
    {
        if ((*pos).object_id() == odb::access::object_traits<Customer>::id(*e))
            break;
    }
    return pos;
}

bool Branch::addEmployee(std::shared_ptr<Employee> e)
{
    auto pos = findEmployee(e);
    if (pos != employees.end())
        return false; // already listed
    employees.push_back(e);
    e->branch = shared_from_this(); // set other end of bidirectional relation
    return true;
}

bool Branch::removeEmployee(std::shared_ptr<Employee> e)
{
    auto pos = findEmployee(e);
    if (pos == employees.end())
        return false; // not listed
    employees.erase(pos);
    e->branch = nullptr;
    return true;
}
