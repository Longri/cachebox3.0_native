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
 * Created by Longri on 25.07.18.
 */


#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../src/utils/FileHandle.h"
#include "../../src/utils/FileReader.h"

using testing::Eq;

namespace {
    class FileReader_Test : public testing::Test {
    public:
    };
}

TEST_F(FileReader_Test, test1) {

    FileHandle testResourcesDir = FileHandle(FileHandle::getWorkingPath()).parent().child("Tests/TestResources");
    ASSERT_TRUE(testResourcesDir.isDirectory());

    FileHandle testFile1 = testResourcesDir.child("TestFile.txt");
    ASSERT_TRUE(testFile1.exists());

    char16_t CHARS[20];
    std::fill(CHARS, CHARS + 20, '\0');
    FileReader reader(testFile1.getPath());
    long resultLength;

    reader.open();
    resultLength = reader.read(CHARS, 0, 20);
    reader.close();

    String16 str(CHARS);
    ASSERT_EQ(str, String16("Hallo Test"));
    ASSERT_EQ(10, resultLength);

    std::fill(CHARS, CHARS + 20, '\0');
    reader.open();
    reader.read(CHARS, 3, 5);
    reader.close();

    String16 str2(CHARS, 3, 5);
    ASSERT_TRUE(str2 == "Hallo");
}

TEST_F(FileReader_Test, testUtf8) {
    FileHandle testResourcesDir = FileHandle(FileHandle::getWorkingPath()).parent().child("Tests/TestResources");
    ASSERT_TRUE(testResourcesDir.isDirectory());

    FileHandle testFile1 = testResourcesDir.child("TestFileUtf8.txt");
    ASSERT_TRUE(testFile1.exists());
    ASSERT_TRUE(testFile1.fileSize() == 20); // 20byte for 14 Chars

    char16_t CHARS[21];
    std::fill(CHARS, CHARS + 20, '\0');

    long readCount = 0;

    FileReader reader(testFile1.getPath());

    reader.open();
    readCount = reader.read(CHARS, 0, 20);
    reader.close();

    String16 str(CHARS);
    ASSERT_TRUE(str == "߷®Hällo߷®Hällo");
    ASSERT_EQ(14, readCount);

    std::fill(CHARS, CHARS + 20, 'z');
    CHARS[20] = '\0';
    reader.open();
    readCount = reader.read(CHARS, 3, 6);
    reader.close();

    ASSERT_EQ(6, readCount);
    ASSERT_EQ(String16(CHARS), String16("zzz߷®Hällzzzzzzzzzzz"));


    std::fill(CHARS, CHARS + 20, 'Z');

    reader.open();
    readCount = reader.read(CHARS, 4, 7);
    reader.close();
    ASSERT_EQ(7, readCount);
    ASSERT_EQ(String16(CHARS), String16("ZZZZ߷®HälloZZZZZZZZZ"));

}

TEST_F(FileReader_Test, testUtf8_2) {

    FileHandle testResourcesDir = FileHandle(FileHandle::getWorkingPath()).parent().child("Tests/TestResources");
    ASSERT_TRUE(testResourcesDir.isDirectory());

    FileHandle testFile1 = testResourcesDir.child("TestFileUtf8_2.txt");
    ASSERT_TRUE(testFile1.exists());
#if defined _WIN32
    ASSERT_EQ(349, testFile1.fileSize()); // 349byte for 185 Chars (\r\n) '\r' is replaced by reader
#else
    ASSERT_EQ(347, testFile1.fileSize()); // 347byte for 185 Chars
#endif



    long charCount = 1000;
    char16_t CHARS[charCount];
    std::fill(CHARS, CHARS + charCount, '\0');

    long readCount = 0;
    FileReader reader(testFile1.getPath());

    reader.open();
    readCount = reader.read(CHARS, 0, 22);
    ASSERT_EQ(22, readCount);
    String16 test("🄐🄑🄒🄓🄔🄕🄖🄗🄘🄙🄚");
    ASSERT_EQ(String16("🄐🄑🄒🄓🄔🄕🄖🄗🄘🄙🄚"), String16(CHARS));

    readCount = reader.read(CHARS, 22, 22);
    ASSERT_EQ(22, readCount);
    ASSERT_EQ(String16("🄐🄑🄒🄓🄔🄕🄖🄗🄘🄙🄚🄛🄜🄝🄞🄟🄠🄡🄢🄣🄤🄥"), String16(CHARS));

    readCount = reader.read(CHARS, 44, 6);
    ASSERT_EQ(6, readCount);
    ASSERT_EQ(String16("🄐🄑🄒🄓🄔🄕🄖🄗🄘🄙🄚🄛🄜🄝🄞🄟🄠🄡🄢🄣🄤🄥🄦🄧🄨"), String16(CHARS));

    readCount = reader.read(CHARS, 50, 13);
    ASSERT_EQ(13, readCount);
    ASSERT_EQ(String16("🄐🄑🄒🄓🄔🄕🄖🄗🄘🄙🄚🄛🄜🄝🄞🄟🄠🄡🄢🄣🄤🄥🄦🄧🄨🄩 ߷®Häl߷®Häl"), String16(CHARS));

    readCount = reader.read(CHARS, 63, 45);
    ASSERT_EQ(45, readCount);
    ASSERT_EQ(String16("🄐🄑🄒🄓🄔🄕🄖🄗🄘🄙🄚🄛🄜🄝🄞🄟🄠🄡🄢🄣🄤🄥🄦🄧🄨🄩 ߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Höl ߷®Häl߷®Häl ߷®Häl߷®Häl "), String16(CHARS));


    readCount = reader.read(CHARS, 108, 45);
    ASSERT_EQ(45, readCount);
    ASSERT_EQ(String16("🄐🄑🄒🄓🄔🄕🄖🄗🄘🄙🄚🄛🄜🄝🄞🄟🄠🄡🄢🄣🄤🄥🄦🄧🄨🄩 ߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Höl ߷®Häl߷®Häl ߷®Häl߷®Häl \n߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Häl "), String16(CHARS));

    readCount = reader.read(CHARS, 153, 200);
    ASSERT_EQ(58, readCount);
    ASSERT_EQ(String16("🄐🄑🄒🄓🄔🄕🄖🄗🄘🄙🄚🄛🄜🄝🄞🄟🄠🄡🄢🄣🄤🄥🄦🄧🄨🄩 ߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Höl ߷®Häl߷®Häl ߷®Häl߷®Häl \n߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Häl\nEOF"), String16(CHARS));

    readCount = reader.read(CHARS, 153, 200);
    ASSERT_EQ(-1, readCount);
    ASSERT_EQ(String16("🄐🄑🄒🄓🄔🄕🄖🄗🄘🄙🄚🄛🄜🄝🄞🄟🄠🄡🄢🄣🄤🄥🄦🄧🄨🄩 ߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Höl ߷®Häl߷®Häl ߷®Häl߷®Häl \n߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Häl ߷®Häl߷®Häl\nEOF"), String16(CHARS));

    //finally close
    reader.close();
}

TEST_F(FileReader_Test, testStringConstructor) {
    FileHandle testResourcesDir = FileHandle(FileHandle::getWorkingPath()).parent().child("Tests/TestResources");
    ASSERT_TRUE(testResourcesDir.isDirectory());

    FileHandle testFile1 = testResourcesDir.child("TestFileUtf8.txt");
    ASSERT_TRUE(testFile1.exists());
    ASSERT_TRUE(testFile1.fileSize() == 20); // 20byte for 14 Chars

    FileReader reader(testFile1.getPath());

    reader.open();
    String16 fileString16 = reader.readString();
    reader.close();

    ASSERT_TRUE(fileString16 == "߷®Hällo߷®Hällo");
}

TEST_F(FileReader_Test, testStringConstructor2) {
    FileHandle testResourcesDir = FileHandle(FileHandle::getWorkingPath()).parent().child("Tests/TestResources");
    ASSERT_TRUE(testResourcesDir.isDirectory());

    FileHandle testFile1 = testResourcesDir.child("TestFileUtf8.txt");
    ASSERT_TRUE(testFile1.exists());
    ASSERT_TRUE(testFile1.fileSize() == 20); // 20byte for 14 Chars

    FileReader reader(testFile1.getPath());
    String16 fileString16 = reader.readString();

    ASSERT_TRUE(fileString16 == "߷®Hällo߷®Hällo");
}
