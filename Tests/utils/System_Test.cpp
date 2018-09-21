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
 * Created by Longri on 28.07.18.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../src/utils/System.h"
#include "../../src/utils/String16.h"

using testing::Eq;

namespace {
    class System_Test : public testing::Test {
    public:
        static bool ASSERT_U_STREQ(char16_t *array1, const char16_t *array2) {
            String16 string1(array1);
            String16 string2(array2);
            return string1 == string2;
        }
    };
}

TEST_F(System_Test, arrayCopyforChar) {

    const char *charArray1 = "12test";
    char *charArray2 = new char[7];

    System::arraycopy(charArray1, 0, charArray2, 0, 7);
    ASSERT_STREQ(charArray2, "12test");

    System::arraycopy(charArray1, 0, charArray2, 4, 2);
    ASSERT_STREQ(charArray2, "12te12");

    System::arraycopy(charArray1, 2, charArray2, 0, 4);
    ASSERT_STREQ(charArray2, "test12");

}

TEST_F(System_Test, arrayCopyforConstChar) {

    const char *charArray1 = "12test";
    char *charArray2 = new char[7];

    System::arraycopy(charArray1, 0, charArray2, 0, 7);
    ASSERT_STREQ(charArray2, "12test");

    System::arraycopy(charArray1, 0, charArray2, 4, 2);
    ASSERT_STREQ(charArray2, "12te12");

    System::arraycopy(charArray1, 2, charArray2, 0, 4);
    ASSERT_STREQ(charArray2, "test12");

}

TEST_F(System_Test, arrayCopyforChar16) {

    const char16_t *charArray1 = u"12test";
    auto *charArray2 = new char16_t[7];

    System::arraycopy(charArray1, 0, charArray2, 0, 7);
    ASSERT_TRUE(ASSERT_U_STREQ(charArray2, u"12test"));

    System::arraycopy(charArray1, 0, charArray2, 4, 2);
    ASSERT_TRUE(ASSERT_U_STREQ(charArray2, u"12te12"));

    System::arraycopy(charArray1, 2, charArray2, 0, 4);
    ASSERT_TRUE(ASSERT_U_STREQ(charArray2, u"test12"));

}

TEST_F(System_Test, arrayCopyforChar32Vector) {

    auto *charArray1 = new std::vector<char16_t>();
    charArray1->push_back('1');
    charArray1->push_back('2');
    charArray1->push_back('t');
    charArray1->push_back('e');
    charArray1->push_back('s');
    charArray1->push_back('t');
    charArray1->push_back('\0');

    auto *charArray2 = new char16_t[7];

    System::arraycopy(charArray1, 0, charArray2, 0, 7);
    ASSERT_TRUE(ASSERT_U_STREQ(charArray2, u"12test"));

    System::arraycopy(charArray1, 0, charArray2, 4, 2);
    ASSERT_TRUE(ASSERT_U_STREQ(charArray2, u"12te12"));

    System::arraycopy(charArray1, 2, charArray2, 0, 4);
    ASSERT_TRUE(ASSERT_U_STREQ(charArray2, u"test12"));

}


