#pragma once

#include <memory>

#include "FileSystemService.h"

namespace common::filesystem
{
class FileSystemServiceFactory
{
public:
    std::unique_ptr<FileSystemService> createFileSystemService() const;
};
}
