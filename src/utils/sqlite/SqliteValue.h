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
 * Created by Longri on 07.09.18.
 */

extern "C" {
#include "../../sqlite_src/sqlite3.h"
}

#include "SqliteException.h"
#include "../String16.h"

#ifndef CACHEBOX_SQLITEVALUE_H
#define CACHEBOX_SQLITEVALUE_H

/**
 *
 * ^(Every value in SQLite has one of five fundamental datatypes:
 *
 * <ul>
 * <li> 64-bit signed integer
 * <li> 64-bit IEEE floating point number
 * <li> string
 * <li> BLOB
 * <li> NULL
 * </ul>)^
 *
 * These constants are codes for each of those types.
 *
 * Note that the SQLITE_TEXT constant was also used in SQLite version 2
 * for a completely different meaning.  Software that links against both
 * SQLite version 2 and SQLite version 3 should use SQLITE3_TEXT, not
 * SQLITE_TEXT.
 *
 *  SQLITE_INTEGER  1
 *  SQLITE_FLOAT    2
 *  SQLITE_BLOB     4
 *  SQLITE_NULL     5
 *  SQLITE_TEXT     3
 */
class SqliteValue {
private:
    int sqliteType;
    long valueLong;
    double valueDouble;
    String16 valueString;


public:
//##############################################################################################################
//  this operator overload is neadet by google tes
//##############################################################################################################
    operator long long() const {
        if (sqliteType != SQLITE_INTEGER) {
            throw SqliteException("SqliteValue has wrong DataType: 'Long'", __FILE__, __FUNCTION__, __LINE__);
        }
        return valueLong;
    }

    operator long double() const {
        if (sqliteType != SQLITE_FLOAT) {
            throw SqliteException("SqliteValue has wrong DataType: 'Double'", __FILE__, __FUNCTION__, __LINE__);
        }
        return valueDouble;
    }

    friend bool operator==(const SqliteValue &v, long double vl) {
        if (v.sqliteType != SQLITE_FLOAT) {
            throw SqliteException("SqliteValue has wrong DataType: 'Long'", __FILE__, __FUNCTION__, __LINE__);
        }
        return v.valueDouble == vl;
    }

    friend bool operator==(long double vl, const SqliteValue &v) {
        if (v.sqliteType != SQLITE_FLOAT) {
            throw SqliteException("SqliteValue has wrong DataType: 'Long'", __FILE__, __FUNCTION__, __LINE__);
        }
        return v.valueDouble == vl;
    }

    friend bool operator==(const SqliteValue &v, long long vl) {
        if (v.sqliteType != SQLITE_INTEGER) {
            throw SqliteException("SqliteValue has wrong DataType: 'Long'", __FILE__, __FUNCTION__, __LINE__);
        }
        return v.valueLong == vl;
    }

    friend bool operator==(long long vl, const SqliteValue &v) {
        if (v.sqliteType != SQLITE_INTEGER) {
            throw SqliteException("SqliteValue has wrong DataType: 'Long'", __FILE__, __FUNCTION__, __LINE__);
        }
        return v.valueLong == vl;
    }

//##############################################################################################################
//  Implementation for Long 'SQLITE_NULL'
//##############################################################################################################
    SqliteValue() : sqliteType(SQLITE_NULL) {}

    bool isNull() const { return sqliteType == SQLITE_NULL; }

//##############################################################################################################
//  Implementation for Long 'SQLITE_INTEGER'
//##############################################################################################################
    SqliteValue(long newValue) : valueLong(newValue), sqliteType(SQLITE_INTEGER) {}

    operator long() const {
        if (sqliteType == SQLITE_NULL)return 0L;
        if (sqliteType != SQLITE_INTEGER) {
            throw SqliteException("SqliteValue has wrong DataType: 'Long'", __FILE__, __FUNCTION__, __LINE__);
        }
        return valueLong;
    }

    SqliteValue &operator=(long const &newValue) {
        if (sqliteType != SQLITE_INTEGER) {
            throw SqliteException("SqliteValue has wrong DataType: 'Long'", __FILE__, __FUNCTION__, __LINE__);
        }
        valueLong = newValue;
        return *this;
    }

    friend bool operator==(const SqliteValue &v, long vl) {
        if (v.isNull())return 0L == vl;
        if (v.sqliteType != SQLITE_INTEGER) {
            throw SqliteException("SqliteValue has wrong DataType: 'Long'", __FILE__, __FUNCTION__, __LINE__);
        }
        return v.valueLong == vl;
    }

    friend bool operator==(long vl, const SqliteValue &v) {
        if (v.isNull())return 0L == vl;
        if (v.sqliteType != SQLITE_INTEGER) {
            throw SqliteException("SqliteValue has wrong DataType: 'Long'", __FILE__, __FUNCTION__, __LINE__);
        }
        return v.valueLong == vl;
    }


//##############################################################################################################
//  Implementation for Double 'SQLITE_FLOAT'
//##############################################################################################################
    SqliteValue(double newValue) : valueDouble(newValue), sqliteType(SQLITE_FLOAT) {}

    operator double() const {
        if (sqliteType == SQLITE_NULL)return 0.0;
        if (sqliteType != SQLITE_FLOAT) {
            throw SqliteException("SqliteValue has wrong DataType: 'Long'", __FILE__, __FUNCTION__, __LINE__);
        }
        return valueDouble;
    }

    SqliteValue &operator=(double const &newValue) {
        if (sqliteType != SQLITE_FLOAT) {
            throw SqliteException("SqliteValue has wrong DataType: 'Double'", __FILE__, __FUNCTION__, __LINE__);
        }
        valueDouble = newValue;
        return *this;
    }

    friend bool operator==(const SqliteValue &v, double vl) {
        if (v.isNull())return 0.0 == vl;
        if (v.sqliteType != SQLITE_FLOAT) {
            throw SqliteException("SqliteValue has wrong DataType: 'Double'", __FILE__, __FUNCTION__, __LINE__);
        }
        return v.valueDouble == vl;
    }

    friend bool operator==(double vl, const SqliteValue &v) {
        if (v.isNull())return 0.0 == vl;
        if (v.sqliteType != SQLITE_FLOAT) {
            throw SqliteException("SqliteValue has wrong DataType: 'Double'", __FILE__, __FUNCTION__, __LINE__);
        }
        return v.valueDouble == vl;
    }


//##############################################################################################################
//  Implementation for String 'SQLITE_TEXT'
//##############################################################################################################
    SqliteValue(String16 newValue) : valueString(newValue), sqliteType(SQLITE_TEXT) {}

    operator String16() const {
        if (sqliteType != SQLITE_TEXT) {
            throw SqliteException("SqliteValue has wrong DataType: 'String'", __FILE__, __FUNCTION__, __LINE__);
        }
        return valueString;
    }

    SqliteValue &operator=(String16 const &newValue) {
        if (sqliteType != SQLITE_TEXT) {
            throw SqliteException("SqliteValue has wrong DataType: 'String'", __FILE__, __FUNCTION__, __LINE__);
        }
        valueString = newValue;
        return *this;
    }

    friend bool operator==(const SqliteValue &v, String16 vl) {
        if (v.sqliteType != SQLITE_TEXT) {
            throw SqliteException("SqliteValue has wrong DataType: 'String'", __FILE__, __FUNCTION__, __LINE__);
        }
        return v.valueString == vl;
    }

    friend bool operator==(String16 vl, const SqliteValue &v) {
        if (v.sqliteType != SQLITE_TEXT) {
            throw SqliteException("SqliteValue has wrong DataType: 'String'", __FILE__, __FUNCTION__, __LINE__);
        }
        return v.valueString == vl;
    }


};

#endif //CACHEBOX_SQLITEVALUE_H
