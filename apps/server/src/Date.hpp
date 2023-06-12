#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"

#ifndef _DATEOFBIRTH_INCLUDED_
#define _DATEOFBIRTH_INCLUDED_

#include <odb/core.hxx>
#include <string>

#pragma db value
class Date
{
public:
    static Date now();

    Date(const int& day, const int& month, const int& year) : year(year), month(month), day(day) {}
    Date()
    {
        *this = Date::now();
    }

    const int& getYear() const
    {
        return this->year;
    }
    const int& getMonth() const
    {
        return this->month;
    }
    const int& getDay() const
    {
        return this->day;
    }
    void setYear(const int& yearInit)
    {
        this->year = yearInit;
    }
    void setMonth(const int& monthInit)
    {
        this->month = monthInit;
    }
    void setDay(const int& dayInit)
    {
        this->day = dayInit;
    }

    int getAge() const;

    inline bool operator==(const Date& other) const
    {
        return this->day == other.day && this->month == other.month && this->year == other.year;
    }

private:
    friend class odb::access;

    int year;
    int month;
    int day;
};

#endif

#pragma clang diagnostic pop
