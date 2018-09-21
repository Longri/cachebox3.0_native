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

#ifndef CACHEBOX_MAP_H
#define CACHEBOX_MAP_H

#include <algorithm>
#include <vector>
#include "../MathUtils.h"


template<class K, class V>
class PointerMap {
private :

    K **keys;
    V **values;


    long size = 0;
    long space = 0;


    void resize(int newSize) {

        auto **newKeys = new K *[newSize];
        auto **newValues = new V *[newSize];

        memcpy(&newKeys[0], &this->keys[0], size * sizeof(K));
        memcpy(&newValues[0], &this->values[0], size * sizeof(V));

        for (long i = 0; i < size; i++) {
            keys[i] = NULL;
            values[i] = NULL;
        }

        delete[] keys;
        delete[] values;

        keys = newKeys;
        values = newValues;
    }

public:


    PointerMap<K, V>() {
        unsigned long count = 16;
        keys = new K *[count];
        values = new V *[count];
        space = count;
    }

    /**
     * TODO comment
     * @param count
     */
    explicit PointerMap<K, V>(unsigned long count) {
        keys = new K *[count];
        values = new V *[count];
        space = count;
    }

    ~PointerMap<K, V>() {
        //destroy values
        delete[] this->keys;
        delete[] this->values;
    }

    long put(K *key, V *value) {
        long index = indexOfKey(key);
        if (index == -1) {
            if (size == space) resize(MathUtils::max(8, (int) (size * 1.75f)));
            index = size++;
        }
        keys[index] = key;
        values[index] = value;
        return index;
    }

    long indexOfKey(K *key) {
        for (long i = 0, n = size; i < n; i++)
            if (key == keys[i]) return i;

        return -1;
    }

    V *get(K *key) {
        long idx = indexOfKey(key);
        return get(idx);
    }

    V *get(long idx) {
        if (idx >= 0) {
            V *ret = values[idx];
            return ret;
        }
        return NULL;
    }


};


#endif //CACHEBOX_MAP_H
