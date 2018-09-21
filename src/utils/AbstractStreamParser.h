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

#ifndef CACHEBOX_ABSTRACTSTREAMPARSER_H
#define CACHEBOX_ABSTRACTSTREAMPARSER_H


#include "Reader.h"
#include "StringObject.h"
#include "AtomicBoolean.h"
#include "System.h"
#include "../logger/LoggerFactory.h"

class AbstractStreamParser {
protected:
    Logger *log = LoggerFactory::getLogger(String16("AbstractStreamParser"));

    const bool DEBUG = true;
    const int DEFAULT_BUFFER_LENGTH;


    double percent = 0;
    long actBufferLength;

    char16_t *buf;
    char16_t *tmp;


    void initial() {
        actBufferLength = DEFAULT_BUFFER_LENGTH;
        buf = new char16_t[actBufferLength];
        tmp = new char16_t[actBufferLength];
    }

public:

    AbstractStreamParser() : DEFAULT_BUFFER_LENGTH{512} {
        initial();
    }

    AbstractStreamParser(int def_buffer_length) : DEFAULT_BUFFER_LENGTH{def_buffer_length} {
        initial();
    }


    AtomicBoolean CANCELD;

    virtual long parseData(const char16_t *data, long length)=0;

    void parse(Reader *reader, long length) {

        log->debug("Start parsing");


        long readed = 0;
        long offset = 0;
        while (!CANCELD.get()) {

            if (offset < actBufferLength && actBufferLength > DEFAULT_BUFFER_LENGTH &&
                offset < DEFAULT_BUFFER_LENGTH) {
                actBufferLength = actBufferLength >> 1;
                if (DEBUG)
                    log->debug("can decrease buffer to {}", new StringObject(actBufferLength));
                buf = new char16_t[actBufferLength];
                System::arraycopy(tmp, 0, buf, 0, offset);
                tmp = new char16_t[actBufferLength];
            }

            if (offset == actBufferLength) {
                //must increase buffer size!
                actBufferLength = actBufferLength << 1;
                if (DEBUG)
                    log->debug("increase buffer to {}", new StringObject(actBufferLength));
                delete[] buf;
                buf = new char16_t[actBufferLength];
                System::arraycopy(tmp, 0, buf, 0, offset);
                delete[] tmp;
                tmp = new char16_t[actBufferLength];
            }

            long canReadLength = actBufferLength - offset;
            long readedLength = 0;
            bool fillBuffer = canReadLength > 0;
            long bufferFillLength = 0;
            while (fillBuffer) {
                readedLength = reader->read(buf, offset, canReadLength);
                if (readedLength < canReadLength && readedLength > -1) {
                    readed += readedLength;
                    offset += readedLength;
                    bufferFillLength += readedLength;
                    canReadLength = actBufferLength - offset;
                    if (canReadLength <= 0) {
                        fillBuffer = false;
                    }
                } else {
                    fillBuffer = false;
                }
            }


            percent = (float) readed / length * 100.0f;
            if (DEBUG)
                log->debug("Read Buffer: available {}/{} = {}%",
                           {new StringObject(readed), new StringObject(length), new StringObject(percent)});
            if (DEBUG) log->debug(String16(buf));

            long lastOffset = parseData(buf, bufferFillLength);
            if (readedLength == -1) break;
            offset = actBufferLength - lastOffset;

            if (offset == 0) {
                // clear buffer


                // fill with 0
                std::fill(buf, buf + actBufferLength, U'\0');
            } else {
                // move unhandled char's
                std::fill(tmp, tmp + actBufferLength, U'\0');
                System::arraycopy(buf, lastOffset, tmp, 0, offset);
                std::fill(buf, buf + actBufferLength, U'\0');
                System::arraycopy(tmp, 0, buf, 0, offset);
            }

            if (DEBUG) log->debug("Last Offset: {}", new StringObject(lastOffset));

        }
    }
};

#endif //CACHEBOX_ABSTRACTSTREAMPARSER_H
