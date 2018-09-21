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
 * Created by Longri on 16.06.18.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/Type.h"

using testing::Eq;

namespace {
    class Type_Test : public testing::Test {
    public:

    };
}

TEST_F(Type_Test, testEquals) {
    Type t1 = Type::PARKING_AREA;
    Type t2 = Type::PARKING_AREA;
    ASSERT_EQ(t1, t2);
}

TEST_F(Type_Test, testSwitch) {
    Type t1 = Type::PARKING_AREA;

    switch (t1) {
        case Type::FINAL_LOCATION:
            ASSERT_TRUE(false);
            break;
        case Type::REFERENCE_POINT:
            ASSERT_TRUE(false);
            break;
        case Type::PARKING_AREA:
            ASSERT_TRUE(true);
            break;
        default:
            ASSERT_TRUE(false);
            break;
    }
}

TEST_F(Type_Test, testParseInt) {
    Type t1 = Type(11);
    ASSERT_EQ(t1, Type::WEBCAM);
}

TEST_F(Type_Test, testParseText) {
    Type t1 = TypeUtils::TypeFromString16((String16(u"  parking")));
    ASSERT_EQ(t1, Type::PARKING_AREA);
}

