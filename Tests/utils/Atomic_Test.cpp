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
 * Created by Longri on 05.08.18.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <thread>
#include "../../src/utils/AtomicBoolean.h"
#include "../../src/utils/AtomicInteger.h"

using testing::Eq;

namespace {
    class Atomic_Test : public testing::Test {
    public:

    };
}

TEST_F(Atomic_Test, test1) {
    AtomicInteger atomicInteger;
    atomicInteger.set(12);
    ASSERT_EQ(12, atomicInteger.get());
    ASSERT_EQ(24, atomicInteger.addAndGet(12));
    ASSERT_EQ(24, atomicInteger.getAndIncrement());
    ASSERT_EQ(25, atomicInteger.get());
    ASSERT_EQ(25, atomicInteger.getAndDecrement());
    ASSERT_EQ(24, atomicInteger.get());
    ASSERT_EQ(24, atomicInteger.getAndAdd(12));
    ASSERT_EQ(36, atomicInteger.get());
    ASSERT_EQ(37, atomicInteger.incrementAndGet());
    ASSERT_EQ(36, atomicInteger.decrementAndGet());
    ASSERT_EQ(48, atomicInteger.addAndGet(12));
    ASSERT_EQ(48, atomicInteger.get());
}


AtomicInteger ai;

void count() {
    for (int i = 0; i < 100000; ++i) {
        ai.incrementAndGet();
    }
}


TEST_F(Atomic_Test, threadTest) {
    ai.set(0);
    std::thread th1{count};
    std::thread th2{count};
    std::thread th3{count};
    th1.join();
    th2.join();
    th3.join();
    ASSERT_EQ(300000, ai.get());
}


AtomicInteger ai2;
AtomicBoolean ab1(false);
AtomicBoolean ab2(false);
AtomicBoolean ab3(false);

void count1() {
    for (int i = 0; i < 100000; ++i) {
        ai2.incrementAndGet();
    }
    ab1.set(true);
}

void count2() {
    for (int i = 0; i < 100000; ++i) {
        ai2.incrementAndGet();
    }
    ab2.set(true);
}

void count3() {
    for (int i = 0; i < 100000; ++i) {
        ai2.incrementAndGet();
    }
    ab3.set(true);
}


TEST_F(Atomic_Test, threadTestBool) {
    ai2.set(0);
    std::thread th1{count1};
    std::thread th2{count2};
    std::thread th3{count3};

    while (!ab1.get()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    while (!ab2.get()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    while (!ab3.get()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    ASSERT_EQ(300000, ai2.get());

    th1.join();
    th2.join();
    th3.join();
}