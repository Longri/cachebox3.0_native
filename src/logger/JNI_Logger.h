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

#ifndef CACHEBOX_JNI_LOGGER_H
#define CACHEBOX_JNI_LOGGER_H


#include "Logger.h"

class JNI_Logger : public Logger {
public:
    JNI_Logger(const String16 &name) : Logger(name) {}

    virtual void finalLog(int &level, String16 &message, std::exception *exception,
                          boost::posix_time::ptime logtime, long mills, char const *file, char const *function, long line) override {
        std::cout << "JNI Logger" << std::endl;
    }


};


#endif //CACHEBOX_JNI_LOGGER_H
