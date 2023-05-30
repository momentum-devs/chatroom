#pragma once

#include "FileSystemService.h"

namespace common::filesystem
{
class FileSystemServiceImpl : public FileSystemService
{
public:
    void write(const std::string& absolutePath, const std::string& content) const override;
    void writeAtPosition(const std::string& absolutePath, const std::basic_string<unsigned char>& data,
                         unsigned int position) const override;
    void append(const std::string& absolutePath, const std::string& content) const override;
    std::string read(const std::string& absolutePath) const override;
    bool exists(const std::string& absolutePath) const override;
    std::string getParentDirectory(const std::string& absolutePath) const override;
    std::string getFileName(const std::string& absolutePath) const override;
    void remove(const std::string& absolutePath) const override;
    void createDirectory(const std::string& absolutePath) const override;
};
}
