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
 * Created by Longri on 05.07.2018
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../src/logger/Logger.h"
#include "../../src/logger/LoggerFactory.h"
#include "../../src/utils/Reader.h"
#include "../../src/utils/FileReader.h"
#include "../../src/utils/JsonStreamParser.h"

using testing::Eq;

const static char16_t testArray[359] = u"{\n"
                                       "  \"Status\": {\n"
                                       "    \"ImageURL\": \"http:\\/\\/www.geocaching.com\\/images\\/wpttypes\\/2.gif\",\n"
                                       "    \"StatusCode\": 0,\n"
                                       "    \"StatusMessage\": \"OK\",\n"
                                       "    \"ExceptionDetails\": \"\",\n"
                                       "    \"Warnings\": [],\n"
                                       "    \"IsContainer\": false,\n"
                                       "    \"IsGrandfathered\": null\n"
                                       "  },\n"
                                       "  \"Geocaches\": [\n"
                                       "    {\n"
                                       "      \"AccountID\": 137464\n"
                                       "    },\n"
                                       "    {\n"
                                       "      \"AccountID\": 137464\n"
                                       "    }\n"
                                       "  ],\n"
                                       "  \"PQCount\": 0\n"
                                       "}";


const static long dataLength = std::char_traits<char16_t>::length(testArray);

namespace {
    class JsonStreamParser_Test : public testing::Test {
    public:

    };
}

TEST_F(JsonStreamParser_Test, getName) {// NOLINT
    JsonStreamParser jtp = JsonStreamParser();
    String16 *result = jtp.getName(testArray, dataLength, 4);
    ASSERT_EQ(String16("Status"), *result);

    result = jtp.getName(testArray, dataLength, 20);
    ASSERT_EQ(String16("ImageURL"), *result);

    result = jtp.getName(testArray, dataLength, 92);
    ASSERT_EQ(String16("StatusCode"), *result);

    result = jtp.getName(testArray, dataLength, 113);
    ASSERT_EQ(String16("StatusMessage"), *result);

    result = jtp.getName(testArray, dataLength, 140);
    ASSERT_EQ(String16("ExceptionDetails"), *result);

    result = jtp.getName(testArray, dataLength, 168);
    ASSERT_EQ(String16("Warnings"), *result);
}


TEST_F(JsonStreamParser_Test, searchNameBefore) {// NOLINT
    JsonStreamParser jtp = JsonStreamParser();
    long result = jtp.searchNameBefore(testArray, 0);
    ASSERT_EQ(-1, result);

    result = jtp.searchNameBefore(testArray, 14);
    ASSERT_EQ(4, result);

    result = jtp.searchNameBefore(testArray, 86);
    ASSERT_EQ(20, result);

    result = jtp.searchNameBefore(testArray, 107);
    ASSERT_EQ(92, result);

    result = jtp.searchNameBefore(testArray, 134);
    ASSERT_EQ(113, result);

    result = jtp.searchNameBefore(testArray, 162);
    ASSERT_EQ(140, result);

    result = jtp.searchNameBefore(testArray, 180);
    ASSERT_EQ(168, result);
}


TEST_F(JsonStreamParser_Test, searchPeekTest) {// NOLINT

    JsonStreamParser jtp = JsonStreamParser();
    long result = jtp.searchPeek(testArray, dataLength, 0);
    ASSERT_EQ(0, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(14, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(86, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(107, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(134, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(162, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(180, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(181, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(182, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(208, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(240, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(241, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(258, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(264, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(296, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(297, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(303, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(335, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(339, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(340, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(357, result);

    result = jtp.searchPeek(testArray, dataLength, result + 1);
    ASSERT_EQ(-1, result);


}

TEST_F(JsonStreamParser_Test, parseData) {// NOLINT

    static auto *sb = new String16();

    class TestClass : public JsonStreamParser {
    public:
        void startArray(String16 *name) override {
            sb->append("startArray: ");
            sb->append(name);
            sb->append("\n");
        }

        void endArray(String16 *name) override {
            sb->append("endArray: ");
            sb->append(name);
            sb->append("\n");
        }

        void startObject(String16 *name) override {
            sb->append("startObject: ");
            sb->append(name);
            sb->append("\n");
        }

        void pop() override {
            sb->append("pop()");
            sb->append("\n");
        }

        void string(String16 *name, String16 *value) override {
            sb->append("StringValue: ");
            sb->append(name);
            sb->append(" = ");
            sb->append(value);
            sb->append("\n");
        }

        void number(String16 *name, double value, String16 *stringValue) override {
            sb->append("DoubleValue: ");
            sb->append(name);
            sb->append(" = ");
            sb->appendDouble(value);
            sb->append("\n");
        }

        void number(String16 *name, long value, String16 *stringValue) override {
            sb->append("longValue: ");
            sb->append(name);
            sb->append(" = ");
            sb->appendLong(value);
            sb->append("\n");
        }

        void boolean(String16 *name, bool value) override {
            sb->append("BoolValue: ");
            sb->append(name);
            sb->append(" = ");
            sb->append(value ? "true" : "false");
            sb->append("\n");
        }
    };

    TestClass jtp = TestClass();

    jtp.parseData(testArray, dataLength);

    const char expectedArray[506] = "startObject: null\n"
                                    "startObject: Status\n"
                                    "StringValue: ImageURL = http://www.geocaching.com/images/wpttypes/2.gif\n"
                                    "longValue: StatusCode = 0\n"
                                    "StringValue: StatusMessage = OK\n"
                                    "StringValue: ExceptionDetails = \n"
                                    "startArray: Warnings\n"
                                    "pop()\n"
                                    "endArray: Warnings\n"
                                    "BoolValue: IsContainer = false\n"
                                    "StringValue: IsGrandfathered = null\n"
                                    "pop()\n"
                                    "startArray: Geocaches\n"
                                    "startObject: null\n"
                                    "longValue: AccountID = 137464\n"
                                    "pop()\n"
                                    "startObject: null\n"
                                    "longValue: AccountID = 137464\n"
                                    "pop()\n"
                                    "pop()\n"
                                    "endArray: Geocaches\n"
                                    "longValue: PQCount = 0\n"
                                    "pop()\n";
    String16 expected(expectedArray);

    ASSERT_EQ(expected, *sb);

}

TEST_F(JsonStreamParser_Test, test2) { // NOLINT

    //resource FileHandle
    FileHandle testResourcesDir = FileHandle(FileHandle::getWorkingPath()).parent().child("Tests/TestResources");


    //create File reader
    FileHandle fileHandle = testResourcesDir.child("JsonArrayTest.json");
    ASSERT_TRUE(fileHandle.exists());
    FileReader reader(fileHandle.getPath());

    //create a Class derived JasonStreamParser as TestClass
    class TestClass : public JsonStreamParser {

    public:

        String16 stringBuilder = String16();

        void startArray(String16 *name) override {
            stringBuilder.append("startArray: ");
            stringBuilder.append(name);
            stringBuilder.append("\n");
        }

        void endArray(String16 *name) override {
            stringBuilder.append("endArray: ");
            stringBuilder.append(name);
            stringBuilder.append("\n");
        }

        void startObject(String16 *name) override {
            stringBuilder.append("startObject: ");
            stringBuilder.append(name);
            stringBuilder.append("\n");
        }

        void pop() override {
            stringBuilder.append("pop()");
            stringBuilder.append("\n");
        }

        void string(String16 *name, String16 *value) override {
            stringBuilder.append("StringValue: ");
            stringBuilder.append(name);
            stringBuilder.append(" = ");
            stringBuilder.append(value);
            stringBuilder.append("\n");
        }

        void number(String16 *name, double value, String16 *stringValue) override {
            stringBuilder.append("DoubleValue: ");
            stringBuilder.append(name);
            stringBuilder.append(" = ");
            stringBuilder.appendDouble(value);
            stringBuilder.append("\n");
        }

        void number(String16 *name, long value, String16 *stringValue) override {
            stringBuilder.append("longValue: ");
            stringBuilder.append(name);
            stringBuilder.append(" = ");
            stringBuilder.appendLong(value);
            stringBuilder.append("\n");
        }

        void boolean(String16 *name, bool value) override {
            stringBuilder.append("BoolValue: ");
            stringBuilder.append(name);
            stringBuilder.append(" = ");
            stringBuilder.appendBool(value);
            stringBuilder.append("\n");
        }

    };

    TestClass testClass = TestClass();
    reader.open();
    testClass.parse(&reader, 1);


    const char *exStr = "startObject: null\n"
                        "startObject: Status\n"
                        "StringValue: ImageURL = http://www.geocaching.com/images/wpttypes/2.gif\n"
                        "longValue: StatusCode = 0\n"
                        "StringValue: StatusMessage = OK\n"
                        "StringValue: ExceptionDetails = \n"
                        "startArray: Warnings\n"
                        "pop()\n"
                        "endArray: Warnings\n"
                        "BoolValue: IsContainer = false\n"
                        "StringValue: IsGrandfathered = null\n"
                        "pop()\n"
                        "startArray: Geocaches\n"
                        "startObject: null\n"
                        "longValue: AccountID = 137464\n"
                        "pop()\n"
                        "startObject: null\n"
                        "longValue: AccountID = 137464\n"
                        "pop()\n"
                        "pop()\n"
                        "endArray: Geocaches\n"
                        "longValue: PQCount = 0\n"
                        "pop()\n";
    
    String16 expected = String16(exStr);
    String16 parseResult = testClass.stringBuilder;

    ASSERT_EQ(expected, parseResult);

}