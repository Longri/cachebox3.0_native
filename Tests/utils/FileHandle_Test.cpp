/*
 * Copyright 2018 Longri
 *
 * This program is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
 *
 * Created by Longri on 22.07.18.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../src/utils/FileHandle.h"

using testing::Eq;

namespace {
    class FileHandle_Test : public testing::Test {
    public:
    };
}

TEST_F(FileHandle_Test, testConstructor) {
    String16 path = String16("@Work/testDir/testFile.txt");
    FileHandle fh(path);
    String16 expectedPath(path);
    ASSERT_EQ(expectedPath, fh.getAbsolutePath());
    ASSERT_STREQ("@Work/testDir/testFile.txt", fh.getPath()); //check utf8 char array

    ASSERT_EQ(String16("testFile.txt"), fh.name());
    ASSERT_EQ(String16("txt"), fh.extension());
    ASSERT_EQ(String16("@Work/testDir/testFile"), fh.pathWithoutExtension());
}

TEST_F(FileHandle_Test, testChilds) {
    String16 path = String16("@Work/testDir");
    FileHandle fh(path);
    String16 child("child.txt");
    FileHandle childFh = fh.child(child);
    ASSERT_STREQ("@Work/testDir/child.txt", childFh.getPath()); //check utf8 char array
}

TEST_F(FileHandle_Test, testParent) {
    String16 path = String16("@Work/testDir/testFile.txt");
    FileHandle fh(path);
    String16 expectedPath(path);
    ASSERT_EQ(expectedPath, fh.getAbsolutePath());
    ASSERT_STREQ("@Work/testDir/testFile.txt", fh.getPath()); //check utf8 char array
    ASSERT_EQ(String16("@Work/testDir"), fh.parent().getAbsolutePath());

    String16 path2 = String16("../../test/cout.txt");
    FileHandle fh2(path2);
    String16 endsWith = String16("/test/cout.txt");
    ASSERT_TRUE(fh2.getAbsolutePath().endsWith(endsWith));
}

TEST_F(FileHandle_Test, testExist) {
    String16 path = String16("./CMakeCache.txt");
    FileHandle fh(path);

    String16 expected = FileHandle::getWorkingPath();
    expected.append("/CMakeCache.txt");

    ASSERT_TRUE(fh.exists());
    ASSERT_EQ(expected, fh.getAbsolutePath());

    ASSERT_FALSE(fh.isDirectory());

    FileHandle parentFH = fh.parent();
    ASSERT_TRUE(parentFH.isDirectory());
    ASSERT_FALSE(parentFH.exists());
}

TEST_F(FileHandle_Test, testList) {

    String16 workingPath = FileHandle::getWorkingPath();
    FileHandle fileHandle(workingPath);

    FileHandle testResourcesDir = fileHandle.parent().child("Tests/TestResources");
    ASSERT_TRUE(testResourcesDir.isDirectory());

    std::vector<FileHandle> list = testResourcesDir.list();

    int expected;
#if defined(__APPLE__)
    expected = 8; //Apple has .DS folder
#else
    expected=7;
#endif

    ASSERT_EQ(expected, list.size());

    bool testDirFound = false;
    for (int i = 0; i < list.size(); ++i) {
        FileHandle fh = list[i];
        if (fh.name() == "TestDir") {
            if (fh.isDirectory()) {
                testDirFound = true;
            }
        }
    }
    ASSERT_TRUE(testDirFound);

}

TEST_F(FileHandle_Test, testListFilter) {

    String16 workingPath = FileHandle::getWorkingPath();
    FileHandle fileHandle(workingPath);

    FileHandle testResourcesDir = fileHandle.parent().child("Tests/TestResources");
    ASSERT_TRUE(testResourcesDir.isDirectory());


    class directoryFilter : public FileFilter {
        bool accept(char *filePath) const override {
            // String16 str(filePath);
            FileHandle fh(filePath);
            return fh.isDirectory();
        }
    };

    std::vector<FileHandle> list = testResourcesDir.list(directoryFilter());
    ASSERT_TRUE(list.size() == 1); //only dir's

    bool testDirFound = false;
    for (int i = 0; i < list.size(); ++i) {
        FileHandle fh = list[i];
        if (fh.name() == "TestDir") {
            if (fh.isDirectory()) {
                testDirFound = true;
            }
        }
    }
    ASSERT_TRUE(testDirFound);

}

TEST_F(FileHandle_Test, testFileSize) {
    FileHandle testResourcesDir = FileHandle(FileHandle::getWorkingPath()).parent().child("Tests/TestResources");
    FileHandle fh = testResourcesDir.child("TestFile.txt");
    long fileSize = fh.fileSize();
    ASSERT_EQ(10, fileSize);
}

TEST_F(FileHandle_Test, testMake_Delet_Dir) {
    FileHandle testResourcesDir = FileHandle(FileHandle::getWorkingPath()).parent().child("Tests/TestResources");
    FileHandle makedDir = testResourcesDir.child("MakeDir");

    ASSERT_TRUE(!makedDir.isDirectory());
    makedDir.mkdirs();
    ASSERT_TRUE(makedDir.isDirectory());

    //Write File into directory
    FileHandle file = makedDir.child("WriteTestFile.txt");

    String16 write("Write Utf8 Text '˷ꁾ' to File!\nWith two lines!");
    file.writeString(write, false);

    String16 read = file.readString();
    ASSERT_EQ(write, read);

    //delete directory
    ASSERT_TRUE(makedDir.deleteDirectory());
    ASSERT_TRUE(!makedDir.isDirectory());
}

TEST_F(FileHandle_Test, testWriteFile) {
    FileHandle testResourcesDir = FileHandle(FileHandle::getWorkingPath()).parent().child(
            "Tests/TestResources/TestDir");
    FileHandle file = testResourcesDir.child("WriteTestFile.txt");

    String16 write("Write Utf8 Text '˷ꁾ' to File!\nWith two lines!");
    file.writeString(write, false);

    String16 read = file.readString();
    ASSERT_EQ(write, read);
}

TEST_F(FileHandle_Test, testCopyFile) {
    FileHandle testResourcesDir = FileHandle(FileHandle::getWorkingPath()).parent().child("Tests/TestResources");
    FileHandle makedDir = testResourcesDir.child("CopyTestDir");

    ASSERT_TRUE(!makedDir.isDirectory());
    makedDir.mkdirs();
    ASSERT_TRUE(makedDir.isDirectory());

    //copy file
    FileHandle oriFile = testResourcesDir.child("TestFileUtf8.txt");
    FileHandle copyfile = makedDir.child("CopyTestFile.txt");

    oriFile.copy(copyfile);

    String16 write("Write Utf8 Text '˷ꁾ' to File!\nWith two lines!");
    copyfile.writeString(write, true);


    String16 copyExpectedRead("߷®Hällo߷®Hällo");
    copyExpectedRead.append(write);

    String16 read = copyfile.readString();
    ASSERT_EQ(copyExpectedRead, read);

    //delete directory
    ASSERT_TRUE(makedDir.deleteDirectory());
    ASSERT_TRUE(!makedDir.isDirectory());
}

