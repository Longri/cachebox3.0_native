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
 * Created by Longri on 19.08.18.
 */

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <iostream>


#ifndef CACHEBOX_STOPWATCH_H
#define CACHEBOX_STOPWATCH_H


class StopWatch {
private:
    const char *name;

    long _start;
    long _time;

    long getNow() {
        boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration duration(now.time_of_day());
        return duration.total_milliseconds();
    }

public:
    StopWatch(const char *name) : name(name) {
        _time = 0;
        _start = getNow();
    }

    void stop() {
        _time += getNow() - _start;
    }

    void start() {
        _start = getNow();
    }

    void reset() {
        _time = 0;
    }

    void print() {
        print("");
    }

    void print(const char *msg) {
        std::cout << name << ": " << msg << _time << "ms\n";
    }
};


#endif //CACHEBOX_STOPWATCH_H
