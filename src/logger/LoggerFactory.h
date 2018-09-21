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
 * Created by Longri on 03.07.2018
 */

#include "EmptyLogger.h"
#include "C__Logger.h"
#include "JNI_Logger.h"
#include "../utils/PointerMap.h"

#ifndef CACHEBOX_LOGGERFACTORY_H
#define CACHEBOX_LOGGERFACTORY_H


#define C_LOG
//#define JNI_LOG


static PointerMap<String16, Logger> loggerMap = PointerMap<String16, Logger>(16);

class LoggerFactory {


public:


    static Logger *getLogger(String16 name) {
        Logger *l = loggerMap.get(&name);

        if (l == NULL) {
#ifdef C_LOG
            C__Logger *log = new C__Logger(name);
#else
#ifdef JNI_LOG
            JNI_Logger* log = new JNI_Logger(message);
#else
            EmptyLogger *log = new EmptyLogger(message);
#endif
#endif
            loggerMap.put(&name, log);
            return loggerMap.get(&name);

        } else {
            return l;
        }
    }

};


#endif //CACHEBOX_LOGGERFACTORY_H
