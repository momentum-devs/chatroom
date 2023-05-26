#pragma once

#include <memory>

#include "FileSystemService.h"

namespace common::utils
{
class FileSystemServiceFactory
{
public:
    std::unique_ptr<FileSystemService> createFileSystemService() const;
};
}
