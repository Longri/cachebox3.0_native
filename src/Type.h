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
 * Created by Longri on 16.06.18.
 */

#ifndef CACHEBOX_TYPE_H
#define CACHEBOX_TYPE_H


#include "utils/String16.h"

enum class Type {

    UNDEFINID = -1,

    //Cache types

    TRADITIONAL = 2,
    MULTI_CACHE = 3,
    VIRTUAL = 4,
    LETTERBOX_HYBRID = 5,
    EVENT = 6,
    MYSTERY_UNKNOWN = 8,
    PROJECT_A_P_E = 9,
    WEBCAM = 11,
    CACHE_IN_TRASH_OUT_EVENT = 13,
    EARTH_CACHE = 137,
    GPS_ADVENTURES_EXHIBIT = 1304,
    WHEREIGO = 1858,
    GEOCACHING_HQ = 3773,
    GIGA_EVENT = 7005,



    //Additional Waypoint types

    PARKING_AREA = 217,
    VIRTUAL_STAGE = 218,
    PHYSICAL_STAGE = 219,
    FINAL_LOCATION = 220,
    TRAILHEAD = 221,
    REFERENCE_POINT = 452
};

const Type UNDEFINID = Type::UNDEFINID;
const Type TRADITIONAL = Type::TRADITIONAL;
const Type MULTI_CACHE = Type::MULTI_CACHE;
const Type VIRTUAL = Type::VIRTUAL;
const Type LETTERBOX_HYBRID = Type::LETTERBOX_HYBRID;
const Type EVENT = Type::EVENT;
const Type MYSTERY_UNKNOWN = Type::MYSTERY_UNKNOWN;
const Type PROJECT_A_P_E = Type::PROJECT_A_P_E;
const Type WEBCAM = Type::WEBCAM;
const Type CACHE_IN_TRASH_OUT_EVENT = Type::CACHE_IN_TRASH_OUT_EVENT;
const Type EARTH_CACHE = Type::EARTH_CACHE;
const Type GPS_ADVENTURES_EXHIBIT = Type::GPS_ADVENTURES_EXHIBIT;
const Type WHEREIGO = Type::WHEREIGO;
const Type GEOCACHING_HQ = Type::GEOCACHING_HQ;
const Type GIGA_EVENT = Type::GIGA_EVENT;
const Type PARKING_AREA = Type::PARKING_AREA;
const Type VIRTUAL_STAGE = Type::VIRTUAL_STAGE;
const Type PHYSICAL_STAGE = Type::PHYSICAL_STAGE;
const Type FINAL_LOCATION = Type::FINAL_LOCATION;
const Type TRAILHEAD = Type::TRAILHEAD;
const Type REFERENCE_POINT = Type::REFERENCE_POINT;


const String16 NAME_UNDEFINID(u"UNDEFINID");
const String16 NAME_TRADITIONAL(u"TRADITIONAL");
const String16 NAME_MULTI_CACHE(u"MULTI_CACHE");
const String16 NAME_VIRTUAL(u"VIRTUAL");
const String16 NAME_LETTERBOX_HYBRID(u"LETTERBOX_HYBRID");
const String16 NAME_EVENT(u"EVENT");
const String16 NAME_MYSTERY_UNKNOWN(u"MYSTERY_UNKNOWN");
const String16 NAME_PROJECT_A_P_E(u"PROJECT_A_P_E");
const String16 NAME_WEBCAM(u"WEBCAM");
const String16 NAME_CACHE_IN_TRASH_OUT_EVENT(u"CACHE_IN_TRASH_OUT_EVENT");
const String16 NAME_EARTH_CACHE(u"EARTH_CACHE");
const String16 NAME_GPS_ADVENTURES_EXHIBIT(u"GPS_ADVENTURES_EXHIBIT");
const String16 NAME_WHEREIGO(u"WHEREIGO");
const String16 NAME_GEOCACHING_HQ(u"GEOCACHING_HQ");
const String16 NAME_GIGA_EVENT(u"GIGA_EVENT");
const String16 NAME_PARKING_AREA(u"PARKING_AREA");
const String16 NAME_VIRTUAL_STAGE(u"VIRTUAL_STAGE");
const String16 NAME_PHYSICAL_STAGE(u"PHYSICAL_STAGE");
const String16 NAME_FINAL_LOCATION(u"FINAL_LOCATION");
const String16 NAME_TRAILHEAD(u"TRAILHEAD");
const String16 NAME_REFERENCE_POINT(u"REFERENCE_POINT");

static String16 toString16(Type type) {
    switch (type) {
        case Type::UNDEFINID:
            return NAME_UNDEFINID;
            break;
        case Type::TRADITIONAL:
            return NAME_TRADITIONAL;
            break;
        case Type::MULTI_CACHE:
            return NAME_MULTI_CACHE;
            break;
        case Type::VIRTUAL:
            return NAME_VIRTUAL;
            break;
        case Type::LETTERBOX_HYBRID:
            return NAME_LETTERBOX_HYBRID;
            break;
        case Type::EVENT:
            return NAME_EVENT;
            break;
        case Type::MYSTERY_UNKNOWN:
            return NAME_MYSTERY_UNKNOWN;
            break;
        case Type::PROJECT_A_P_E:
            return NAME_PROJECT_A_P_E;
            break;
        case Type::WEBCAM:
            return NAME_WEBCAM;
            break;
        case Type::CACHE_IN_TRASH_OUT_EVENT:
            return NAME_CACHE_IN_TRASH_OUT_EVENT;
            break;
        case Type::EARTH_CACHE:
            return NAME_EARTH_CACHE;
            break;
        case Type::GPS_ADVENTURES_EXHIBIT:
            return NAME_GPS_ADVENTURES_EXHIBIT;
            break;
        case Type::WHEREIGO:
            return NAME_WHEREIGO;
            break;
        case Type::GEOCACHING_HQ:
            return NAME_GEOCACHING_HQ;
            break;
        case Type::GIGA_EVENT:
            return NAME_GIGA_EVENT;
            break;
        case Type::PARKING_AREA:
            return NAME_PARKING_AREA;
            break;
        case Type::VIRTUAL_STAGE:
            return NAME_VIRTUAL_STAGE;
            break;
        case Type::PHYSICAL_STAGE:
            return NAME_PHYSICAL_STAGE;
            break;
        case Type::FINAL_LOCATION:
            return NAME_FINAL_LOCATION;
            break;
        case Type::TRAILHEAD:
            return NAME_TRAILHEAD;
            break;
        case Type::REFERENCE_POINT:
            return NAME_REFERENCE_POINT;
            break;
    }
    return NAME_UNDEFINID;
}

static std::ostream &operator<<(std::ostream &os, Type &obj) {
    return os << toString16(obj);
}


#endif //CACHEBOX_TYPE_H

#ifndef CACHEBOX_TYPE_UTILS_H
#define CACHEBOX_TYPE_UTILS_H

class TypeUtils {

public:

    static Type TypeFromString16(String16 string) {
        string.toLowerCase();
        string.trim();
        if (string.contains(u"virtual cache")) {
            return VIRTUAL;
        } else if (string == String16(u"Cache In Trash Out Event")) {
            return CACHE_IN_TRASH_OUT_EVENT;
        } else {
            // remove trailing "-cache" fragments
            if (string.contains(u"-")) {
                string = string.subString(0, string.indexOf(u"-"));
                string.trim();
            }

            // remove trailing "Geocache|" fragments
            if (string.contains(u"|")) {
                string = string.subString(string.indexOf(u"|") + 1);
                string.trim();
            }

            if (string.contains(u"flag")) {
                if (string.contains(u"green"))
                    return REFERENCE_POINT;
            }

            // remove trailing " cache" or " hybrid" fragments
            if (string.contains(u" ")) {
                string = string.subString(0, string.indexOf(u" "));
                string.trim();
            }

            // Replace some opencaching.de / geotoad cache types
            if (string.contains(u"unknown"))
                return MYSTERY_UNKNOWN;
            if (string.contains(u"multicache"))
                return MULTI_CACHE;
            if (string.contains(u"whereigo"))
                return WHEREIGO;
            if (string.contains(u"other"))
                return MYSTERY_UNKNOWN;
            if (string.contains(u"earthcache"))
                return EARTH_CACHE;
            if (string.contains(u"webcam"))
                return WEBCAM;
//        if (string.contains(u"question"))
//            return MULTI_QUESTION;
            if (string.contains(u"reference"))
                return REFERENCE_POINT;
            if (string.contains(u"referenzpunkt"))
                return REFERENCE_POINT;
            if (string.contains(u"parking"))
                return PARKING_AREA;
            if (string.contains(u"stages"))
                return PHYSICAL_STAGE;
//        if (string.contains(u"mega"))
//            return MEGA_EVENT;
            if (string.contains(u"letterbox"))
                return LETTERBOX_HYBRID;
            if (string.contains(u"virtual"))
                return VIRTUAL_STAGE;
            if (string.contains(u"physical"))
                return PHYSICAL_STAGE;
        }
        return UNDEFINID;
    }
};

#endif //CACHEBOX_TYPE_UTILS_H