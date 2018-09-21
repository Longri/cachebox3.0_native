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
 * Created by Longri on 14.07.18.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/utils/String16.h"
#include "../src/MathUtils.h"

using testing::Eq;

namespace {
    class MathUtils_Test : public testing::Test {
    public:

    };
}

TEST_F(MathUtils_Test, testToHexString) {
    long long int i = 0xAB8C;
    String16 expected = String16("0xAB8C");
    const char* chars = MathUtils::getHexString(i);
    String16 hex = String16(chars);
    delete[] chars;
    ASSERT_EQ(hex, expected);
}