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
 * Created by Longri on 25.07.18.
 */

#ifndef CACHEBOX_FILEREADER_H
#define CACHEBOX_FILEREADER_H

#include "Reader.h"
#include "String16.h"
#include "System.h"
#include<fstream>

class FileReader : public Reader {
private:
    static const long MIN_READ_BUFFER_SIZE = 1024;
    const char *path;
    bool isOpen = false;
    std::ifstream inputFileStream;
    std::streampos begin = -1;
    std::streampos end = -1;
    std::streampos act = -1;

    char16_t *charCache{};
    long charCacheSize = -1;

    char *byteCache{};
    long byteCacheSize = -1;

    static int isUtf8begin(char byte) {
        if (!(byte & 0x80)) { // 0xxx xxxx
            return 1;
        } else if ((byte & 0xE0) == 0xC0) { // 110x xxyy  10yy yyyy
            return 2;
        } else if ((byte & 0xF0) == 0xE0) { // 1110 xxxx  10xx xxyy  10yy yyyy
            return 3;
        } else if ((byte & 0xF8) == 0xF0) { // 1111 0xxx  10xx yyyy  10yy yyzz  10zz zzzz
            return 4;
        }
        return 0;
    }

public:
    explicit FileReader(const char *path) : path(path) {}

    ~FileReader() {
        close();
    }

    void open() {
        if (isOpen)return;
        inputFileStream.open(path, std::ios::binary);
        isOpen = true;

        //with open read file length and set read pos to 0
        inputFileStream.seekg(0, std::ios::end);
        end = inputFileStream.tellg();
        inputFileStream.seekg(0);
        act = inputFileStream.tellg();
        begin = inputFileStream.tellg();
    }

    void close() {
        if (!isOpen)return;
        inputFileStream.close();
        isOpen = false;

        //set act to -1 as signal to eof
        act = -1;

        // delete caches
        if (charCacheSize > 0) {
            delete[] charCache;
            charCacheSize = 0;
        }
        if (byteCacheSize > 0) {
            delete[] byteCache;
            byteCacheSize = 0;
        }
    }

    /**
    * Reads characters into a portion of an array.  This method will block
    * until some input is available, an I/O error occurs, or the end of the
    * stream is reached.
    *
    * @param      cbuf  Destination buffer
    * @param      off   Offset at which to start storing characters
    * @param      len   Maximum number of characters to read
    *
    * @return     The number of characters read, or -1 if the end of the
    *             stream has been reached
    */
    long read(char16_t *cbuf, long offset, long len) override {
        if (!isOpen)return 0;

        if (charCacheSize > 0) {
            if (charCacheSize >= len) {
                //return from cached chars
                System::arraycopy(charCache, 0, cbuf, offset, len);
                long rest = charCacheSize - len;
                if (rest > 0) {
                    auto *tmp = new char16_t[rest];;
                    System::arraycopy(charCache, len, tmp, 0, rest);
                    delete[] charCache;
                    charCache = tmp;
                    charCacheSize = rest;
                }
                return len;
            }
        }


        long cap = MathUtils::max(MIN_READ_BUFFER_SIZE, (len * 4) + 2);

        auto *output = new char[cap];//char32_t can up to 4 bytes + 1 byte '/0'
        std::fill(output, output + cap, '\0');
        inputFileStream.read(output, cap);
        long readLength = inputFileStream.gcount();

        //if byteCache size > 0 add byteCache to output
        if (byteCacheSize > 0) {
            auto *tmp = new char[cap + byteCacheSize];
            System::arraycopy(byteCache, 0, tmp, 0, byteCacheSize);
            System::arraycopy(output, 0, tmp, byteCacheSize, readLength);
            readLength += byteCacheSize;
            delete[] output;
            output = tmp;
            delete[]byteCache;
            byteCacheSize = -1;
        }


        if (readLength == 0) {
            // nothing to read
            // return charCache if filled
            if (charCacheSize > 0) {
                System::arraycopy(charCache, 0, cbuf, offset, charCacheSize);
                delete[] charCache;
                long retVal = charCacheSize;
                charCacheSize = -1;
                return retVal;
            }
            return -1;
        }

        // maybe the last four byte a splitted utf8 char
        // so remove this from array and return the size of bytes
        // they are handled!
        long utf8begin = 0;
        long readIdx = readLength - 1;
        long utf8end = readIdx;
        while (!isUtf8begin(output[utf8begin]))
            utf8begin++;

        while (true) {
            long i = isUtf8begin(output[utf8end]);
            if (i == 1) {
                break;
            }
            if (i > 1) {
                utf8end--;
                break;
            }
            utf8end--;
        }

        if (utf8end < readIdx) {
            byteCacheSize = readIdx - utf8end;
            byteCache = new char[byteCacheSize];
            System::arraycopy(output, utf8end + 1, byteCache, 0, byteCacheSize);
            readLength = utf8end + 1;
        }

        if (utf8begin > 0) {
//            // remove the bytes, they are not a char32
//            char tmp[readLength];
//            memcpy(&tmp[0], &output[utf8begin], static_cast<size_t>(readLength));
//            memcpy(&output[0], &tmp[0], static_cast<size_t>(readLength));
            throw StringException("should never come to pass", __FILE__, __func__, __LINE__);
        }

        //convert to char32_t
        output[readLength] = '\0';
        std::vector<char16_t> char16vec = String16::utf8ToChar16Vector(output);

        //if charCache size > 0 add charCache to char32vec
        if (charCacheSize > 0) {
            for (long i = charCacheSize - 1; i >= 0; --i) {
                char16vec.insert(char16vec.begin(), charCache[i]);
            }
            charCacheSize = -1;
            delete[] charCache;
        }


        auto charLength = MathUtils::min((long) char16vec.size(), len);

        //move read, but not used char32 to tmp vector
        if (charCacheSize < 0) {
            long unusedLength = char16vec.size() - len;
            if (unusedLength > 0) {
                charCache = new char16_t[unusedLength];
                charCacheSize = unusedLength;
                System::arraycopy(&char16vec, charLength, charCache, 0, unusedLength);
            }
        }

        System::arraycopy(&char16vec, 0, cbuf, offset, charLength);
        delete[] output;
        return charLength;
    }

    /**
     * Reads a complete file into a String16 and returns it!
     * @return
     */
    String16 readString() {
        bool mustClose = false;
        if (!isOpen) {
            mustClose = true;
            open();
        }

        long byteLength = (long) (end - begin);

        auto *CHARS = new char16_t[byteLength + 1];
        std::fill(CHARS, CHARS + byteLength + 1, '\0');
        read(CHARS, 0, byteLength);
        String16 retString(CHARS);
        if (mustClose)close();
        delete[] CHARS;
        return retString;
    }
};


#endif //CACHEBOX_FILEREADER_H
