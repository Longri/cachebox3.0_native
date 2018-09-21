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
 * Created by Longri on 01.07.18.
 */

#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include "../../src/logger/Logger.h"
#include "../../src/logger/LoggerFactory.h"
#include "../../src/utils/StringObject.h"
#include "../../src/utils/FileHandle.h"
#include "../../src/utils/FileReader.h"


using testing::Eq;

namespace {
    class Logger_Test : public testing::Test {
    public:

    };
}

TEST_F(Logger_Test, testConstructor) {

    String16 testName = String16("TestLogger");
    Logger *log = LoggerFactory::getLogger(testName);
    log->debug("test");

    String16 strName = log->getName();
    ASSERT_EQ(testName, strName);
    String16 str = log->toString();
    String16 start = String16("Logger@");
    ASSERT_TRUE(str.startsWith(start));

}

TEST_F(Logger_Test, testCout) {


    String16 workingPath = FileHandle::getWorkingHandle().parent().getAbsolutePath();

    String16 expected("[DEBUG] TestLogger -  msg:test log -> debug\n"
                    "[DEBUG] TestLogger -  msg:test log -> Hello \n"
                    "[DEBUG] TestLogger -  msg:test log -> Hello World\n"
                    "[DEBUG] TestLogger -  msg:test log -> with list off four arguments \n"
                    "[DEBUG] TestLogger -  String LogTest\n"
                    "[DEBUG] TestLogger -  String LogTest Hello \n"
                    "[DEBUG] TestLogger -  String LogTest Hello World \n"
                    "[DEBUG] TestLogger -  String LogTest with list off four arguments\n"
                    "[ERROR] TestLogger -  NumberFormatException: Error with parse long\n"
                    "                      Number is malformed: 'NoNumberToParse'\n"
                    "                      WORKING_PATH/src/utils/String16.h,639\n"
                    "                      WORKING_PATH/Tests/logger/Logger_Test.cpp,127\n"
                    "                      \n"
                    "[DEBUG] TestLogger -  msg:test log -> debug\n");


    String16 testName = String16("TestLogger");
    Logger *log = LoggerFactory::getLogger(testName);


    FileHandle testResourcesDir = FileHandle(FileHandle::getWorkingPath()).parent().child("Tests/TestResources");
    ASSERT_TRUE(testResourcesDir.isDirectory());

    FileHandle logFile = testResourcesDir.child("TestDir/logTest.txt");

    //delete exist log file
    if (logFile.exists())
        logFile._delete();


    //redirect cout to file
    std::ofstream file;
    file.open(logFile.getUtf8Path(), std::ofstream::binary);

    std::cout.flush();
    std::streambuf *sbuf = std::cout.rdbuf();

    std::cout.rdbuf(file.rdbuf());

    log->debug("msg:test log -> debug");

    log->debug("msg:test log -> {} ", new StringObject("Hello"));

    log->debug("msg:test log -> {} {}", new StringObject("Hello"), new StringObject("World"));

    log->debug("msg:test log -> {} {} {} {} ",
               {new StringObject("with"), new StringObject("list"), new StringObject("off"),
                new StringObject("four arguments")});

    log->debug(String16("String LogTest"));

    log->debug(String16("String LogTest {} "), new StringObject("Hello"));

    log->debug(String16("String LogTest {} {} "), new StringObject("Hello"), new StringObject("World"));

    log->debug(String16("String LogTest {} {} {} {}"),
               {new StringObject("with"), new StringObject("list"), new StringObject("off"),
                new StringObject("four arguments")});


    //log error with Exception
    bool exceptionThrowed = false;
    String16 str = String16("NoNumberToParse");
    try {
        long l = str.parseLong();
    } catch (std::exception &ex) {
        std::exception *ptr = &ex;
        log->error("Error with parse long", ptr, __FILE__, __FUNCTION__, __LINE__);
        exceptionThrowed = true;
        String16 expected = String16("Number is malformed: 'NoNumberToParse'");
        String16 exMessage2 = String16(ex.what());
        ASSERT_EQ(expected, exMessage2);
    }
    ASSERT_TRUE(exceptionThrowed);


    log->debug("msg:test log -> debug");


    // revert redirection
    std::cout.flush();
    file.flush();
    file.close();
    std::cout.rdbuf(sbuf);

    String16 strName = log->getName();
    ASSERT_EQ(testName, strName);
    String16 str2 = log->toString();
    String16 start = String16("Logger@");
    ASSERT_TRUE(str2.startsWith(start));

    //read logTest.txt and check Log output line by line
    String16 s("              ");


    String16 fileString = FileReader(logFile.getPath()).readString();


    //split into lines
    std::vector<String16> lines = fileString.split('\n');
    ASSERT_TRUE(lines.size() <= 15);

    boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
    String16 FORMAT_STR = String16("dd-MM-yy hh:mm:");
    String16 dateTime = String16::getFormattedDate(now, FORMAT_STR);

    int idx = 1;
    for (String16 line:lines) {
        if (idx < 15) {
            if (idx <= 9 || idx > 13) {
                ASSERT_TRUE(line.startsWith(dateTime));
            } else {
                ASSERT_TRUE(line.startsWith(s));
            }
        }
        idx++;
    }

    //remove date from line and check content
    String16 stringBuilder;
    idx = 0;
    for (String16 line:lines) {
        String16 newLine(line, 22, line.length());
        stringBuilder.append(newLine);
        stringBuilder.append('\n');
        if (idx++ > 12)break;
    }

    stringBuilder.replaceAll(workingPath, String16("WORKING_PATH"));

    ASSERT_EQ(expected, stringBuilder);
}

TEST_F(Logger_Test, testResolveMessage) {

    String16 str = String16("Hallo {}, this is a formated {} with {} replaced arguments");
    String16 strExspected = String16("Hallo test, this is a formated message with 3 replaced arguments");
    StringObject *arg1 = new StringObject(String16("test"));
    StringObject *arg2 = new StringObject(String16("message"));
    StringObject *arg3 = new StringObject(3);


    std::vector<Object *> vec = std::vector<Object *>();
    vec.push_back(arg1);
    vec.push_back(arg2);
    vec.push_back(arg3);

    String16 message = Logger::resolveMessage(str, vec);

    ASSERT_EQ(message, strExspected);
}

TEST_F(Logger_Test, testResolveMessage2) {

    String16 str = String16("Hallo {}, this is a formated {}");
    String16 strExspected = String16("Hallo test, this is a formated message");
    StringObject *arg1 = new StringObject(String16("test"));
    StringObject *arg2 = new StringObject(String16("message"));

    std::vector<Object *> vec = std::vector<Object *>();
    vec.push_back(arg1);
    vec.push_back(arg2);

    String16 message = Logger::resolveMessage(str, vec);

    ASSERT_EQ(message, strExspected);
}


