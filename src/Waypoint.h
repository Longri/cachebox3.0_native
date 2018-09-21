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
 * Created by Longri on 17.06.18.
 */

#include "../src/Coordinate.h"
#include "../src/Type.h"

#ifndef CACHEBOX_WAYPOINT_H
#define CACHEBOX_WAYPOINT_H


class Waypoint : public Coordinate {

private:
    const long ID;


public:
    Waypoint(double latitude, double longitude,long id) : Coordinate(latitude, longitude), ID(id) {};

    Type type =Type::UNDEFINID;

    Type getType() const {
        return type;
    }

    void setType(Type type) {
        Waypoint::type = type;
    }

};


#endif //CACHEBOX_WAYPOINT_H
