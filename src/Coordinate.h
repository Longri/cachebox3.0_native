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
 * Created by Longri on 11.06.18.
 */

#include "MathUtils.h"
#include "utils/String16.h"
#include <cmath>
#include <vector>

#ifndef CACHEBOX_COORDINATE_H
#define CACHEBOX_COORDINATE_H

static String16 EMPTY_STR = String16(u"");
static String16 WHITESPACE_STR = String16(u" ");
static String16 DOT_STR = String16(u".");
static String16 COMMA_STR = String16(u",");
static String16 ABB = String16(u"'");
static String16 MINUTE = String16(u"′");
static String16 SECOND = String16(u"″");
static String16 GRAD = String16(u"°");
static String16 AA = String16(u"\"");
static String16 BB = String16(u"\r");
static String16 CC = String16(u"\n");
static String16 SLASH = String16(u"/");

static char16_t NORTH_CHAR = 'N';
static char16_t SOUTH_CHAR = 'S';
static char16_t WEST_CHAR = 'W';
static char16_t EAST_CHAR = 'E';
static char16_t EAST_GERMAN_CHAR = 'O';


/**
 * Created by Longri on 11.06.18
 */
class Coordinate {
private:

    /**
     * Maximum possible latitude coordinate.
     */
    static constexpr double LATITUDE_MAX = 90;

    /**
     * Minimum possible latitude coordinate.
     */
    static constexpr double LATITUDE_MIN = -LATITUDE_MAX;

    /**
     * Maximum possible longitude coordinate.
     */
    static constexpr double LONGITUDE_MAX = 180;

    /**
     * Minimum possible longitude coordinate.
     */
    static constexpr double LONGITUDE_MIN = -LONGITUDE_MAX;


    double latitude;
    double longitude;


public:


    /**
     * Constructor for given latitude and longitude as double
     * @param latitude
     * @param longitude
     */
    Coordinate(double latitude, double longitude) {
        this->latitude = latitude;
        this->longitude = longitude;
    }

    explicit Coordinate(String16 &string) {

        String16 text = String16(string);


        text.toUpperCase();
        text.replace(u",", u".");

        //try utm
        {

        }

        text.replaceAll(&EAST_GERMAN_CHAR, &EAST_CHAR);
        text.replaceAll(ABB, WHITESPACE_STR);
        text.replaceAll(GRAD, WHITESPACE_STR);
        text.replaceAll(AA, EMPTY_STR);
        text.replaceAll(BB, EMPTY_STR);
        text.replaceAll(CC, EMPTY_STR);
        text.replaceAll(SLASH, EMPTY_STR);
        text.replaceAll(MINUTE, WHITESPACE_STR);
        text.replaceAll(SECOND, WHITESPACE_STR);

        double lat = 0;
        double lon = 0;
        int ilat = text.indexOf(NORTH_CHAR);
        if (ilat < 0)
            ilat = text.indexOf(SOUTH_CHAR);

        int ilon = text.indexOf(EAST_CHAR);
        if (ilon < 0)
            ilon = text.indexOf(WEST_CHAR);

        if (ilat < 0) {
            std::vector<String16> latlon = text.split(WHITESPACE_STR);
            if (latlon.size() == 2) {
                this->latitude = latlon[0].parseDouble();
                this->longitude = latlon[1].parseDouble();
                return;
            }
        }

        if (ilon < 0)
            return;
        if (ilat > ilon)
            return;
        text.replace(u"\u00B0", u"");

        //find new
        ilat = text.indexOf(NORTH_CHAR);
        if (ilat < 0)
            ilat = text.indexOf(SOUTH_CHAR);

        ilon = text.indexOf(EAST_CHAR);
        if (ilon < 0)
            ilon = text.indexOf(WEST_CHAR);


        char32_t dlat = text.charAt(ilat);
        char32_t dlon = text.charAt(ilon);

        String16 slat;
        String16 slon;

        if (ilat < 2) {
            slat = text.subString(ilat + 1, ilon - 1);
            slon = text.subString(ilon + 1, text.length());
        } else {
            slat = text.subString(0, ilat - 1);
            slon = text.subString(ilat + 1, ilon - 1);
        }

        std::vector<String16> llat = slat.split(WHITESPACE_STR);
        std::vector<String16> llon = slon.split(WHITESPACE_STR);


        if ((llat.size() == 1) && (llon.size() == 1)) {
            // Decimal
            lat = llat[0].parseDouble();
            lon = llon[0].parseDouble();
        } else if ((llat.size() == 2) && (llon.size() == 2)) {
            // Decimal Minute
            lat = llat[0].parseLong();
            lat += llat[1].parseDouble() / 60;
            lon = llon[0].parseLong();
            lon += llon[1].parseDouble() / 60;
        } else if ((llat.size() == 3) && (llon.size() == 3)) {
            // Decimal - Minute - Second
            lat = llat[0].parseLong();
            lat += llat[1].parseDouble() / 60;
            lat += llat[2].parseDouble() / 3600;
            lon = llon[0].parseLong();
            lon += llon[1].parseDouble() / 60;
            lon += llon[2].parseDouble() / 3600;
        } else {
            return;
        }

        if (dlat == SOUTH_CHAR)
            lat = -lat;
        if (dlon == WEST_CHAR)
            lon = -lon;

        this->latitude = lat;
        this->longitude = lon;

        llat.clear();
        llon.clear();
    }

    /**
     *
     * @return
     */
    double getLatitude() {
        return latitude;
    }

    /**
     *
     * @return
     */
    double getLongitude() {
        return longitude;
    }

    /**
     *
     * @param latitude
     */
    void setLatitude(double latitude) {
        this->longitude = latitude;
    }

    /**
     *
     * @param longitude
     */
    void setLongitude(double longitude) {
        this->longitude = longitude;
    }

    /**
     * A Coordinate is valid, if lat/lon in range and not 0,0!
     * 0,0 is in Range of max/min lat/lon, but we handle this as not valid
     *
     * @return
     */
    bool isValid() {
        if (latitude < LATITUDE_MIN || latitude > LATITUDE_MAX || latitude == 0) return false;
        return !(longitude < LONGITUDE_MIN || longitude > LONGITUDE_MAX || longitude == 0);
    }

    /**
     *
     * @return
     */
    bool isZero() {
        if (isValid())return false;
        return (latitude == 0) && (longitude == 0);
    }

    /**
     *
     * @param direction
     * @param distance
     * @return
     */
    Coordinate project(double direction, double distance) {
        double dist = distance / MathUtils::WGS84_MAJOR_AXIS; // convert dist to angular distance in radians
        double brng = direction * MathUtils::DEG_RAD; //
        double lat1 = getLatitude() * MathUtils::DEG_RAD;
        double lon1 = getLongitude() * MathUtils::DEG_RAD;
        double lat2 = asin(sin(lat1) * cos(dist) + cos(lat1) * sin(dist) * cos(brng));
        double lon2 = lon1 + atan2(sin(brng) * sin(dist) * cos(lat1), cos(dist) - sin(lat1) * sin(lat2));
        lon2 = fmod(lon2 + 3 * M_PI, 2 * M_PI) - M_PI; // normalise to -180°..+180°
        return Coordinate(lat2 * MathUtils::RAD_DEG, lon2 * MathUtils::RAD_DEG);
    }

    double distance(Coordinate other) {
        double lat1 = this->latitude * MathUtils::DEG_RAD;
        double lon1 = this->longitude * MathUtils::DEG_RAD;
        double lat2 = other.latitude * MathUtils::DEG_RAD;
        double lon2 = other.longitude * MathUtils::DEG_RAD;

        double distance = (MathUtils::WGS84_MAJOR_AXIS *
                           acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos((lon2 - lon1))));
        return distance;
    }

};


#endif //CACHEBOX_COORDINATE_H
