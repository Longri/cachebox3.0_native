/*
 * Copyright 2018 Longri
 * Copyright 2018 Larsi
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
#include <cstring>
#include <locale>
#include <cstddef>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "exceptions/NumberFormatException.h"
#include "exceptions/ArithmeticException.h"
#include "../MathUtils.h"

extern "C" {
#include "../sqlite_src/sqlite3.h"
}


#ifndef CACHEBOX_STRING16_H
#define CACHEBOX_STRING16_H

template<class T>
char *toString(T arg) {
    std::string r;
    std::stringstream s;
    s << arg;
    r = s.str();
    auto *bytes = new char[32];
    memcpy(bytes, r.data(), 32);
    return bytes;
}

static const int SIZE_OF = sizeof(char16_t);

static inline void cpy(char16_t *source, long sourceOffset, char16_t *target, long targetOffset, long length) {
    memcpy(&target[targetOffset], &source[sourceOffset], static_cast<size_t>(length * SIZE_OF));
}

static inline void cpy(const char16_t *source, long sourceOffset, char16_t *target, long targetOffset, long length) {
    memcpy(&target[targetOffset], &source[sourceOffset], static_cast<size_t>(length * SIZE_OF));
}

class String16 {

private:

    static const int INITIAL_CAPACITY = 6;
    char16_t *CHARS;
    long itemSize = 0;
    long size = 0;
    bool isNull = false;

    void ensureCapacity(long needed) {
        if (size + needed >= itemSize) {
            long newCapacity = size + needed;
            newCapacity = (int) (newCapacity * 1.75f);
            auto *newItems = new char16_t[newCapacity];
            cpy(CHARS, 0, newItems, 0, size);
            itemSize = newCapacity;
            free(CHARS);
            CHARS = newItems;
            CHARS[size] = TERMINATE;
        }
    }

    static bool isNumeric(char16_t *data, long offset) {
        return '0' <= data[offset] && data[offset] <= '9';
    }


public:

    static const char TERMINATE = U'\0';

    explicit String16(long long int lli) {
        char *value = toString(lli);
        std::vector<char16_t> vec = utf8ToChar16Vector(value);
        this->size = static_cast<long>(vec.size());
        this->itemSize = this->size + 1;
        this->CHARS = new char16_t[this->itemSize];
        cpy(vec.data(), 0, this->CHARS, 0, this->size);
        vec.clear();
        this->CHARS[size] = TERMINATE;
    }

    explicit String16(long l) {
        char *value = toString(l);
        std::vector<char16_t> vec = utf8ToChar16Vector(value);
        this->size = static_cast<long>(vec.size());
        this->itemSize = this->size + 1;
        this->CHARS = new char16_t[this->itemSize];
        cpy(vec.data(), 0, this->CHARS, 0, this->size);
        vec.clear();
        this->CHARS[size] = TERMINATE;
    }

    explicit String16(int i) {
        char *value = toString(i);
        std::vector<char16_t> vec = utf8ToChar16Vector(value);
        this->size = static_cast<long>(vec.size());
        this->itemSize = this->size + 1;
        this->CHARS = new char16_t[this->itemSize];
        cpy(vec.data(), 0, this->CHARS, 0, this->size);
        vec.clear();
        this->CHARS[size] = TERMINATE;
    }

    explicit String16(double d) {
        char *value = toString(d);
        std::vector<char16_t> vec = utf8ToChar16Vector(value);
        this->size = static_cast<long>(vec.size());
        this->itemSize = this->size + 1;
        this->CHARS = new char16_t[this->itemSize];
        cpy(vec.data(), 0, this->CHARS, 0, this->size);
        vec.clear();
        this->CHARS[size] = TERMINATE;
    }

    explicit String16(char16_t &c) {
        CHARS = new char16_t[INITIAL_CAPACITY];
        CHARS[0] = c;
        CHARS[1] = TERMINATE;
        itemSize = INITIAL_CAPACITY;
        size = 1;
    }

    explicit String16() {
        CHARS = new char16_t[INITIAL_CAPACITY];
        CHARS[0] = TERMINATE;
        itemSize = INITIAL_CAPACITY;
        size = 0;
    }

    explicit String16(const char16_t *value) {
        size_t SIZE_T = std::char_traits<char16_t>::length(value);
        this->size = static_cast<int>(SIZE_T);
        this->itemSize = this->size + 1;
        this->CHARS = new char16_t[this->itemSize];
        cpy(const_cast<char16_t *>(value), 0, this->CHARS, 0, this->size);
        this->CHARS[size] = TERMINATE;
    }

    String16(const char16_t *value, long length) {
        this->size = length;
        this->itemSize = this->size + 1;
        this->CHARS = new char16_t[this->itemSize];
        cpy(const_cast<char16_t *>(value), 0, this->CHARS, 0, this->size);
        this->CHARS[size] = TERMINATE;
    }

    explicit String16(const char *value) {
        std::vector<char16_t> vec = utf8ToChar16Vector(const_cast<char *>(value));
        this->size = static_cast<long>(vec.size());
        this->itemSize = this->size + 1;
        this->CHARS = new char16_t[this->itemSize];
        cpy(vec.data(), 0, this->CHARS, 0, this->size);
        vec.clear();
        this->CHARS[size] = TERMINATE;
    }

    explicit String16(String16 *other) {
        if (other == nullptr) {
            this->isNull = true;
            return;
        }
        if (other->isNull) {
            this->isNull = true;
            return;
        }
        this->size = other->size;
        this->itemSize = this->size + 1;
        this->CHARS = new char16_t[this->itemSize];
        cpy(other->CHARS, 0, this->CHARS, 0, size);
        this->CHARS[size] = TERMINATE;
    }

    String16(char16_t *string, long startIdx, long length) {
        this->size = length;
        this->itemSize = this->size + 1;
        this->CHARS = new char16_t[this->itemSize];
        cpy(string, startIdx, this->CHARS, 0, size);
        this->CHARS[size] = TERMINATE;
    }

    String16(const char16_t *string, long startIdx, long length) {
        this->size = length;
        if (size <= 0) {
            CHARS = new char16_t[INITIAL_CAPACITY];
            CHARS[0] = TERMINATE;
            itemSize = INITIAL_CAPACITY;
            size = 0;
        } else {
            this->itemSize = this->size + 1;
            this->CHARS = new char16_t[this->itemSize];
            cpy(string, startIdx, this->CHARS, 0, size);
            this->CHARS[size] = TERMINATE;
        }
    }

    String16(String16 &other, int startIdx, int length) {
        size = length;
        if (size <= 0) {
            CHARS = new char16_t[INITIAL_CAPACITY];
            CHARS[0] = TERMINATE;
            itemSize = INITIAL_CAPACITY;
            size = 0;
            return;
        }
        itemSize = this->size + 1;
        CHARS = new char16_t[itemSize];
        cpy(other.CHARS, startIdx, CHARS, 0, size);
        CHARS[size] = TERMINATE;
    }

//    String16(const String16 &other) = delete;

    String16(const String16 &other) {//copy constructor
        this->size = other.size;
        this->itemSize = this->size + 1;
        this->CHARS = new char16_t[this->itemSize];
        cpy(other.CHARS, 0, this->CHARS, 0, size);
        this->CHARS[size] = TERMINATE;
    }

    ~String16() {
        if (!isNull) {
            //destroy values
            delete[] this->CHARS;
            isNull = true;
        }
    }

    void set(String16 &other) {
        this->size = other.size;
        this->itemSize = this->size + 1;
        this->CHARS = new char16_t[this->itemSize];
        cpy(other.CHARS, 0, this->CHARS, 0, size);
        this->CHARS[size] = TERMINATE;
    }

    long length() {
        return this->size;
    }

    /**
     * Tests if this string ends with the specified suffix.
     *
     * @param   suffix   the suffix.
     * @return  {@code true} if the character sequence represented by the
     *          argument is a suffix of the character sequence represented by
     *          this object.
     */
    bool endsWith(String16 suffix) {
        return startsWith(suffix, size - suffix.length());
    }

    bool startsWith(const String16 &search) {
        return startsWith(search, 0);
    }

    bool startsWith(const String16 &search, long offset) {
        return indexOf(search, offset) == offset;
    }


    void append(const char *value) {
        std::vector<char16_t> vec = utf8ToChar16Vector(const_cast<char *>(value));
        for (char16_t i : vec)
            append(i);
    }

    void appendInt(int value) {
        char *chrs = toString(value);
        std::vector<char16_t> vec = utf8ToChar16Vector(chrs);
        for (char16_t i : vec)
            append(i);
    }

    void appendLong(long value) {
        char *chrs = toString(value);
        std::vector<char16_t> vec = utf8ToChar16Vector(chrs);
        for (char16_t i : vec)
            append(i);
    }

    void appendBool(bool value) {
        if (value) {
            append(u"true");
        } else {
            append(u"false");
        }
    }

    void appendDouble(double value) {
        char *chrs = toString(value);
        std::vector<char16_t> vec = utf8ToChar16Vector(chrs);
        for (char16_t i : vec)
            append(i);
    }


    void append(char16_t value) {
        this->ensureCapacity(1);
        this->CHARS[this->size++] = value;
        this->CHARS[this->size] = '\0';
    }

    void append(char16_t value, long count) {
        this->ensureCapacity(count);
        for (int i = 0; i < count; ++i) {
            this->CHARS[this->size++] = value;
        }
        this->CHARS[this->size] = '\0';
    }

    void append(String16 string) {
        append(string.CHARS);
    }

    void append(String16 *string) {
        if (string == nullptr) {
            append(u"null");
            return;
        }
        append(string->CHARS);
    }

    void append(const char16_t *value) {
        auto addLength = std::char_traits<char16_t>::length(value);
        ensureCapacity(addLength);
        for (int i = 0; i < addLength; i++) {
            this->CHARS[this->size + i] = (value[i]);
        }
        this->size += addLength;
        this->CHARS[this->size] = TERMINATE;
    }

    void append(String16 string, long startIndex) {
        append(string.CHARS, startIndex, string.size);
    }

    void append(String16 string, long startIndex, long endIndex) {
        append(string.CHARS, startIndex, endIndex);
    }

    void append(String16 *string, long startIndex, long endIndex) {
        append(string->CHARS, startIndex, endIndex);
    }

    void append(const char16_t *value, long startIndex, long endIndex) {
        auto addLength = (endIndex - startIndex);//+ 1;
        ensureCapacity(addLength);
        for (long i = startIndex; i <= endIndex; i++) {
            this->CHARS[this->size + (i - startIndex)] = (value[i]);
        }
        this->size += addLength;
        this->CHARS[this->size] = TERMINATE;
    }

    long indexOf(const char16_t *search) {
        String16 str = String16(search);
        return indexOf(str, 0);
    }

    long indexOf(char16_t c) {
        return indexOf(c, 0);
    }

    long indexOf(char16_t c, long fromIndex) {
        String16 searchStr = String16(c);
        return indexOf(searchStr, fromIndex);
    }

    long indexOf(const String16 &search) {
        return indexOf(search, 0);
    }

    long indexOf(const String16 &search, long fromIndex) {
        char16_t *source = this->CHARS;
        long sourceOffset = 0;
        long sourceCount = this->size;
        char16_t *target = search.CHARS;
        long targetOffset = 0;
        long targetCount = search.size;

        if (fromIndex >= sourceCount) {
            return (targetCount == 0 ? sourceCount : -1);
        }
        if (fromIndex < 0) {
            fromIndex = 0;
        }
        if (targetCount == 0) {
            return fromIndex;
        }

        char16_t first = target[targetOffset];
        long max = sourceOffset + (sourceCount - targetCount);

        for (long i = sourceOffset + fromIndex; i <= max; i++) {
            /* Look for first character. */
            if (source[i] != first) {
                while (++i <= max && source[i] != first);
            }

            /* Found first character, now look at the rest of v2 */
            if (i <= max) {
                long j = i + 1;
                long end = j + targetCount - 1;
                for (long k = targetOffset + 1; j < end && source[j]
                                                           == target[k]; j++, k++);

                if (j == end) {
                    /* Found whole storage. */
                    return i - sourceOffset;
                }
            }
        }
        return -1;
    }

    long lastIndexOf(char16_t character) {
        return lastIndexOf(character, this->size);
    }

    long lastIndexOf(char16_t character, long from) {
        long i = MathUtils::min(this->size, from);
        while (--i >= 0 && this->CHARS[i] != character);
        return i;
    }

    void replaceAll(String16 &search, String16 &target) {
        while (replace(search, target, 0) > 0);
    }

    void replaceAll(char16_t &search, char16_t &target) {
        while (replace(search, target, 0) > 0);
    }

    void replaceAll(const char16_t *search, const char16_t *target) {
        while (replace(search, target, 0) > 0);
    }

    void replaceAll(const String16 &search, const String16 &target) {
        while (replace(search, target, 0) > 0);
    }

    long replace(const String16 &search, const String16 &target) {
        return replace(search, target, 0);
    }

    long replace(char16_t search, char16_t target) {
        return replace(search, target, 0);
    }

    long replace(char16_t search, char16_t target, int fromIndex) {
        String16 searchStr = String16(search);
        String16 targetStr = String16(target);
        return replace(searchStr, targetStr, fromIndex);
    }

    long replace(const char16_t *search, const char16_t *target) {
        return replace(search, target, 0);
    }

    long replace(const char16_t *search, const char16_t *target, int fromIndex) {
        String16 searchStr = String16(search);
        String16 targetStr = String16(target);
        return replace(searchStr, targetStr, fromIndex);
    }

    long replace(const String16 &search, const String16 &target, int fromIndex) {
        long idx = indexOf(search, fromIndex);
        if (idx >= 0) {
            if (search.size == target.size) {
                if (search.size == 1) {
                    this->CHARS[idx] = target.CHARS[0];
                } else {
                    cpy(target.CHARS, 0, CHARS, idx, target.size);
                }
            } else {
                if (search.size < target.size) {
                    long sizeChange = target.size - search.size;
                    this->ensureCapacity(sizeChange);
                    long restLength = this->size - search.size - idx;
                    auto *rest = new char16_t[restLength];
                    long offset = ((idx + search.size));
                    long copyLength = restLength;
                    cpy(CHARS, offset, rest, 0, restLength);
                    cpy(target.CHARS, 0, CHARS, idx, size);
                    cpy(rest, 0, CHARS, idx + target.size, restLength);
                    delete[] rest;
                    size += sizeChange;
                    CHARS[size] = TERMINATE;
                } else {
                    cpy(target.CHARS, 0, CHARS, idx, target.size);
                    long sizeChange = search.size - target.size;
                    cpy(CHARS, idx + search.size, CHARS, idx + target.size, size - idx - sizeChange);
                    size -= sizeChange;
                    CHARS[size] = TERMINATE;
                }
            }
        }
        return idx;
    }

    void toUpperCase() {
        for (long i = this->size; i >= 0; i--) {
            this->CHARS[i] = static_cast<char16_t>(toupper(this->CHARS[i]));
        }
    }

    String16 *toLowerCase() {
        for (long i = this->size; i >= 0; i--) {
            CHARS[i] = static_cast<char16_t>(tolower(CHARS[i]));
        }
        return this;
    }

    bool contains(const char16_t *search) {
        return indexOf(search) >= 0;
    }

    bool contains(String16 search) {
        return indexOf(search) >= 0;
    }

    double parseDouble() {
        bool isNegative = false;
        int offset = 0;
        long length = size;
        bool hasDecimal = false;

        // check for operators
        switch (this->CHARS[offset]) {
            case u'+':
                offset++;
                length--;
                break;

            case u'-':
                offset++;
                length--;
                isNegative = true;
                break;

            case u'.':
                offset++;
                length--;
                hasDecimal = true;
                break;
            default:break;
        }

        double ip = 0.0, dp = 0.0;
        double fd = 1.0;

        for (long i = offset, n = length + offset; i < n; i++) {
            long digit = this->CHARS[i] - U'0';

            if (isNumeric(this->CHARS, i) && digit != U'0') {
                if (!hasDecimal) {
                    ip *= 10;
                    ip += digit;
                } else {
                    dp *= 10;
                    dp += digit;
                    fd *= 10;
                }
            } else if (this->CHARS[i] == U'.') {
                if (hasDecimal) {
                    String16 msg = String16("Number is malformed: \'");
                    msg.append(this, offset, length);
                    msg.append('\'');
                    throw NumberFormatException(String16::toUtf8_Chars(msg), __FILE__, __FUNCTION__, __LINE__);
                }
                hasDecimal = true;
            } else {
                String16 msg = String16("Number is malformed: \'");
                msg.append(this, offset, length);
                msg.append('\'');
                throw NumberFormatException(String16::toUtf8_Chars(msg), __FILE__, __FUNCTION__, __LINE__);
            }
        }
        dp = dp / fd;
        return isNegative ? (ip + dp) * -1 : ip + dp;
    }


    long parseLong() {
        return parseLong(this, 0, size);
    }

    static long parseLong(String16 *str, long index, long length) {
        bool isNegative = false;
        long offset = index;

        // check for operators
        switch (str->CHARS[offset]) {
            case u'+':
                offset++;
                length--;
                break;
            case u'-':
                offset++;
                length--;
                isNegative = true;
                break;
            default:
                break;
        }


        int ip = 0, lastIp = 0;
        for (long i = offset, n = length + offset; i < n; i++) {
            if (!isNumeric(str->CHARS, i)) {
                String16 msg = String16("Number is malformed: \'");
                msg.append(str, offset, length);
                msg.append('\'');
                throw NumberFormatException(String16::toUtf8_Chars(msg), __FILE__, __FUNCTION__, __LINE__);
            }

            long digit = str->CHARS[i] - U'0';
            ip *= 10;
            ip += digit;

            if (ip < lastIp) {
                String16 msg = String16("Number is overflow: \'");
                msg.append(str, offset, length);
                msg.append('\'');
                throw ArithmeticException(String16::toUtf8_Chars(msg), __FILE__, __FUNCTION__, __LINE__);
            }

            lastIp = ip;
        }
        return isNegative ? ip * -1 : ip;
    }

    long long parseLongLong() {
        bool isNegative = false;
        long offset = 0;
        long length = size;

        // check for operators
        switch (this->CHARS[offset]) {
            case U'+':
                offset++;
                length--;
                break;

            case U'-':
                offset++;
                length--;
                isNegative = true;
                break;
            default:
                break;
        }


        long long ip = 0, lastIp = 0;
        for (long i = offset, n = length + offset; i < n; i++) {
            if (!isNumeric(this->CHARS, i)) {
                String16 msg = String16("Number is malformed: \'");
                msg.append(this, offset, length);
                msg.append('\'');
                throw NumberFormatException(String16::toUtf8_Chars(msg), __FILE__, __FUNCTION__, __LINE__);
            }
            long digit = this->CHARS[i] - U'0';
            ip *= 10LL;
            ip += digit;

            if (ip < lastIp) {
                String16 msg = String16("Number is overflow: \'");
                msg.append(this, offset, length);
                msg.append('\'');
                throw ArithmeticException(String16::toUtf8_Chars(msg), __FILE__, __FUNCTION__, __LINE__);
            }
            lastIp = ip;
        }
        return isNegative ? ip * -1 : ip;
    }

    bool parseBool() {
        // TRUE.length=4 / FALSE.length=5
        if (size == 4) {
            if (this->CHARS[0] == U'T' || this->CHARS[0] == U't') {
                return true;
            }
        } else if (size == 5) {
            if (this->CHARS[0] == U'F' || this->CHARS[0] == U'f') {
                return false;
            }
        }
        String16 msg("Number is malformed: \'");
        msg.append(this, 0, this->size);
        msg.append('\'');
        throw NumberFormatException(String16::toUtf8_Chars(msg), __FILE__, __FUNCTION__, __LINE__);
    }

    std::vector<String16> split(const char16_t *regex) {
        return split(String16(regex));
    }

    std::vector<String16> split(String16 regex) {
        long length = regex.size;
        long startIdx = 0;
        long endIdx = 0;
        std::vector<String16> split = std::vector<String16>();
        while (startIdx < this->size) {
            endIdx = indexOf(regex, startIdx);
            if (startIdx == endIdx) {
                startIdx += length;
                continue;
            }
            if (endIdx < 0)endIdx = this->size;
            split.push_back(String16(this->CHARS, startIdx, endIdx - startIdx));
            startIdx = endIdx + length;
        }
        return split;
    }

    std::vector<String16> split(const char16_t regex) {
        long startIdx = 0;
        long endIdx = 0;
        std::vector<String16> split = std::vector<String16>();
        while (startIdx < this->size) {
            endIdx = indexOf(regex, startIdx);
            if (startIdx == endIdx) {
                startIdx++;
                continue;
            }
            if (endIdx < 0)endIdx = this->size;
            split.push_back(String16(this->CHARS, startIdx, endIdx - startIdx));
            startIdx = endIdx + 1;
        }
        return split;
    }

    /**
     * remove any leading and trailing whitespace.
     */
    void trim() {
        long len = this->length();
        long st = 0;
        while ((st < len) && (CHARS[st] <= U' ')) {
            st++;
        }
        while ((st < len) && (CHARS[len - 1] <= U' ')) {
            len--;
        }
        this->size = len - st;
        cpy(CHARS, st, CHARS, 0, size);
        CHARS[this->size] = TERMINATE;
    }

    char16_t charAt(int idx) {
        return this->CHARS[idx];
    }

    bool empty() {
        return this->size <= 0;
    }

    String16 subString(int startIdx, long endIdx) {
        return String16(this->CHARS, startIdx, endIdx - startIdx);
    }

    String16 subString(long startIdx) {
        return String16(this->CHARS, startIdx, this->size - startIdx);
    }

    /**
    * Return a pointer to a utf8 encoded char[]
    * You must delete this char[], after usage!
    * @param String
    * @return *toUtf8_Chars
    */
    static char *toUtf8_Chars(const String16 &string) {
        return toUtf8_Chars(string.CHARS, string.size);
    }

    /**
    * Return a pointer to a utf8 encoded char[]
    * You must delete this char[], after usage!
    * @param char16_t *chars
    * @return *toUtf8_Chars
    * Copyright 2018 Larsi
    */
    static char *toUtf8_Chars(char16_t *chars, long length) {
        std::vector<char> byteVector;
        for (unsigned long i = 0; i < length; i++) {
            char32_t unicode = chars[i];

            if (unicode >= 0xD800 && unicode <= 0xDBFF) {
                // we have two 16bit words for this char

                char32_t highMarker = 0xD800;  //high word marker 1101 10xx  xxxx xxxx
                char32_t lowMarker = 0xDCFF;    //low  word marker 1101 11xx  xxxx xxxx
                char32_t mask = 0x3FF;

                char32_t cHigh = chars[i++];
                char32_t clow = chars[i];

                if ((cHigh & highMarker) != highMarker || (clow & lowMarker) != lowMarker) {
                    // combine 10bit from high and low char16
                    unicode = (cHigh & mask) << 10;
                    unicode |= (clow & mask);
                    unicode += 0x10000;
                } else {
                    // wrong char!
                    // add replacement character ï¿½
                    byteVector.push_back((char) 0xEF);
                    byteVector.push_back((char) 0xBF);
                    byteVector.push_back((char) 0xBD);
                    continue;
                }
            }

            // get utf8 sequence from unicode
            if (!(unicode & 0xFFFFFF80)) { // 0000 0000  0000 0000  0000 0000  0xxx xxxx -> 0xxx xxxx
                // one byte unicode
                byteVector.push_back((unsigned char) unicode);
            } else if (!(unicode & 0xFFFFF800)) { // 0000 0000  0000 0000  0000 0xxx  xxyy yyyy -> 110x xxxx  10yy yyyy
                // two byte unicode with 11 bit length
                // e.g. 'Â®' 174 = [1010 1110]
                //   [xxx0 0010] 5 bit    (1100 0010)=> 194  (0xC2)
                //   [xx10 1110] 6 bit    (1010 1110)=> 174  (0xAE)
                byteVector.push_back(0xC0 | (unsigned char) ((unicode & 0x7C0) >> 6));
                byteVector.push_back(0x80 | (unsigned char) (unicode & 0x3F));
            } else if (!(unicode &
                         0xFFFF0000)) { // 0000 0000  0000 0000  xxxx yyyy  yyzz zzzz -> 1110 xxxx  10yy yyyy  10zz zzzz
                // three byte unicode with 16 bit length
                // e.g. 'ê¾' 41086 = [1010 0000  0111 1110]
                //   [xxxx 0000] 4 bit    (1110 1010)=> 234 (0xEA)
                //   [xx00 0010] 6 bit    (1000 0001)=> 129 (0x81)
                //   [xx11 1110] 6 bit    (1011 1110)=> 190 (0xBE)
                byteVector.push_back(0xE0 | (unsigned char) ((unicode & 0xF000) >> 12));
                byteVector.push_back(0x80 | (unsigned char) ((unicode & 0xFC0) >> 6));
                byteVector.push_back(0x80 | (unsigned char) (unicode & 0x3F));
            } else if (!(unicode &
                         0xFFE00000)) { // 0000 0000  000w wwxx  xxxx yyyy  yyzz zzzz -> 1111 0www  10xx xxxx  10yy yyyy  10zz zzzz
                // four byte unicode with 16 bit length
                // e.g. 'ðŸ˜€' 128512 = [0000 0001  1111 0110  0000 0000]
                //   [xxxx x000] 4 bit    (1111 0000)=> 240 (0xF0)
                //   [xx01 1111] 6 bit    (1001 1111)=> 159 (0x9F)
                //   [xx01 1000] 6 bit    (1001 1000)=> 152 (0x98)
                //   [xx00 0000] 6 bit    (1000 0000)=> 128 (0x80)
                byteVector.push_back(0xF0 | (unsigned char) ((unicode & 0x1C0000) >> 18)); // ###Schnitzer: falsch war 0xF8
                byteVector.push_back(0x80 | (unsigned char) ((unicode & 0x3F000) >> 12));
                byteVector.push_back(0x80 | (unsigned char) ((unicode & 0xFC0) >> 6));
                byteVector.push_back(0x80 | (unsigned char) (unicode & 0x3F));
            } else {
                // add replacement character ï¿½
                byteVector.push_back((char) 0xEF);
                byteVector.push_back((char) 0xBF);
                byteVector.push_back((char) 0xBD);
            }
        }


        auto *bytes = new char[byteVector.size() + 1];
        long idx = 0;
        for (char &c : byteVector)
            bytes[idx++] = c;

        //terminate
        bytes[idx] = '\0';

        return bytes;
    }

    /**
     *
     * @param chars
     * @return
     * Copyright 2018 Larsi
     */
    static std::vector<char16_t> utf8ToChar16Vector(char *chars) {
        auto vec = std::vector<char16_t>();
        unsigned long length = std::strlen(chars);

        for (long i = 0; i < length; i++) {
            // get unicode from utf8 sequence
            char32_t unicode = 0;
            unsigned char bytes = static_cast<unsigned char>(chars[i]);
            if (!(bytes & 0x80)) { // 0xxx xxxx
                unicode = static_cast<char16_t>(chars[i]);
            } else if ((bytes & 0xE0) == 0xC0) { // 110x xxyy  10yy yyyy
                unicode = (((char16_t) (chars[i] & 0x1F)) << 6) | ((char16_t) (chars[i + 1] & 0x3F));
                i += 1;
            } else if ((bytes & 0xF0) == 0xE0) { // 1110 xxxx  10xx xxyy  10yy yyyy
                unicode = (((char16_t) (chars[i] & 0x0F)) << 12) | (((char16_t) (chars[i + 1] & 0x3F)) << 6) |
                          ((char16_t) (chars[i + 2] & 0x3F));
                i += 2;
            } else if ((bytes & 0xF8) == 0xF0) { // 1111 0xxx  10xx yyyy  10yy yyzz  10zz zzzz
                unicode = (((char16_t) (chars[i] & 0x07)) << 18) | (((char16_t) (chars[i + 1] & 0x3F)) << 12) |
                          (((char16_t) (chars[i + 2] & 0x3F)) << 6) | ((char16_t) (chars[i + 3] & 0x3F));
                i += 3;
            }
            if (unicode != '\r') {
                if (unicode >= 0xFFFF || (unicode >= 0xD800 && unicode <= 0xDBFF)) {
                    // create two char16
                    char32_t tmp = unicode - 0x10000;
                    vec.push_back(static_cast<char16_t>((tmp >> 10) | 0xD800));
                    vec.push_back(static_cast<char16_t>((tmp & 0x3FF) | 0xDC00));

                } else {
                    vec.push_back((char16_t) unicode);
                }
            }

        }
        return vec;
    }

//##################################################################################################################
//  Object overrides
//##################################################################################################################

//    String toString()override{
//        return this;
//    }


//##################################################################################################################
//  operators
//##################################################################################################################

    friend std::ostream &operator<<(std::ostream &os, const String16 &obj) {
        char *chars = toUtf8_Chars(obj);
        os << chars;
        delete[] chars;
        return os;
    }

    friend bool operator==(const String16 &str1, const String16 &str2) {
        if (str1.size != str2.size)return false;
        for (long i = str1.size; i >= 0; i--)
            if (str1.CHARS[i] != str2.CHARS[i]) return false;
        return true;
    }

    bool equals(String16 *other) const {
        if (other->size != size)return false;
        for (long i = size; i >= 0; i--)
            if (other->CHARS[i] != CHARS[i]) return false;
        return true;
    }

    bool equals(const char16_t *other) const {
        size_t SIZE_T = std::char_traits<char16_t>::length(other);
        if (static_cast<int>(SIZE_T) != size)return false;
        for (long i = size; i >= 0; i--)
            if (other[i] != CHARS[i]) return false;
        return true;
    }

    bool equals(const char *other) const {
        String16 otherStr(other);
        return equals(&otherStr);
    }

    bool operator==(String16 *other) {
        if (other->size != size)return false;
        for (long i = size; i >= 0; i--)
            if (other->CHARS[i] != CHARS[i]) return false;
        return true;
    }

    bool operator==(const char *string) {
        String16 otherString(string);
        return otherString == this;
    }

    String16 &operator=(const String16 &other) {
        if (this == &other)return *this;
        this->size = other.size;
        this->itemSize = this->size + 1;
        this->CHARS = new char16_t[this->itemSize];
        cpy(other.CHARS, 0, this->CHARS, 0, size);
        this->CHARS[size] = TERMINATE;
        return *this;
    }

    static String16 getFormattedDate(boost::posix_time::ptime ptime, String16 &FORMAT_STRING) {
        String16 stringBuilder(FORMAT_STRING); //copy for replace
        long yearEnd = 0, yearStart = -1;
        long monthEnd = 0, monthStart = -1;
        long dayEnd = 0, dayStart = -1;
        long hourEnd = 0, hourStart = -1;
        long minuteEnd = 0, minuteStart = -1;
        long secondEnd = 0, secondStart = -1;
        long millisecondEnd = 0, millisecondStart = -1;

        parseDateStartEnd(FORMAT_STRING, yearEnd, yearStart, monthEnd, monthStart, dayEnd,
                          dayStart, hourEnd, hourStart, minuteEnd, minuteStart, secondEnd,
                          secondStart, millisecondEnd, millisecondStart);


        const boost::posix_time::time_duration &timeDuration = ptime.time_of_day();
        const boost::gregorian::date &date = ptime.date();


        if (yearStart >= 0) {
            String16 year(date.year());
            long offset = 0;
            long length = year.length();
            if (yearEnd - yearStart == 2) {
                offset += 2;
                length -= 2;
            }
            cpy(year.CHARS, offset, stringBuilder.CHARS, yearStart, length);
        }

        if (monthStart >= 0) {
            String16 month(date.month());
            if (month.length() == 1) {
                String16 st("0");
                st.append(month);
                month = st;
            }
            cpy(month.CHARS, 0, stringBuilder.CHARS, monthStart, month.length());
        }

        if (dayStart >= 0) {
            String16 day(date.day());
            if (day.length() == 1) {
                String16 st("0");
                st.append(day);
                day = st;
            }
            cpy(day.CHARS, 0, stringBuilder.CHARS, dayStart, day.length());
        }

        if (hourStart >= 0) {
            String16 hour(timeDuration.hours());
            if (hour.length() == 1) {
                String16 st("0");
                st.append(hour);
                hour = st;
            }
            cpy(hour.CHARS, 0, stringBuilder.CHARS, hourStart, hour.length());
        }

        if (minuteStart >= 0) {
            String16 minute(timeDuration.minutes());
            if (minute.length() == 1) {
                String16 st("0");
                st.append(minute);
                minute = st;
            }
            cpy(minute.CHARS, 0, stringBuilder.CHARS, minuteStart, minute.length());
        }

        if (secondStart >= 0) {
            String16 second(timeDuration.seconds());
            if (second.length() == 1) {
                String16 st("0");
                st.append(second);
                second = st;
            }
            cpy(second.CHARS, 0, stringBuilder.CHARS, secondStart, second.length());
        }

        if (millisecondStart >= 0) {
            String16 millisecond = String16(timeDuration.fractional_seconds());
            long templateLength = millisecondEnd - millisecondStart;


            if (templateLength > millisecond.length()) {
                // fill with '0'

                for (long i = templateLength - 1, n = templateLength - millisecond.length(); i > n; i--) {
                    stringBuilder.CHARS[millisecondStart + i] = '0';
                }
                templateLength = millisecond.length();
            }

            cpy(millisecond.CHARS, 0, stringBuilder.CHARS, millisecondStart, templateLength);
        }

        return stringBuilder;
    }

    static boost::posix_time::ptime getPtime(String16 *dateTime, String16 &FORMAT_STRING) {

        long year = 0, yearEnd = 0, yearStart = -1;
        long month = 0, monthEnd = 0, monthStart = -1;
        long day = 0, dayEnd = 0, dayStart = -1;
        long hour = 0, hourEnd = 0, hourStart = -1;
        long minute = 0, minuteEnd = 0, minuteStart = -1;
        long second = 0, secondEnd = 0, secondStart = -1;
        long millisecond = 0, millisecondEnd = 0, millisecondStart = -1;

        parseDateStartEnd(FORMAT_STRING, yearEnd, yearStart, monthEnd, monthStart, dayEnd,
                          dayStart, hourEnd, hourStart, minuteEnd, minuteStart, secondEnd,
                          secondStart, millisecondEnd, millisecondStart);


        //        try { catch parse long exceptions
        year = parseLong(dateTime, yearStart, yearEnd - yearStart);
        month = parseLong(dateTime, monthStart, monthEnd - monthStart);
        day = parseLong(dateTime, dayStart, dayEnd - dayStart);
        hour = parseLong(dateTime, hourStart, hourEnd - hourStart);
        minute = parseLong(dateTime, minuteStart, minuteEnd - minuteStart);
        second = parseLong(dateTime, secondStart, secondEnd - secondStart);
        millisecond = parseLong(dateTime, millisecondStart, millisecondEnd - millisecondStart);


        boost::gregorian::date d;
        boost::posix_time::time_duration td;
        try {
            d = boost::gregorian::date(year, month, day);
            td = boost::posix_time::time_duration(hour, minute, second,
                                                  millisecond);//01:02:03.000004 when resolution is micro seconds
        } catch (std::out_of_range e) {

            //log error and return now

            return boost::posix_time::microsec_clock::local_time();
        }

        boost::posix_time::ptime ret = boost::posix_time::ptime(d, td);

        return ret;
    }


    static void parseDateStartEnd(String16 &FORMAT_STRING, long &yearEnd, long &yearStart, long &monthEnd,
                                  long &monthStart, long &dayEnd, long &dayStart, long &hourEnd, long &hourStart,
                                  long &minuteEnd, long &minuteStart, long &secondEnd, long &secondStart,
                                  long &millisecondEnd, long &millisecondStart) {


        bool dayInYear = false;
        bool amPmMarker = false;
        bool pm = false;
        bool hourZeroIndex = false;
        bool unescape = false;
        long unescapeIndex = 0;
        long parseLength = 0;


        for (int i = 0; i < FORMAT_STRING.length(); i++) {

            if (unescape) {
                if (FORMAT_STRING.CHARS[i] == '\'') {
                    unescape = false;
                    unescapeIndex++;
                }
                continue;
            }

            switch (FORMAT_STRING.CHARS[i]) {
                case 'y':
                case 'Y':
                    if (yearStart == -1) yearStart = i - unescapeIndex;
                    else yearEnd = i - unescapeIndex + 1;
                    break;
                case 'M':
                    if (monthStart == -1) monthStart = i - unescapeIndex;
                    else monthEnd = i - unescapeIndex + 1;
                    break;
//                case 'D':
//                    dayInYear = true;
                case 'd':
                    if (dayStart == -1) dayStart = i - unescapeIndex;
                    else dayEnd = i - unescapeIndex + 1;
                    break;
                case 'a':
                    if (!amPmMarker) amPmMarker = true;
//                    else pm = true;
                    break;
//                case 'H':
//                case 'K':
//                    hourZeroIndex = true;
                case 'k':
                case 'h':
                    if (hourStart == -1) hourStart = i - unescapeIndex;
                    else hourEnd = i - unescapeIndex + 1;
                    break;
                case 'm':
                    if (minuteStart == -1) minuteStart = i - unescapeIndex;
                    else minuteEnd = i - unescapeIndex + 1;
                    break;
                case 's':
                    if (secondStart == -1) secondStart = i - unescapeIndex;
                    else secondEnd = i - unescapeIndex + 1;
                    break;
                case '\'':
                    unescape = true;
                    unescapeIndex++;
                    break;
                case 'S':
                    if (millisecondStart == -1) millisecondStart = i - unescapeIndex;
                    else millisecondEnd = i - unescapeIndex + 1;
                    break;
                default:
                    break;
            }
        }
    }

};

#endif //CACHEBOX_STRING16_H
