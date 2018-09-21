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
 * Created by Longri on 11.06.18.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/Coordinate.h"

using testing::Eq;

namespace {
    class Coordinate_Test : public testing::Test {
    public:

    };
}

TEST_F(Coordinate_Test, testLatLonConstructor) {
    Coordinate coordinate = Coordinate(13.5, 12.3);
    ASSERT_THAT(13.5, testing::Eq(coordinate.getLatitude()));
    ASSERT_THAT(12.3, testing::Eq(coordinate.getLongitude()));
    ASSERT_TRUE(coordinate.isValid());
    ASSERT_FALSE(coordinate.isZero());
}

TEST_F(Coordinate_Test, testStringConstructorDecimal) {

    String16 coordStr = String16(u"12.321 / -15.124");
    String16 coordSTR = String16(coordStr);
    Coordinate coordinate = Coordinate(coordStr);

    // input coord String16 must not changed
    ASSERT_EQ(coordSTR, coordStr);

    ASSERT_THAT(12.321, testing::Eq(coordinate.getLatitude()));
    ASSERT_THAT(-15.124, testing::Eq(coordinate.getLongitude()));
    ASSERT_TRUE(coordinate.isValid());
    ASSERT_FALSE(coordinate.isZero());
}

TEST_F(Coordinate_Test, testStringConstructorDecimalMinute) {

    String16 coordStr = String16(u"N 48° 40.441 E 009° 23.470");
    String16 coordSTR = String16(coordStr);
    Coordinate coordinate = Coordinate(coordStr);

    // input coord String16 must not changed
    ASSERT_EQ(coordSTR, coordStr);

    EXPECT_NEAR(48.674017, coordinate.getLatitude(), 0.00001);
    EXPECT_NEAR(9.391167, coordinate.getLongitude(), 0.00001);
    ASSERT_TRUE(coordinate.isValid());
    ASSERT_FALSE(coordinate.isZero());

    coordStr = String16(u"S 48° 40.441 W 009° 23.470");
    coordSTR = String16(coordStr);
    coordinate = Coordinate(coordStr);

    // input coord String16 must not changed
    ASSERT_EQ(coordSTR, coordStr);

    EXPECT_NEAR(-48.674017, coordinate.getLatitude(), 0.00001);
    EXPECT_NEAR(-9.391167, coordinate.getLongitude(), 0.00001);
    ASSERT_TRUE(coordinate.isValid());
    ASSERT_FALSE(coordinate.isZero());
}

TEST_F(Coordinate_Test, testStringConstructorDecimalMinuteSecond) {

    String16 coordStr{u"52°34′49.7316″N 13°23′8.3364″E"};
    String16 coordSTR = String16(coordStr);
    Coordinate coordinate = Coordinate(coordStr);

    // input coord String16 must not changed
    ASSERT_EQ(coordSTR, coordStr);

    EXPECT_NEAR(52.580481, coordinate.getLatitude(), 0.00001);
    EXPECT_NEAR(13.385649, coordinate.getLongitude(), 0.00001);
    ASSERT_TRUE(coordinate.isValid());
    ASSERT_FALSE(coordinate.isZero());

    String16 coordStr2{u"52°34′49.7316″s 13°23′8.3364″W"};
    coordSTR = coordStr2;
    coordinate = Coordinate(coordStr2);

    // input coord String16 must not changed
    ASSERT_EQ(coordSTR, coordStr2);

    EXPECT_NEAR(-52.580481, coordinate.getLatitude(), 0.00001);
    EXPECT_NEAR(-13.385649, coordinate.getLongitude(), 0.00001);
    ASSERT_TRUE(coordinate.isValid());
    ASSERT_FALSE(coordinate.isZero());
}

TEST_F(Coordinate_Test, testCoordValidZero) {
    Coordinate coordinate = Coordinate(0, 0);
    ASSERT_THAT(0, testing::Eq(coordinate.getLatitude()));
    ASSERT_THAT(0, testing::Eq(coordinate.getLongitude()));
    ASSERT_FALSE(coordinate.isValid());
    ASSERT_TRUE(coordinate.isZero());

    coordinate = Coordinate(95, 0);
    ASSERT_THAT(95, testing::Eq(coordinate.getLatitude()));
    ASSERT_THAT(0, testing::Eq(coordinate.getLongitude()));
    ASSERT_FALSE(coordinate.isValid());
    ASSERT_FALSE(coordinate.isZero());

    coordinate = Coordinate(-95, 0);
    ASSERT_THAT(-95, testing::Eq(coordinate.getLatitude()));
    ASSERT_THAT(0, testing::Eq(coordinate.getLongitude()));
    ASSERT_FALSE(coordinate.isValid());
    ASSERT_FALSE(coordinate.isZero());

    coordinate = Coordinate(13.2, 197);
    ASSERT_THAT(13.2, testing::Eq(coordinate.getLatitude()));
    ASSERT_THAT(197, testing::Eq(coordinate.getLongitude()));
    ASSERT_FALSE(coordinate.isValid());
    ASSERT_FALSE(coordinate.isZero());

    coordinate = Coordinate(13.2, -197);
    ASSERT_THAT(13.2, testing::Eq(coordinate.getLatitude()));
    ASSERT_THAT(-197, testing::Eq(coordinate.getLongitude()));
    ASSERT_FALSE(coordinate.isValid());
    ASSERT_FALSE(coordinate.isZero());
}

TEST_F(Coordinate_Test, testProject) {
    double distance = 24;//meter
    Coordinate c1 = Coordinate(1, 1);
    Coordinate c2 = c1.project(12, distance);
    double calculatedDistance = c1.distance(c2);
    EXPECT_NEAR(distance, calculatedDistance,0.001);
}