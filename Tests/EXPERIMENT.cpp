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
 * Created by Longri on 11.09.18.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/utils/String16.h"
#include "utils/StopWatch.h"
#include "../src/utils/FileHandle.h"
#include "../src/utils/sqlite/Sqlite.h"
#include <cstring>

using testing::Eq;

namespace {

    //resource FileHandle
    FileHandle testResourcesDir = FileHandle(FileHandle::getWorkingPath()).parent().child("Tests/TestResources");
    FileHandle testFolder = testResourcesDir.child("TestDir/Sqlite/testResources2");


    class EXPERIMENT : public testing::Test {
    protected:
        static void SetUpTestCase() {
            //delete old tests
            testFolder.deleteDirectory();
            testFolder.mkdirs();
        }

        static void TearDownTestCase() {
            //delete old tests
            testFolder.deleteDirectory();
//            ASSERT_FALSE(testFolder.isDirectory()) << "TestFolder must delete after Test";
        }

    };
}

TEST_F(EXPERIMENT, sqliteChar16) {
    FileHandle dbFileHandle = testFolder.child("createTest5.db3");
    Sqlite db(dbFileHandle);
    db.openOrCreateDatabase();

    String16 CREATE("CREATE TABLE COMPANY("
                  "ID INT PRIMARY KEY     NOT NULL,"
                  "NAME           TEXT    NOT NULL,"
                  "AGE            INT     NOT NULL,"
                  "ADDRESS        CHAR(50),"
                  "SALARY         REAL );");

    String16 INSERT("INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
                  "VALUES (1, '€', 32, 'California', 20000.12 ); "
                  "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
                  "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "
                  "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
                  "VALUES (3, 'Teddy', 23, 'Norway', NULL );"
                  "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
                  "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );");

    db.execSQL(CREATE);
    db.execSQL(INSERT);

    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_prepare_v2(db.getSqlite3Pointer(), "SELECT * FROM COMPANY", -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        sqlite3_step(stmt);


        const char16_t *text16 = static_cast<char16_t const *>(sqlite3_column_text16(stmt, 1));
        
        
        
//        std::cout << text16;
    } else {
        ASSERT_TRUE(false) << "SQL Error:" << sqlite3_errmsg(db.getSqlite3Pointer()) << std::endl;
    }

}