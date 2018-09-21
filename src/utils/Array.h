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

#include <vector>
#include <algorithm>
#include "../MathUtils.h"
#include "exceptions/IllegalStateException.h"
#include "exceptions/IndexOutOfBoundsException.h"
#include "StringObject.h"

#ifndef CACHEBOX_ARRAY_H
#define CACHEBOX_ARRAY_H


template<class T>
class Array {

private:


public:
    std::vector<T> items;

    /**
     * TODO comment
     */
    Array<T>() {
        Array(16);
    }

    /**
     * TODO comment
     * @param count
     */
    explicit Array<T>(long count) {
        items = std::vector<T>();
    }


    void add(T value) {
//        if (size == sizeof(values)) values = resize(MathUtils::max(8, (int) (size * 1.75f)));
        items.push_back(value);
    }


    void dispose() {
        //TODO dispose all values
    }

    T get(long i) {
        if (i >= items.size()) {
            String16 msg("index can't be >= size: ");
            msg.appendLong(i);
            msg.append(" >= ");
            msg.appendLong(items.size());
            throw IndexOutOfBoundsException(String16::toUtf8_Chars(msg), __FILE__, __FUNCTION__, __LINE__);
        }
        return items[i];
    }

    /** Removes and returns the last item. */
    T pop() {
        if (items.size() == 0) throw IllegalStateException("Array is empty.", __FILE__, __FUNCTION__, __LINE__);
        T item = items[items.size() - 1];
        items.pop_back();
        return item;
    }

    /** Returns the last item. */
    T peek() {
        if (items.size() == 0) throw new IllegalStateException("Array is empty.", __FILE__, __FUNCTION__, __LINE__);
        return items[items.size() - 1];
    }

    /** Returns the first item. */
    T first() {
        if (items.size() == 0) throw new IllegalStateException("Array is empty.", __FILE__, __FUNCTION__, __LINE__);
        return items[0];
    }

    /** Returns true if the array is empty. */
    bool isEmpty() {
        return items.size() == 0;
    }

    void clear() {
        items.clear();
    }

    long size() {
        return items.size();
    }

    String16 toString16() {
        if (items.size() == 0) return String16("[]");
        String16 buffer;
        buffer.append('[');
        buffer.append(items[0]);
        for (int i = 1; i < items.size(); i++) {
            buffer.append(", ");
            if (String16 *v = dynamic_cast<String16 *>(items[i])) {
                buffer.append(v);
            }
//            else if (StringObject *o = dynamic_cast<StringObject *>(items[i])) {
//                buffer.append(o->toString16());
//            } else {
//                buffer.append("items[");
//                buffer.append(String16(i));
//                buffer.append("]");
//            }
        }
        buffer.append(']');
        return buffer;
    }
};


#endif //CACHEBOX_ARRAY_H
