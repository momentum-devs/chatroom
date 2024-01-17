#pragma once

#include <gmock/gmock.h>

#include "S3Service.h"

namespace server::application
{
class S3ServiceMock : public S3Service
{
public:
    MOCK_METHOD(void, putObject, (const PutObjectPayload& payload), (const override));
    MOCK_METHOD(std::string, getObject, (const GetObjectPayload& payload), (const override));
    MOCK_METHOD(bool, objectExists, (const ObjectExistsPayload& payload), (const override));
};
}
