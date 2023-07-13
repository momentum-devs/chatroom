#pragma once

#include <string>

namespace server::application
{
struct SendEmailPayload
{
    std::string to;
    std::string from;
    std::string subject;
    std::string data;
};

inline bool operator==(const SendEmailPayload& lhs, const SendEmailPayload& rhs)
{
    return (lhs.to == rhs.to && lhs.from == rhs.from && lhs.subject == rhs.subject && lhs.data == rhs.data);
}

}
