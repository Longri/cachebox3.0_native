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
 * Created by Longri on 02.09.18.
 */

#ifndef CACHEBOX_SQLITECURSOR_H
#define CACHEBOX_SQLITECURSOR_H

extern "C" {
#include "../../sqlite_src/sqlite3.h"
}

#include <vector>
#include "../exceptions/IndexOutOfBoundsException.h"
#include "../String16.h"
#include "SqliteException.h"

class SqliteCursor {
private:
    sqlite3_stmt *pStmt;
    int rc = -1;
    std::vector<String16> columnNames;
    std::vector<int> columnTypes;

    int resolveIndex(String16 columnName, bool throwException, const char *function, long lineNumber) {
        int idx = 0;
        for (String16 name:columnNames) {
            if (name == columnName)return idx;
            idx++;
        }

        if (throwException) {
            String16 msg("No such column name:");
            msg.append(columnName);
            throw SqliteException(String16::toUtf8_Chars(msg), __FILE__, function, lineNumber);
        }
        return -1;
    }

    bool chkClosed(bool throwException) {
        if (rc < 0) {
            // cursor is closed
            if (throwException) {
                throw SqliteException("Cursor is closed", __FILE__, __FUNCTION__, __LINE__);
            }
            return true;
        }
        return false;
    }

public:

    explicit SqliteCursor(sqlite3_stmt *pStmt) : pStmt(pStmt) {}

    void moveToFirst() {
        next();
    }

    void next() {
        rc = sqlite3_step(pStmt);
        if (rc == SQLITE_DONE) {
            rc = -2;
        } else {
            //read columnName and type after row changed
            columnNames.clear();
            columnTypes.clear();
            int colCount = sqlite3_column_count(pStmt);
            for (int colIndex = 0; colIndex < colCount; colIndex++) {
                columnNames.push_back(String16(sqlite3_column_name(pStmt, colIndex)));
                columnTypes.push_back(sqlite3_column_type(pStmt, colIndex));
            }
        }
    }

    void close() {
        //release resources
        columnNames.clear();
        std::vector<String16>().swap(columnNames);
        columnTypes.clear();
        std::vector<int>().swap(columnTypes);
        rc = sqlite3_finalize(pStmt);
    }

    bool isAfterLast() {
        return rc == -2;
    }

    long getInt(const char *strId) {
        return getInt(resolveIndex(String16(strId), true, __FUNCTION__, __LINE__));
    }

    long getInt(long colIndex) {
        chkClosed(true);
        checkIndex(colIndex);
        if (columnTypes[colIndex] != SQLITE_INTEGER) {
            throw SqliteException("Wrong column type!", __FILE__, __FUNCTION__, __LINE__);
        }
        return sqlite3_column_int64(pStmt, colIndex);
    }

    double getDouble(const char *strId) {
        return getDouble(resolveIndex(String16(strId), true, __FUNCTION__, __LINE__));
    }

    double getDouble(long colIndex) {
        chkClosed(true);
        checkIndex(colIndex);
        if (columnTypes[colIndex] != SQLITE_FLOAT) {
            throw SqliteException("Wrong column type!", __FILE__, __FUNCTION__, __LINE__);
        }
        return sqlite3_column_double(pStmt, colIndex);
    }

    String16 getString(const char *strId) {
        return getString(resolveIndex(String16(strId), true, __FUNCTION__, __LINE__));
    }

    String16 getString(long colIndex) {
        chkClosed(true);
        checkIndex(colIndex);
        if (columnTypes[colIndex] != SQLITE_TEXT) {
            throw SqliteException("Wrong column type!", __FILE__, __FUNCTION__, __LINE__);
        }
        const char *bytes;
        bytes = (const char *) sqlite3_column_text(pStmt, colIndex);
        return String16(bytes);
    }

    bool isNull(const char *strId) {
        return isNull(resolveIndex(String16(strId), true, __FUNCTION__, __LINE__));
    }

    bool isNull(long colIndex) {
        chkClosed(true);
        checkIndex(colIndex);
        return (columnTypes[colIndex] == SQLITE_NULL);
    }

    long getColumnType(const char *columnName) {
        return getColumnType(resolveIndex(String16(columnName), false, __FUNCTION__, __LINE__));
    }

    long getColumnType(long idx) {
        return sqlite3_column_type(pStmt, static_cast<int>(idx));
    }

    void checkIndex(long idx) {
        if (columnNames.size() < idx) {
            String16 msg("column index is out of bounds: ");
            msg.appendLong(idx);
            throw IndexOutOfBoundsException(String16::toUtf8_Chars(msg), __FILE__, __FUNCTION__, __LINE__);
        }
    }

    String16 getColumnName(long idx) {
        chkClosed(true);
        resolveIndex(String16(), false, __FUNCTION__, __LINE__);
        checkIndex(idx);
        return columnNames[idx];
    }

    /**
     * Returns the index of the given column name, <br>
     * or -1 if column not exist!
     *
     * @param columnName the name of the target column.
     * @return index of the column by name
     */
    int getColumnIndex(String16 columnName) {
        chkClosed(true);
        return resolveIndex(columnName, false, __FUNCTION__, __LINE__);
    }

};


#endif //CACHEBOX_SQLITECURSOR_H
