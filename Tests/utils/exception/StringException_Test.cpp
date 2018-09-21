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
 * Created by Longri on 19.07.18.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../src/utils/Array.h"
#include "../../src/utils/String16.h"
#include "../../../src/utils/exceptions/StringException.h"

using testing::Eq;

namespace {
    class StringException_Test : public testing::Test {
    public:

    };
}

static void throwNewEx(int idx) {
    switch (idx) {
        case 0:
            throw StringException("Fire String16 Exception", __FILE__, __FUNCTION__, __LINE__);
        case 1:
            String16 msg = String16("StringMsg");
            throw StringException(String16::toUtf8_Chars(msg), __FILE__, __FUNCTION__, __LINE__);
    }
}

TEST_F(StringException_Test, test1) {
    bool exceptionThrowed = false;
    try {
        throwNewEx(0);
    } catch (StringException ex) {
        exceptionThrowed = true;
        String16 expected = String16("Fire String16 Exception");
        String16 exMessage2 = String16(ex.what());
        ASSERT_EQ(expected, exMessage2);
    }
    ASSERT_TRUE(exceptionThrowed);
}

TEST_F(StringException_Test, test2) {
    bool exceptionThrowed = false;
    try {
        throwNewEx(1);
    } catch (StringException ex) {
        exceptionThrowed = true;
        String16 expected = String16("StringMsg");

        String16 exMessage2 = String16(ex.what());
        ASSERT_EQ(expected, exMessage2);
    }
    ASSERT_TRUE(exceptionThrowed);
}

TEST_F(StringException_Test, testNumberFormatException) {
    bool exceptionThrowed = false;
    String16 str = String16("NoNumberToParse");
    try {
        long l = str.parseLong();
    } catch (std::exception &ex) {
        exceptionThrowed = true;
        String16 expected = String16("Number is malformed: 'NoNumberToParse'");
        String16 exMessage2 = String16(ex.what());
        ASSERT_EQ(expected, exMessage2);
    }
    ASSERT_TRUE(exceptionThrowed);
}

TEST_F(StringException_Test, testNumberFormatExceptionBool) {
    bool exceptionThrowed = false;
    String16 str = String16("NoBoolToParse");
    try {
        bool b = str.parseBool();
    } catch (std::exception &ex) {
        exceptionThrowed = true;
        String16 expected = String16("Number is malformed: 'NoBoolToParse'");
        String16 exMessage2 = String16(ex.what());
        ASSERT_EQ(expected, exMessage2);
    }
    ASSERT_TRUE(exceptionThrowed);
}

TEST_F(StringException_Test, testInstanceOf) {
    bool exceptionThrowed = false;
    String16 str = String16("NoBoolToParse");
    try {
        bool b = str.parseBool();
    } catch (std::exception &ex) {
        exceptionThrowed = true;
        String16 expected = String16("Number is malformed: 'NoBoolToParse'");
        String16 exMessage2 = String16(ex.what());
        ASSERT_EQ(expected, exMessage2);

        if (StringException::instanceOf(&ex)) {
            // test ClassName and Message
            auto *stringException = dynamic_cast<StringException *>(&ex);

            String16 expectedClassName("NumberFormatException");
            String16 className(stringException->getClassName());
            ASSERT_EQ(expectedClassName, className);

            String16 fileName(stringException->getFileName());
            ASSERT_TRUE(fileName.contains(u"/src/utils/String16.h"));

            String16 expectedFunction("parseBool");
            String16 functionName(stringException->getFunctionName());
            ASSERT_EQ(expectedFunction, functionName);

            ASSERT_EQ(717, stringException->getLineNumber());


        } else {
            ASSERT_TRUE(false);
        }
    }
    ASSERT_TRUE(exceptionThrowed);
}

TEST_F(StringException_Test, testInstanceOf2) {
    bool exceptionThrowed = false;
    String16 str = String16("123456789009876543212345566789924152334755643");
    try {
        long l = str.parseLong();
    } catch (std::exception &ex) {
        exceptionThrowed = true;
        String16 expected = String16("Number is overflow: '123456789009876543212345566789924152334755643'");
        String16 exMessage2 = String16(ex.what());
        ASSERT_EQ(expected, exMessage2);

        if (StringException::instanceOf(&ex)) {
            // test ClassName and Message
            auto *stringException = dynamic_cast<StringException *>(&ex);

            String16 expectedClassName("ArithmeticException");
            String16 className(stringException->getClassName());
            ASSERT_EQ(expectedClassName, className);

            String16 fileName(stringException->getFileName());
            ASSERT_TRUE(fileName.contains(u"/src/utils/String16.h"));

            String16 expectedFunction("parseLong");
            String16 functionName(stringException->getFunctionName());
            ASSERT_EQ(expectedFunction, functionName);

            ASSERT_EQ(650, stringException->getLineNumber());


        } else {
            ASSERT_TRUE(false);
        }
    }
    ASSERT_TRUE(exceptionThrowed);
}

