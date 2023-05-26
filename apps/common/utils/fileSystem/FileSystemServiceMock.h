#pragma once

#include <gmock/gmock.h>

#include "FileSystemService.h"

namespace common::utils
{
class FileSystemServiceMock : public FileSystemService
{
public:
    MOCK_METHOD(void, write, (const std::string& absolutePath, const std::string& data), (const));
    MOCK_METHOD(void, writeAtPosition,
                (const std::string& absolutePath, const std::basic_string<unsigned char>& data, unsigned int position),
                (const));
    MOCK_METHOD(void, append, (const std::string& absolutePath, const std::string& data), (const));
    MOCK_METHOD(std::string, read, (const std::string& absolutePath), (const));
    MOCK_METHOD(bool, exists, (const std::string& absolutePath), (const));
    MOCK_METHOD(std::string, getParentDirectory, (const std::string& absolutePath), (const));
    MOCK_METHOD(std::string, getFileName, (const std::string& absolutePath), (const));
    MOCK_METHOD(void, remove, (const std::string& absolutePath), (const));
    MOCK_METHOD(void, createDirectory, (const std::string& absolutePath), (const));
};
}
