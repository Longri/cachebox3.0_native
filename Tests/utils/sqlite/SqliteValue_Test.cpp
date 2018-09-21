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
 * Created by Longri on 08.09.18.
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstring>
#include "../../../src/utils/sqlite/SqliteValue.h"
#include "../../../src/utils/String16.h"

using testing::Eq;

namespace {

    class SqliteValue_Test : public testing::Test {
    protected:
        static void SetUpTestCase() {

        }

        static void TearDownTestCase() {

        }
    };
}

TEST_F(SqliteValue_Test, testOperatorLong) {
    bool exceptionThrown = false;
    SqliteValue longValue(10L);
    long testValueL = longValue;
    ASSERT_EQ(testValueL, 10L);
    longValue = 123456789L;
    ASSERT_EQ(longValue, 123456789L);
    try {
        longValue = 12.3456789;
    } catch (SqliteException &e) {
        exceptionThrown = std::strcmp(e.what(), "SqliteValue has wrong DataType: 'Double'") == 0;
    }
    ASSERT_TRUE(exceptionThrown) << "SqliteValue assignment with wrong type must throw a SqliteException";
}

TEST_F(SqliteValue_Test, testOperatorDouble) {
    bool exceptionThrown = false;
    SqliteValue doubleValue(10.123);
    double testValueL = doubleValue;
    ASSERT_EQ(testValueL, 10.123);
    doubleValue = 12.3456789;
    ASSERT_EQ(doubleValue, 12.3456789);
    try {
        doubleValue = 12234L;
    } catch (SqliteException &e) {
        exceptionThrown = std::strcmp(e.what(), "SqliteValue has wrong DataType: 'Long'") == 0;
    }
    ASSERT_TRUE(exceptionThrown) << "SqliteValue assignment with wrong type must throw a SqliteException";
}

TEST_F(SqliteValue_Test, testOperatorString) {
    bool exceptionThrown = false;
    SqliteValue stringValue(String16("test string"));
    String16 stringValueTest = stringValue;
    ASSERT_EQ(stringValueTest, String16("test string"));
    stringValue = String16("test string2");
    ASSERT_EQ(stringValue, String16("test string2"));
    try {
        stringValue = 12.3456789;
    } catch (SqliteException &e) {
        exceptionThrown = std::strcmp(e.what(), "SqliteValue has wrong DataType: 'Double'") == 0;
    }
    ASSERT_TRUE(exceptionThrown) << "SqliteValue assignment with wrong type must throw a SqliteException";
}

TEST_F(SqliteValue_Test, testOperatorNull) {
    SqliteValue nullValue;
    ASSERT_TRUE(nullValue.isNull());

    ASSERT_TRUE(nullValue == 0L);
    long testLong = nullValue;
    ASSERT_TRUE(testLong == 0L);

    ASSERT_TRUE(nullValue == 0.0);
    double testDouble = nullValue;
    ASSERT_TRUE(testDouble == 0.0);

}