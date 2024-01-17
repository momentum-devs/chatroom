#pragma once

#include "payloads/GetObjectPayload.h"
#include "payloads/ObjectExistsPayload.h"
#include "payloads/PutObjectPayload.h"

namespace server::application
{
class S3Service
{
public:
    virtual ~S3Service() = default;

    virtual void putObject(const PutObjectPayload& payload) const = 0;
    virtual std::string getObject(const GetObjectPayload& payload) const = 0;
    virtual bool objectExists(const ObjectExistsPayload& payload) const = 0;
};
}
