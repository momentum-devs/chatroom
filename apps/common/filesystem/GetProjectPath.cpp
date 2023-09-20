#include "GetProjectPath.h"

#include <filesystem>

#include "collection/StringHelper.h"
#include "errors/FileNotFound.h"
#include "GetExecutablePath.h"

namespace
{
constexpr auto fileNotFoundMessage{"Project directory not found in path: "};
}

namespace common::filesystem
{
std::string getProjectPath(const std::string& projectName)
{
    std::filesystem::path executablePath{getExecutablePath()};

    for (auto path = executablePath.parent_path(); path != path.root_directory(); path = path.parent_path())
    {
        if (path.filename() == projectName)
        {
            return path.generic_string();
        }
    }

    throw errors::FileNotFound{fileNotFoundMessage + executablePath.generic_string()};
}
}
