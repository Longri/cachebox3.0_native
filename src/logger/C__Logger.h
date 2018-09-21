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
 * Created by Longri on 01.07.18.
 */

#ifndef CACHEBOX_C_LOGGER_H
#define CACHEBOX_C_LOGGER_H

#include "Logger.h"
#include "../utils/FileHandle.h"

class C__Logger : public Logger {
public:
    C__Logger(const String16 &name) : Logger(name) {}

    virtual void finalLog(int &level, String16 &message, std::exception *exception,
                          boost::posix_time::ptime logtime, long mills, char const *file, char const *function,
                          long line) override {
        String16 buf = String16();

        // append date-time if so configured
        if (CONFIG.SHOW_DATE_TIME) {
            if (!CONFIG.DATE_TIME_FORMAT_STR.empty()) {
                String16 st = String16::getFormattedDate(logtime, CONFIG.DATE_TIME_FORMAT_STR);
                buf.append(st);
                buf.append(' ');
            } else {
                String16 st = String16(mills - START_TIME);
                buf.append(st);
                buf.append(' ');
            }
        }

        if (CONFIG.LEVEL_IN_BRACKETS)
            buf.append('[');

        // Append a readable representation of the log level
        switch (level) {
            case LOG_LEVEL_TRACE:
                buf.append("TRACE");
                break;
            case LOG_LEVEL_DEBUG:
                buf.append("DEBUG");
                break;
            case LOG_LEVEL_INFO:
                buf.append("INFO");
                break;
            case LOG_LEVEL_WARN:
                buf.append("WARN");
                break;
            case LOG_LEVEL_ERROR:
                buf.append("ERROR");
                break;
        }
        if (CONFIG.LEVEL_IN_BRACKETS)
            buf.append(']');
        buf.append(' ');

        // append the name of the log instance if so configured
        if (CONFIG.SHOW_LOG_NAME) {
            buf.append(name);
            buf.append(" - ");
        }
        buf.append(' ');


        if (exception != nullptr) {
            //append name of Exception with following linebreak
            if (StringException::instanceOf(exception)) {
                long pos = buf.length();
                auto stringException = dynamic_cast<StringException *>(exception);
                const char *className = stringException->getClassName();
                buf.append(className);
                buf.append(": ");
                buf.append(message);
                buf.append('\n');
                buf.append(' ', pos);
                buf.append(exception->what());
                buf.append('\n');
                buf.append(' ', pos);

                //resolve FilePath with help of FileHandle
                FileHandle fh(String16(stringException->getFileName()));
                buf.append(fh.getAbsolutePath());
                buf.append(',');
                String16 lineNumberStr(stringException->getLineNumber());
                buf.append(lineNumberStr);
                buf.append('\n');
                buf.append(' ', pos);
                String16 fileString(file);
#if defined _WIN32
                fileString.replaceAll(String16("\\"), String16("/"));
#endif
                buf.append(fileString);
                buf.append(',');
                String16 lineNumberString(line);
                buf.append(lineNumberString);
                buf.append('\n');
                buf.append(' ', pos);

            } else {
                buf.append(exception->what());
            }
        } else {
            // append the message
            buf.append(message);
        }


        //finally write to console
        std::cout << buf << std::endl;
    }

};


#endif //CACHEBOX_C_LOGGER_H
