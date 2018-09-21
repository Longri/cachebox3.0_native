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
#include "../../src/utils/PointerMap.h"
#include "../../src/utils/String16.h"

using testing::Eq;

namespace {
    class PointerMap_Test : public testing::Test {
    public:

    };
}

TEST_F(PointerMap_Test, testConstructor) {

    PointerMap<String16, String16> strMap = PointerMap<String16, String16>(2);

    String16 strKey1 = String16("Key1");
    String16 strVal1 = String16("Val1");
    String16 strKey2 = String16("Key2");
    String16 strVal2 = String16("Val2");
    String16 strKey3 = String16("Key3");
    String16 strVal3 = String16("Val3");
    String16 strKey4 = String16("Key4");
    String16 strVal4 = String16("Val4");
    String16 strKey5 = String16("Key5");
    String16 strVal5 = String16("Val5");

    strMap.put(&strKey1, &strVal1);
    strMap.put(&strKey2, &strVal2);
    strMap.put(&strKey3, &strVal3);
    strMap.put(&strKey4, &strVal4);
    strMap.put(&strKey5, &strVal5);

    String16 *res = strMap.get(&strKey3);
    String16 strOfPtr1;
    strOfPtr1 = String16(res);

    ASSERT_EQ(strVal3, strOfPtr1);
    ASSERT_EQ(strVal5, String16(strMap.get(&strKey5)));


}



