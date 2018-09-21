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

#ifndef CACHEBOX_STRINGOBJECT_H
#define CACHEBOX_STRINGOBJECT_H

#include "Object.h"

/**
 *
 */
class StringObject : public Object {
private:
    const String16 stringValue;


public:


    explicit StringObject(const String16 &value) : stringValue(value), Object("StringObject") {}

    explicit StringObject(long value) : stringValue(value), Object("StringObject") {}

    explicit StringObject(int value) : stringValue(value), Object("StringObject") {}

    explicit StringObject(double value) : stringValue(value), Object("StringObject") {}

    explicit StringObject(const char *value) : stringValue(value), Object("StringObject") {}

    explicit StringObject( StringObject *pObject) : stringValue(pObject->stringValue), Object("StringObject") {}

    virtual String16 toString() override {
        return stringValue;
    }


    StringObject() : Object("StringObject") {

    }
};


#endif //CACHEBOX_STRINGOBJECT_H
