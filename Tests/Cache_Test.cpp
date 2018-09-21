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
 * Created by Longri on 17.06.18.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/Waypoint.h"
#include "../src/Cache.h"

using testing::Eq;

namespace {
    class Cache_Test : public testing::Test {
    public:

    };
}

TEST_F(Cache_Test, testLatLonConstructor) {
    Cache cache = Cache(13.5, 12.3,1234567890);
    ASSERT_THAT(13.5, testing::Eq(cache.getLatitude()));
    ASSERT_THAT(12.3, testing::Eq(cache.getLongitude()));
    ASSERT_TRUE(cache.isValid());
    ASSERT_FALSE(cache.isZero());
}

