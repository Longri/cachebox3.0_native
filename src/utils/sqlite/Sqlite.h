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

#ifndef CACHEBOX_SQLITE_H
#define CACHEBOX_SQLITE_H

extern "C" {
#include "../../sqlite_src/sqlite3.h"
}

#include <vector>
#include <cstring>
#include <stdlib.h>
#include <cstdlib>
#include "SqliteException.h"
#include "SqliteCursor.h"
#include "../String16.h"
#include "../FileHandle.h"
#include "SqliteValue.h"

class Sqlite {
private:
    const char *path;
    sqlite3 *db = nullptr;

public:

    explicit Sqlite(FileHandle &handle) : path(handle.getUtf8Path()) {}

    //Destructor
    ~Sqlite() {
        delete[] path;
        closeDatabase();
    }

    sqlite3 *getSqlite3Pointer() {
        return db;
    }

    static const char *getSqliteVersion() {
        return sqlite3_libversion();
    }

    void openOrCreateDatabase() {
        if (db == nullptr) {
            if (sqlite3_open(path, &db) != SQLITE_OK) {
                throw SqliteException(sqlite3_errmsg(db), __FILE__, __FUNCTION__, __LINE__);
            }
        }
    }

    void execSQL(String16 string) {
        char *zErrMsg = 0;
        int rc;
        char *sql_bytes = String16::toUtf8_Chars(string);
        rc = sqlite3_exec(db, sql_bytes, 0, 0, &zErrMsg);
        free(sql_bytes);
        if (rc > 0) {
            throw SqliteException(zErrMsg, __FILE__, __FUNCTION__, __LINE__);
        }
    }

    void closeDatabase() {
        if (db != nullptr) {
            sqlite3_close(db);
            db = nullptr;
        }
    }

    long changes() {
        if (db != nullptr) {
            return sqlite3_changes(db);
        }
        return 0;
    }

    SqliteCursor rawQuery(String16 &string) {
        char *strArr = String16::toUtf8_Chars(string);
        SqliteCursor cur = rawQuery(strArr);
        delete[] strArr;
        return cur;
    }

    SqliteCursor rawQuery(const char *string) {

        sqlite3_stmt *stmt = nullptr;
        int rc = sqlite3_prepare_v2(db, string, -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            SqliteCursor cursor(stmt);
            return cursor;
        } else {
            throw SqliteException(sqlite3_errmsg(db), __FILE__, __FUNCTION__, __LINE__);
        }
    }

    void rawQuery(const char *string,
                  std::function<void(std::vector<String16>, std::vector<int>, std::vector<SqliteValue>,
                                     int)> const &callback) {
        sqlite3_stmt *pStmt = nullptr;
        int rc = sqlite3_prepare_v2(db, string, -1, &pStmt, nullptr);
        if (rc == SQLITE_OK) {
            std::vector<String16> columnNames;
            std::vector<int> columnTypes;
            std::vector<SqliteValue> values;

            // iterate statement and callback
            rc = sqlite3_step(pStmt);
            int count = 0;
            bool namesResolved = false;
            while (rc == SQLITE_ROW) {
                //read columnName and type after row changed
                columnTypes.clear();
                values.clear();
                int colCount = sqlite3_column_count(pStmt);
                for (int colIndex = 0; colIndex < colCount; colIndex++) {
                    if (!namesResolved)columnNames.push_back(String16(sqlite3_column_name(pStmt, colIndex)));
                    int columnType = sqlite3_column_type(pStmt, colIndex);
                    columnTypes.push_back(columnType);
                    switch (columnType) {
                        case SQLITE_INTEGER:
                            values.push_back(SqliteValue((long) sqlite3_column_int64(pStmt, colIndex)));
                            break;
                        case SQLITE_FLOAT:
                            values.push_back(SqliteValue((double) sqlite3_column_double(pStmt, colIndex)));
                            break;
                        case SQLITE_TEXT :
                            values.push_back(String16((const char *) sqlite3_column_text(pStmt, colIndex)));
                            break;
                        case SQLITE_NULL :
                            values.push_back(SqliteValue());
                            break;

                    }
                }
                callback(columnNames, columnTypes, values, count++);
                rc = sqlite3_step(pStmt);
                namesResolved = true;
            }
            rc = sqlite3_finalize(pStmt);

        } else {
            throw SqliteException(sqlite3_errmsg(db), __FILE__, __FUNCTION__, __LINE__);
        }
    }
};


#endif //CACHEBOX_SQLITE_H
