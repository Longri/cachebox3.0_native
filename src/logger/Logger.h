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

#ifndef CACHEBOX_LOGGER_H
#define CACHEBOX_LOGGER_H

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "../utils/String16.h"
#include "../utils/Object.h"
#include "../utils/StringObject.h"
#include <ctime>
#include <utility>
#include <list>

static const int LOG_LEVEL_TRACE = 0;
static const int LOG_LEVEL_DEBUG = 10;
static const int LOG_LEVEL_INFO = 20;
static const int LOG_LEVEL_WARN = 30;
static const int LOG_LEVEL_ERROR = 40;


static String16 _PATTERN = String16("{}");
static int CurrentLogLevel = LOG_LEVEL_DEBUG;

struct CONFIGStruct {
    bool SHOW_DATE_TIME = true;
    String16 DATE_TIME_FORMAT_STR = String16("dd-MM-yy hh:mm:ss-SSS");
    bool LEVEL_IN_BRACKETS = true;
    bool SHOW_SHORT_LOG_NAME = false;
    bool SHOW_LOG_NAME = true;
};


static long gettimeMillsNow() {
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration duration(now.time_of_day());
    long mills = duration.total_milliseconds();
    return mills;
}


static CONFIGStruct CONFIG;
static long START_TIME = gettimeMillsNow();

class Logger : public Object {

public :

    const String16 name;

    explicit Logger(const String16 &name) : name(name), Object("Logger") {}

    void debug(char const *var1) {
        std::vector<Object *> vec = std::vector<Object *>();
        String16 str = String16(var1);
        debug(vec, str);
    }

    void debug(char const *var1, Object *var2) {
        std::vector<Object *> vec = std::vector<Object *>();
        vec.push_back(var2);
        String16 str = String16(var1);
        debug(vec, str);
    }

    void debug(char const *var1, String16 *var2) {
        std::vector<Object *> vec = std::vector<Object *>();
        StringObject *strObj = new StringObject(String16(var2));
        vec.push_back(strObj);
        String16 str = String16(var1);
        debug(vec, str);
    }

    void debug(char const *var1, Object *var2, Object *var3) {
        std::vector<Object *> vec = std::vector<Object *>();
        vec.push_back(var2);
        vec.push_back(var3);
        String16 str = String16(var1);
        debug(vec, str);
    }

    void debug(char const *var1, const std::list<Object *> &args) {
        std::vector<Object *> vec = std::vector<Object *>();
        for (Object *obj:args) {
            vec.push_back(obj);
        }
        String16 str = String16(var1);
        debug(vec, str);
    }

    void debug(const String16 &var1) {
        std::vector<Object *> vec = std::vector<Object *>();
        debug(vec, var1);
    }

    void debug(const String16 &var1, Object *var2) {
        std::vector<Object *> vec = std::vector<Object *>();
        vec.push_back(var2);
        debug(vec, var1);
    }

    void debug(const String16 &var1, Object *var2, Object *var3) {
        std::vector<Object *> vec = std::vector<Object *>();
        vec.push_back(var2);
        vec.push_back(var3);
        debug(vec, var1);
    }

    void debug(String16 var1, const std::list<Object *> &args) {
        std::vector<Object *> vec = std::vector<Object *>();
        for (Object *obj:args) {
            vec.push_back(obj);
        }
        debug(vec, var1);
    }

    void debug(std::vector<Object *> vec, String16 var1) {
        if (!isLevelEnabled(LOG_LEVEL_DEBUG))return;
        int logLevel = LOG_LEVEL_DEBUG;
        String16 message = resolveMessage(var1, std::move(vec));
        logNow(logLevel, message, nullptr, nullptr, nullptr, 0);
    }

    void error(char const *var1) {
        std::vector<Object *> vec = std::vector<Object *>();
        String16 str = String16(var1);
        std::exception *ex = nullptr;
        error(str, vec, ex, nullptr, nullptr, 0);
    }

    void error(char const *var1, Object *var2) {
        std::vector<Object *> vec = std::vector<Object *>();
        vec.push_back(var2);
        String16 str = String16(var1);
        std::exception *ex = nullptr;
        error(str, vec, ex, nullptr, nullptr, 0);
    }

    void error(char const *var1, Object *var2, Object *var3) {
        std::vector<Object *> vec = std::vector<Object *>();
        vec.push_back(var2);
        vec.push_back(var3);
        String16 str = String16(var1);
        std::exception *ex = nullptr;
        error(str, vec, ex, nullptr, nullptr, 0);
    }

    void error(char const *var1, const std::list<Object *> &args) {
        std::vector<Object *> vec = std::vector<Object *>();
        for (Object *obj:args) {
            vec.push_back(obj);
        }
        String16 str = String16(var1);
        std::exception *ex = nullptr;
        error(str, vec, ex, nullptr, nullptr, 0);
    }

    void error(String16 &var1) {
        std::vector<Object *> vec = std::vector<Object *>();
        std::exception *ex = nullptr;
        error(var1, vec, ex, nullptr, nullptr, 0);
    }

    void error(String16 &var1, Object *var2) {
        std::vector<Object *> vec = std::vector<Object *>();
        vec.push_back(var2);
        std::exception *ex = nullptr;
        error(var1, vec, ex, nullptr, nullptr, 0);
    }

    void error(String16 &var1, Object *var2, Object *var3) {
        std::vector<Object *> vec = std::vector<Object *>();
        vec.push_back(var2);
        vec.push_back(var3);
        std::exception *ex = nullptr;
        error(var1, vec, ex, nullptr, nullptr, 0);
    }

    void error(String16 &var1, const std::list<Object *> &args) {
        std::vector<Object *> vec = std::vector<Object *>();
        for (Object *obj:args) {
            vec.push_back(obj);
        }
        std::exception *ex = nullptr;
        error(var1, vec, ex, nullptr, nullptr, 0);
    }


    void error(char const *var1, std::exception *ex, char const *file, char const *function, long line) {
        std::vector<Object *> vec = std::vector<Object *>();
        String16 str = String16(var1);
        error(str, vec, ex, file, function, line);
    }

    void error(char const *var1, Object *var2, std::exception *ex, char const *file, char const *function, long line) {
        std::vector<Object *> vec = std::vector<Object *>();
        vec.push_back(var2);
        String16 str = String16(var1);
        error(str, vec, ex, file, function, line);
    }

    void error(char const *var1, Object *var2, Object *var3, std::exception *ex, char const *file, char const *function,
               long line) {
        std::vector<Object *> vec = std::vector<Object *>();
        vec.push_back(var2);
        vec.push_back(var3);
        String16 str = String16(var1);
        error(str, vec, ex, file, function, line);
    }

    void
    error(char const *var1, const std::list<Object *> &args, std::exception *ex, char const *file, char const *function,
          long line) {
        std::vector<Object *> vec = std::vector<Object *>();
        for (Object *obj:args) {
            vec.push_back(obj);
        }
        String16 str = String16(var1);
        error(str, vec, ex, file, function, line);
    }

    void error(String16 &var1, std::exception *ex, char const *file, char const *function, long line) {
        std::vector<Object *> vec = std::vector<Object *>();
        error(var1, vec, ex, file, function, line);
    }

    void error(String16 &var1, Object *var2, std::exception *ex, char const *file, char const *function, long line) {
        std::vector<Object *> vec = std::vector<Object *>();
        vec.push_back(var2);
        error(var1, vec, ex, file, function, line);
    }

    void error(String16 &var1, Object *var2, Object *var3, std::exception *ex, char const *file, char const *function,
               long line) {
        std::vector<Object *> vec = std::vector<Object *>();
        vec.push_back(var2);
        vec.push_back(var3);
        error(var1, vec, ex, file, function, line);
    }

    void
    error(String16 &var1, const std::list<Object *> &args, std::exception *ex, char const *file, char const *function,
          long line) {
        std::vector<Object *> vec = std::vector<Object *>();
        for (Object *obj:args) {
            vec.push_back(obj);
        }
        error(var1, vec, ex, file, function, line);
    }

    void error(String16 &var1, std::vector<Object *> vec, std::exception *ex, char const *file, char const *function,
               long line) {
        if (!isLevelEnabled(LOG_LEVEL_ERROR))return;
        int logLevel = LOG_LEVEL_ERROR;
        String16 message = resolveMessage(var1, std::move(vec));
        logNow(logLevel, message, ex, file, function, line);
    }


    static bool isLevelEnabled(int logLevel) {
        // log level are numerically ordered so can use simple numeric
        // comparison
        return (logLevel >= CurrentLogLevel);
    }

    static String16 resolveMessage(String16 messagePattern, std::vector<Object *> argArray) {
        if (argArray.empty()) {
            return messagePattern;
        }

        int i = 0;
        String16 sbuf = String16();

        for (int L = 0; L < argArray.size(); ++L) {
            int j = messagePattern.indexOf(_PATTERN, i);
            if (j == -1) {
                if (i == 0) {
                    return messagePattern;
                }

                sbuf.append(messagePattern);
                return sbuf;
            }

            sbuf.append(messagePattern, i, j);
            String16 string = argArray[L]->toString();
            sbuf.append(string);
            i = j + 2;

        }
        if (i < messagePattern.length())
            sbuf.append(messagePattern, i);
        return sbuf;
    }

    void logNow(int logLevel, String16 &message, std::exception *exception, char const *file, char const *function,
                long line) {
        boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration duration(now.time_of_day());
        long mills = duration.total_milliseconds();
        finalLog(logLevel, message, exception, now, mills, file, function, line);
    }

    virtual void
    finalLog(int &level, String16 &message, std::exception *exception, boost::posix_time::ptime logtime, long mills,
             char const *file, char const *function, long line) =0;


    String16 getName() {
        return name;
    }
};


#endif //CACHEBOX_LOGGER_H
