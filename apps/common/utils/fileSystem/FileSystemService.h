#pragma once

#include <string>

namespace common::utils
{
class FileSystemService
{
public:
    virtual ~FileSystemService() = default;

    virtual void write(const std::string& absolutePath, const std::string& data) const = 0;
    virtual void writeAtPosition(const std::string& absolutePath, const std::basic_string<unsigned char>& data,
                                 unsigned int position) const = 0;
    virtual void append(const std::string& absolutePath, const std::string& data) const = 0;
    virtual std::string read(const std::string& absolutePath) const = 0;
    virtual bool exists(const std::string& absolutePath) const = 0;
    virtual std::string getParentDirectory(const std::string& absolutePath) const = 0;
    virtual std::string getFileName(const std::string& absolutePath) const = 0;
    virtual void remove(const std::string& absolutePath) const = 0;
    virtual void createDirectory(const std::string& absolutePath) const = 0;
};
}
