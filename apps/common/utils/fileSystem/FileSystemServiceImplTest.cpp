#include "FileSystemServiceImpl.h"

#include <format>
#include "gtest/gtest.h"

#include "errors/FileNotFound.h"
#include "GetProjectPath.h"

using namespace ::testing;
using namespace common::utils;

namespace
{
const std::string testFilesDirectory{
    std::format("{}apps/common/utils/fileSystem/testFiles", getProjectPath("chatroom"))};
const std::string textToWrite{"write method"};
const std::basic_string<unsigned char> textToWriteAtPosition{reinterpret_cast<const unsigned char*>("position data")};
const std::string textToWriteAtPositionAsString{"position data"};
const std::string textToAppend{"append method"};
const std::string textAfterWriteAndAppend{textToWrite + textToAppend};
const std::string filenameForReading = "testReading.txt";
const std::string filenameForWriting = "testWriting.txt";
const std::string filenameForWritingAtPosition = "testWritingAtPosition.txt";
const std::string pathForReading{std::format("{}/{}", testFilesDirectory, filenameForReading)};
const std::string pathForWriting{std::format("{}/{}", testFilesDirectory, filenameForWriting)};
const std::string nestedFilePath{std::format("{}/dir1/dir2/{}", testFilesDirectory, filenameForWriting)};
const std::string pathForWritingAtPosition{std::format("{}/{}", testFilesDirectory, filenameForWritingAtPosition)};
const std::string exampleContent{"example data\n"};
const std::string incorrectPath = "433\\UTzxxxx/fi123xtF";
}

class FileSystemServiceImplTest : public Test
{
public:
    FileSystemServiceImpl fileSystemService;
};

TEST_F(FileSystemServiceImplTest, givenCorrectPath_shouldWriteToFile)
{
    fileSystemService.write(pathForWriting, textToWrite);

    const auto actualFileContent = fileSystemService.read(pathForWriting);

    ASSERT_EQ(actualFileContent, textToWrite);
}

TEST_F(FileSystemServiceImplTest, givenCorrectPath_shouldAppendToFile)
{
    fileSystemService.write(pathForWriting, textToWrite);
    fileSystemService.append(pathForWriting, textToAppend);

    const auto actualFileContent = fileSystemService.read(pathForWriting);

    ASSERT_EQ(actualFileContent, textAfterWriteAndAppend);
}

TEST_F(FileSystemServiceImplTest, givenCorrectPath_shouldReturnContentOfFile)
{
    const auto actualFileContent = fileSystemService.read(pathForReading);

    ASSERT_EQ(actualFileContent, exampleContent);
}

TEST_F(FileSystemServiceImplTest, givenCorrectPath_shouldWriteToFileAtPosition)
{
    fileSystemService.writeAtPosition(pathForWritingAtPosition, textToWriteAtPosition, 5);
    fileSystemService.writeAtPosition(pathForWritingAtPosition, textToWriteAtPosition, 30);

    const auto actualFileContent = fileSystemService.read(pathForWritingAtPosition);

    ASSERT_EQ(actualFileContent.size(), 43);
    ASSERT_EQ(actualFileContent.substr(5, textToWriteAtPosition.size()), textToWriteAtPositionAsString);
    ASSERT_EQ(actualFileContent.substr(30, textToWriteAtPosition.size()), textToWriteAtPositionAsString);
}

TEST_F(FileSystemServiceImplTest, givenFilePath_shouldReturnParentDirectory)
{
    const auto parentDirectory = fileSystemService.getParentDirectory(pathForReading);

    ASSERT_EQ(parentDirectory, testFilesDirectory);
}

TEST_F(FileSystemServiceImplTest, givenFilePath_shouldReturnFileName)
{
    const auto fileName = fileSystemService.getFileName(pathForReading);

    ASSERT_EQ(fileName, filenameForReading);
}

TEST_F(FileSystemServiceImplTest, givenNestedPath_shouldCreateFileWithSubdirectories)
{
    if (fileSystemService.exists(nestedFilePath))
        fileSystemService.remove(nestedFilePath);

    fileSystemService.write(nestedFilePath, "");

    ASSERT_TRUE(fileSystemService.exists(nestedFilePath));
}
