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

#ifndef CACHEBOX_ATOMICINTEGER_H
#define CACHEBOX_ATOMICINTEGER_H

#include <mutex>

class AtomicInteger {

private:
    mutable std::mutex mx;
    volatile long value;

public:

    AtomicInteger() : value(0) {}

    explicit AtomicInteger(long newValue) : value(newValue) {}

    long get() {
        std::lock_guard<std::mutex> guard{mx};
        return value;
    }

    void set(long newValue) {
        std::lock_guard<std::mutex> guard{mx};
        value = newValue;
    }


    /**
    * Atomically increments by one the current value.
    *
    * @return the previous value
    */
    long getAndIncrement() {
        std::lock_guard<std::mutex> guard{mx};
        return value++;
    }

    /**
     * Atomically decrements by one the current value.
     *
     * @return the previous value
     */
    long getAndDecrement() {
        std::lock_guard<std::mutex> guard{mx};
        return value--;
    }

    /**
     * Atomically adds the given value to the current value.
     *
     * @param delta the value to add
     * @return the previous value
     */
    long getAndAdd(int delta) {
        std::lock_guard<std::mutex> guard{mx};
        long oldValue = value;
        value += delta;
        return oldValue;
    }

    /**
     * Atomically increments by one the current value.
     *
     * @return the updated value
     */
    long incrementAndGet() {
        std::lock_guard<std::mutex> guard{mx};
        return ++value;
    }

    /**
     * Atomically decrements by one the current value.
     *
     * @return the updated value
     */
    long decrementAndGet() {
        std::lock_guard<std::mutex> guard{mx};
        return --value;
    }

    /**
     * Atomically adds the given value to the current value.
     *
     * @param delta the value to add
     * @return the updated value
     */
    long addAndGet(int delta) {
        std::lock_guard<std::mutex> guard{mx};
        value += delta;
        return value;
    }

};


#endif //CACHEBOX_ATOMICINTEGER_H
