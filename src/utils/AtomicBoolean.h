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

#ifndef CACHEBOX_ATOMICBOOLEAN_H
#define CACHEBOX_ATOMICBOOLEAN_H

#include <mutex>

class AtomicBoolean {
private:
    mutable std::mutex mx;
    volatile bool value;

public:

    AtomicBoolean() : value(false) {}

    explicit AtomicBoolean(bool value) : value(value) {}

    bool get() {
        std::lock_guard<std::mutex> guard{mx};
        return value;
    }

    void set(bool newValue) {
        std::lock_guard<std::mutex> guard{mx};
        value = newValue;
    }
};


#endif //CACHEBOX_ATOMICBOOLEAN_H
