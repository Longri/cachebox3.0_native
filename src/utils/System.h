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
 * Created by Longri on 28.07.18.
 */

#ifndef CACHEBOX_SYSTEM_H
#define CACHEBOX_SYSTEM_H

#include<string>
#include<vector>


class System {

public:

    /**
    * Copies an array from the specified source array, beginning at the
    * specified position, to the specified position of the destination array.
    * A subsequence of array components are copied from the source
    * array referenced by <code>src</code> to the destination array
    * referenced by <code>dest</code>. The number of components copied is
    * equal to the <code>length</code> argument. The components at
    * positions <code>srcPos</code> through
    * <code>srcPos+length-1</code> in the source array are copied into
    * positions <code>destPos</code> through
    * <code>destPos+length-1</code>, respectively, of the destination
    * array.
    *
    * @param      src      the source array.
    * @param      srcPos   starting position in the source array.
    * @param      dest     the destination array.
    * @param      destPos  starting position in the destination data.
    * @param      length   the number of array elements to be copied.
    */
    static void arraycopy(char *src, long srcPos, char *dest, long destPos, long length) {
        memcpy(&dest[destPos], &src[srcPos], static_cast<size_t>(length));
    }

    static void arraycopy(const char *src, long srcPos, char *dest, long destPos, long length) {
        memcpy(&dest[destPos], &src[srcPos], static_cast<size_t>(length));
    }

    static void arraycopy(const char16_t *src, long srcPos, char16_t *dest, long destPos, long length) {
        memcpy(&dest[destPos], &src[srcPos], static_cast<size_t>(length * sizeof(char16_t)));
    }

    static void arraycopy(std::vector<char16_t> *src, long srcPos, char16_t *dest, long destPos, long length) {
        memcpy(&dest[destPos], &src->data()[srcPos], static_cast<size_t>(length * sizeof(char16_t)));
    }

    static void arraycopy(std::vector<char16_t> *src, long srcPos, std::vector<char16_t> *dest, long destPos, long length) {
        memcpy(&dest->data()[destPos], &src->data()[srcPos], static_cast<size_t>(length * sizeof(char16_t)));
    }

};


#endif //CACHEBOX_SYSTEM_H
