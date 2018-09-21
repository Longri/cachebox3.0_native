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
 * Created by Longri on 21.06.18.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../src/utils/Array.h"
#include "../../src/utils/String16.h"

using testing::Eq;

namespace {
    class Array_Test : public testing::Test {
    public:

    };
}

TEST_F(Array_Test, testConstructor) {

    Array<String16> strArr = Array<String16>();

    String16 str1 = String16("str1");
    String16 str2 = String16("str2");
    String16 str3 = String16("str3");
    String16 str4 = String16("str4");

    strArr.add(str1);
    strArr.add(str2);
    strArr.add(str3);
    strArr.add(str4);
    strArr.add(String16("str5"));

    ASSERT_EQ(strArr.size(), 5);


    String16 popStr1 = strArr.pop();
    ASSERT_EQ(String16("str5"), popStr1);
    ASSERT_EQ(strArr.size(), 4);

    String16 popStr2 = strArr.peek();
    ASSERT_EQ(str4, popStr2);
    ASSERT_EQ(strArr.size(), 4);


    String16 getStr2 = strArr.get(2);
    ASSERT_EQ(str3, getStr2);
    ASSERT_EQ(strArr.size(), 4);
}

