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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../../../src/utils/FileHandle.h"
#include "../../../src/utils/sqlite/Sqlite.h"
#include "../../../src/utils/sqlite/SqliteCursor.h"
#include "../../../src/utils/AtomicInteger.h"

using testing::Eq;

namespace {

    //resource FileHandle
    FileHandle testResourcesDir = FileHandle(FileHandle::getWorkingPath()).parent().child("Tests/TestResources");
    FileHandle testFolder = testResourcesDir.child("TestDir/Sqlite/testResources1");

    class Sqlite_Test : public testing::Test {
    protected:
        static void SetUpTestCase() {
            //delete old tests
            testFolder.deleteDirectory();
            testFolder.mkdirs();
        }

        static void TearDownTestCase() {
            //delete old tests
            testFolder.deleteDirectory();
            ASSERT_FALSE(testFolder.isDirectory()) << "TestFolder must delete after Test";
        }
    };
}

TEST_F(Sqlite_Test, testConstructor) {

    ASSERT_TRUE(testFolder.isDirectory()) << "TestFolder must exist";

}

TEST_F(Sqlite_Test, getSqliteVersion) {
    const char *expected = "3.21.0";
    const char *version = Sqlite::getSqliteVersion();
    ASSERT_EQ(*expected, *version) << "SQLite version must be correct";
}

TEST_F(Sqlite_Test, openOrCreateDatabase) {

    FileHandle dbFileHandle = testFolder.child("createTest.db3");
    Sqlite db = Sqlite(dbFileHandle);
    db.openOrCreateDatabase();

    String16 CREATE = String16("CREATE TABLE Test (\n"
                           "    Id          INTEGER        NOT NULL\n"
                           "                               PRIMARY KEY AUTOINCREMENT,\n"
                           "    testName NVARCHAR (255)\n"
                           ");");

    db.execSQL(CREATE);
    db.closeDatabase();

    String16 dbFileString = dbFileHandle.readString();
    ASSERT_TRUE(dbFileString.startsWith(String16("SQLite format 3"))) << "\nDB file must created";


}

TEST_F(Sqlite_Test, openOrCreateDatabaseWithException) {
    // try if invalid path trow's a exception
    bool exceptionThrowed = false;
    FileHandle excFileHandle = testFolder.child("/xxxxx/xxxx/createTest.db3");
    Sqlite dbexc(excFileHandle);
    try {
        dbexc.openOrCreateDatabase();
    } catch (SqliteException &e) {
        exceptionThrowed = true;
    }
    ASSERT_TRUE(exceptionThrowed) << "Invalid DB path must throw a exception";
}

TEST_F(Sqlite_Test, execSQL) {

    FileHandle dbFileHandle = testFolder.child("createTest2.db3");
    Sqlite db(dbFileHandle);
    db.openOrCreateDatabase();

    String16 CREATE = String16("CREATE TABLE Test (\n"
                           "    Id          INTEGER        NOT NULL\n"
                           "                               PRIMARY KEY AUTOINCREMENT,\n"
                           "    testName NVARCHAR (255)\n"
                           ");");

    db.execSQL(CREATE);

    bool exceptionThrowed = false;
    try {
        db.execSQL(CREATE);
    } catch (SqliteException e) {
        exceptionThrowed = true;
    }
    ASSERT_TRUE(exceptionThrowed) << "Create a exist Table must throw a exception";

    db.closeDatabase();
}

TEST_F(Sqlite_Test, insertTest) {
    FileHandle dbFileHandle = testFolder.child("createTest3.db3");
    Sqlite db(dbFileHandle);
    db.openOrCreateDatabase();

    String16 CREATE("CREATE TABLE COMPANY("
                  "ID INT PRIMARY KEY     NOT NULL,"
                  "NAME           TEXT    NOT NULL,"
                  "AGE            INT     NOT NULL,"
                  "ADDRESS        CHAR(50),"
                  "SALARY         REAL );");

    String16 INSERT("INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
                  "VALUES (1, 'Paul', 32, 'California', 20000.00 ); ");


    String16 INSERT2("INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
                   "VALUES "
                   "(2, 'Allen', 25, 'Texas', 15000.00 ),"
                   "(3, 'Teddy', 23, 'Norway', 20000.00 ),"
                   "(4, 'Mark', 25, 'Rich-Mond ', 65000.00 );");

    String16 WRONG_INSERT("INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
                        "VALUES (1, 'Paul', 32, 'California', 20000.00 ); ");

    String16 WRONG_INSERT2("INSERT INTO COMPANY ID,NAME,AGE,ADDRESS,SALARY) "
                         "VALUES 5, 'Paul', 32, 'California', 20000.00 ); ");

    db.execSQL(CREATE);
    db.execSQL(INSERT);
    ASSERT_EQ(1, db.changes()) << "Changes must be one";

    db.execSQL(INSERT2);
    ASSERT_EQ(3, db.changes()) << "Changes must be three";

    bool exceptionThrowed = false;
    try {
        db.execSQL(WRONG_INSERT);
    } catch (SqliteException &e) {
        exceptionThrowed = true;
    }
    ASSERT_TRUE(exceptionThrowed) << "Insert a exist entry must throw a exception";

    exceptionThrowed = false;
    try {
        db.execSQL(WRONG_INSERT2);
    } catch (SqliteException &e) {
        exceptionThrowed = true;
    }
    ASSERT_TRUE(exceptionThrowed) << "Execute a invalid SQL statement must throw a exception";

    db.closeDatabase();
}

TEST_F(Sqlite_Test, rawQuery) {
    FileHandle dbFileHandle = testFolder.child("createTest4.db3");
    Sqlite db(dbFileHandle);
    db.openOrCreateDatabase();

    String16 CREATE("CREATE TABLE COMPANY("
                  "ID INT PRIMARY KEY     NOT NULL,"
                  "NAME           TEXT    NOT NULL,"
                  "AGE            INT     NOT NULL,"
                  "ADDRESS        CHAR(50),"
                  "SALARY         REAL );");

    String16 INSERT("INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
                  "VALUES (1, 'Paul', 32, 'California', 20000.12 ); "
                  "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
                  "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "
                  "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
                  "VALUES (3, 'Teddy', 23, 'Norway', NULL );"
                  "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
                  "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );");

    db.execSQL(CREATE);
    db.execSQL(INSERT);

    bool exceptionThrowed = false;
    try {
        SqliteCursor cursor = db.rawQuery("SELECT * FROM CHRISTMAS");
    } catch (SqliteException &e) {
        exceptionThrowed = true;
    }
    ASSERT_TRUE(exceptionThrowed) << "Query on not exist Table must throw a exception";


    SqliteCursor cursor = db.rawQuery("SELECT * FROM COMPANY");
//    ASSERT_TRUE(cursor.getCount() == 4) << "Cursor count must be 4";
    ASSERT_TRUE(!cursor.isAfterLast()) << "Cursor must not after last";
    exceptionThrowed = false;
    try {
        ASSERT_TRUE(cursor.getInt(0L) == 1) << "Cursor column 1 must be a Integer : 1";
    } catch (SqliteException &e) {
        exceptionThrowed = true;
    }
    ASSERT_TRUE(exceptionThrowed) << "Invalid cursor position must throw a exception";
    cursor.moveToFirst();

    ASSERT_TRUE(cursor.getColumnType(0L) == SQLITE_INTEGER) << "Cursor column 1 must be a SQLite Type : SQLITE_INTEGER";
    ASSERT_TRUE(cursor.getColumnType(1) == SQLITE_TEXT) << "Cursor column 2 must be a SQLite Type : SQLITE_TEXT";
    ASSERT_TRUE(cursor.getColumnType(2) == SQLITE_INTEGER) << "Cursor column 3 must be a SQLite Type : SQLITE_INTEGER";
    ASSERT_TRUE(cursor.getColumnType(3) == SQLITE_TEXT) << "Cursor column 4 must be a SQLite Type : SQLITE_TEXT";
    ASSERT_TRUE(cursor.getColumnType(4) == SQLITE_FLOAT) << "Cursor column 5 must be a SQLite Type : SQLITE_FLOAT";

    ASSERT_TRUE(cursor.getColumnName(0).equals("ID")) << "Cursor column name 1 must be: ID";
    ASSERT_TRUE(cursor.getColumnName(1).equals("NAME")) << "Cursor column name 2 must be: NAME";
    ASSERT_TRUE(cursor.getColumnName(2).equals("AGE")) << "Cursor column name 3 must be: AGE";
    ASSERT_TRUE(cursor.getColumnName(3).equals("ADDRESS")) << "Cursor column name 4 must be: ADDRESS";
    ASSERT_TRUE(cursor.getColumnName(4).equals("SALARY")) << "Cursor column name 5 must be: SALARY";

    ASSERT_TRUE(cursor.getInt("ID") == 1) << "Cursor column 1 must be a Integer : 1";
    ASSERT_TRUE(cursor.getString("NAME").equals("Paul")) << "Cursor column 2 must be a String : Paul";
    ASSERT_TRUE(cursor.getInt("AGE") == 32) << "Cursor column 3 must be a Integer : 32";
    ASSERT_TRUE(cursor.getString("ADDRESS").equals("California")) << "Cursor column 4 must be a String : California";
    ASSERT_TRUE(cursor.getDouble("SALARY") == 20000.12) << "Cursor column 5 must be a Double : 20000.12";

    ASSERT_TRUE(cursor.getInt(0L) == 1) << "Cursor column 1 must be a Integer : 1";
    ASSERT_TRUE(cursor.getString(1).equals("Paul")) << "Cursor column 2 must be a String : Paul";
    ASSERT_TRUE(cursor.getInt(2) == 32) << "Cursor column 3 must be a Integer : 32";
    ASSERT_TRUE(cursor.getString(3).equals("California")) << "Cursor column 4 must be a String : California";
    ASSERT_TRUE(cursor.getDouble(4) == 20000.12) << "Cursor column 5 must be a Double : 20000.12";


    cursor.next();
    ASSERT_TRUE(cursor.getInt(0L) == 2) << "Cursor column 1 must be a Integer : 2";
    ASSERT_TRUE(cursor.getString(1).equals("Allen")) << "Cursor column 2 must be a String : Allen";
    ASSERT_TRUE(cursor.getInt(2) == 25) << "Cursor column 3 must be a Integer : 25";
    ASSERT_TRUE(cursor.getString(3).equals("Texas")) << "Cursor column 4 must be a String : Texas";
    ASSERT_TRUE(cursor.getDouble(4) == 15000.00) << "Cursor column 5 must be a Double : 15000.00";

    cursor.next();
    ASSERT_TRUE(cursor.getInt(0L) == 3) << "Cursor column 1 must be a Integer : 3";
    ASSERT_TRUE(cursor.getString(1).equals("Teddy")) << "Cursor column 2 must be a String : Teddy";
    ASSERT_TRUE(cursor.getInt(2) == 23) << "Cursor column 3 must be a Integer : 23";
    ASSERT_TRUE(cursor.getString(3).equals("Norway")) << "Cursor column 4 must be a String : California";
    ASSERT_TRUE(cursor.isNull(4)) << "Cursor column 5 must be a Double : NuLL";

    cursor.next();
    ASSERT_TRUE(cursor.getInt(0L) == 4) << "Cursor column 1 must be a Integer : 4";
    ASSERT_TRUE(cursor.getString(1).equals("Mark")) << "Cursor column 2 must be a String : Mark";
    ASSERT_TRUE(cursor.getInt(2) == 25) << "Cursor column 3 must be a Integer : 25";
    ASSERT_TRUE(cursor.getString(3).equals("Rich-Mond ")) << "Cursor column 4 must be a String : Rich-Mond ";
    ASSERT_TRUE(cursor.getDouble(4) == 65000.00) << "Cursor column 5 must be a Double : 65000.00";

    cursor.next();
    ASSERT_TRUE(cursor.isAfterLast()) << "Cursor must after Last";

    cursor.close();
    db.closeDatabase();

}

TEST_F(Sqlite_Test, callBackTest) {
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
                  "VALUES (1, 'Paul', 32, 'California', 20000.12 ); "
                  "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
                  "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "
                  "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
                  "VALUES (3, 'Teddy', 23, 'Norway', NULL );"
                  "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
                  "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );");

    db.execSQL(CREATE);
    db.execSQL(INSERT);

    AtomicInteger rowCount(0);
    db.rawQuery("SELECT * FROM COMPANY",
                [&rowCount](std::vector<String16> columnName, std::vector<int> types, std::vector<SqliteValue> values,
                            int row) {
                    ASSERT_TRUE(columnName[0].equals("ID")) << "ColumnName must be ID";
                    ASSERT_TRUE(columnName[1].equals("NAME")) << "ColumnName must be NAME";
                    ASSERT_TRUE(columnName[2].equals("AGE")) << "ColumnName must be AGE";
                    ASSERT_TRUE(columnName[3].equals("ADDRESS")) << "ColumnName must be ADDRESS";
                    ASSERT_TRUE(columnName[4].equals("SALARY")) << "ColumnName must be SALARY";

                    ASSERT_TRUE(rowCount.getAndIncrement() == row);

                    switch (row) {
                        case 0:
                            ASSERT_TRUE(types[0] = SQLITE_INTEGER && values[0] == 1L)
                                                        << "Value 0 must Instance of Long and 1";
                            ASSERT_TRUE(types[1] = SQLITE_TEXT && values[1] == String16("Paul"))
                                                        << "Value 1 must Instance of String and Paul";
                            ASSERT_TRUE(types[2] = SQLITE_INTEGER && values[2] == 32L)
                                                        << "Value 2 must Instance of Long and 32";
                            ASSERT_TRUE(types[3] = SQLITE_TEXT && values[3] == String16("California"))
                                                        << "Value 3 must Instance of String and California";
                            ASSERT_TRUE(types[4] = SQLITE_FLOAT && values[4] == 20000.12)
                                                        << "Value 4 must Instance of Double and 20000.12";
                            break;
                        case 1:

                            ASSERT_TRUE(types[0] = SQLITE_INTEGER && values[0] == 2L)
                                                        << "Value 0 must Instance of Long and 2";
                            ASSERT_TRUE(types[1] = SQLITE_TEXT && values[1] == String16("Allen"))
                                                        << "Value 1 must Instance of String and Allen";
                            ASSERT_TRUE(types[2] = SQLITE_INTEGER && values[2] == 25L)
                                                        << "Value 2 must Instance of Long and 25";
                            ASSERT_TRUE(types[3] = SQLITE_TEXT && values[3] == String16("Texas"))
                                                        << "Value 3 must Instance of String and Texas";
                            ASSERT_TRUE(types[4] = SQLITE_FLOAT && values[4] == 15000.00)
                                                        << "Value 4 must Instance of Double and 15000.00";
                            break;
                        case 2:

                            ASSERT_TRUE(types[0] = SQLITE_INTEGER && values[0] == 3L)
                                                        << "Value 0 must Instance of Long and 3";
                            ASSERT_TRUE(types[1] = SQLITE_TEXT && values[1] == String16("Teddy"))
                                                        << "Value 1 must Instance of String and Teddy";
                            ASSERT_TRUE(types[2] = SQLITE_INTEGER && values[2] == 23L)
                                                        << "Value 2 must Instance of Long and 23";
                            ASSERT_TRUE(types[3] = SQLITE_TEXT && values[3] == String16("Norway"))
                                                        << "Value 3 must Instance of String and Norway";
                            ASSERT_TRUE(types[4] == SQLITE_NULL) << "Value 4 must be NULL";
                            break;
                        case 3:

                            ASSERT_TRUE(types[0] = SQLITE_INTEGER && values[0] == 4L)
                                                        << "Value 0 must Instance of Long and 4";
                            ASSERT_TRUE(types[1] = SQLITE_TEXT && values[1] == String16("Mark"))
                                                        << "Value 1 must Instance of String and Mark";
                            ASSERT_TRUE(types[2] = SQLITE_INTEGER && values[2] == 25L)
                                                        << "Value 2 must Instance of Long and 25";
                            ASSERT_TRUE(types[3] = SQLITE_TEXT && values[3] == String16("Rich-Mond "))
                                                        << "Value 3 must Instance of String and Rich-Mond ";
                            ASSERT_TRUE(types[4] = SQLITE_FLOAT && values[4] == 65000.00)
                                                        << "Value 4 must Instance of Double and 65000.00";
                            break;
                        default:
                            ASSERT_TRUE(false) << "Unknown result";
                    }
                });
    db.closeDatabase();

}

TEST_F(Sqlite_Test, writeReadUtf8) {
    FileHandle dbFileHandle = testFolder.child("createTest7.db3");
    Sqlite db(dbFileHandle);
    db.openOrCreateDatabase();

    String16 CREATE("CREATE TABLE Test (\n"
                  "    ID          INTEGER        NOT NULL\n"
                  "                               PRIMARY KEY AUTOINCREMENT,\n"
                  "    utf NVARCHAR (255)\n"
                  ");");

    db.execSQL(CREATE);

    String16 INSERT("INSERT INTO Test (ID, utf) "
                  "VALUES (1, '˷ꁾ'); "
                  "INSERT INTO Test (ID, utf) "
                  "VALUES (2, '߷®'); "
                  "INSERT INTO Test (ID, utf)"
                  "VALUES (3, NULL);"
                  "INSERT INTO Test (ID, utf)"
                  "VALUES (4, 'Mark' );");

    db.execSQL(INSERT);

    AtomicInteger rowCount(-1);
    db.rawQuery("SELECT * FROM Test",
                [&rowCount](std::vector<String16> columnName, std::vector<int> types, std::vector<SqliteValue> value,
                            int row) {
                    switch (rowCount.incrementAndGet()) {
                        case 0:
                            ASSERT_TRUE(columnName[0].equals("ID")) << "ColumnName must be ID";
                            ASSERT_TRUE(columnName[1].equals("utf")) << "ColumnName must be utf";

                            ASSERT_TRUE(types[0] == SQLITE_INTEGER && value[0] == 1L)
                                                        << "Value 0 must Instance of Long and 1";
                            ASSERT_TRUE(types[1] == SQLITE_TEXT && value[1] == String16("˷ꁾ"))
                                                        << "Value 1 must Instance of String and '˷ꁾ'";
                            break;
                        case 1:
                            ASSERT_TRUE(columnName[0].equals("ID")) << "ColumnName must be ID";
                            ASSERT_TRUE(columnName[1].equals("utf")) << "ColumnName must be utf";

                            ASSERT_TRUE(types[0] == SQLITE_INTEGER && value[0] == 2L)
                                                        << "Value 0 must Instance of Long and 2";
                            ASSERT_TRUE(types[1] == SQLITE_TEXT && value[1] == String16("߷®"))
                                                        << "Value 1 must Instance of String and Allen";
                            break;
                        case 2:
                            ASSERT_TRUE(columnName[0].equals("ID")) << "ColumnName must be ID";
                            ASSERT_TRUE(columnName[1].equals("utf")) << "ColumnName must be utf";

                            ASSERT_TRUE(types[0] == SQLITE_INTEGER && value[0] == 3L)
                                                        << "Value 0 must Instance of Long and 3";
                            ASSERT_TRUE(value[1].isNull()) << "Value 1 must be NULL";
                            break;
                        case 3:
                            ASSERT_TRUE(columnName[0].equals("ID")) << "ColumnName must be ID";
                            ASSERT_TRUE(columnName[1].equals("utf")) << "ColumnName must be utf";

                            ASSERT_TRUE(types[0] == SQLITE_INTEGER && value[0] == 4L)
                                                        << "Value 0 must Instance of Long and 4";
                            ASSERT_TRUE(types[1] == SQLITE_TEXT && value[1] == String16("Mark"))
                                                        << "Value 1 must Instance of String and Mark";
                            break;
                        default:
                            ASSERT_TRUE(false) << "Unknown result";
                    }

                });
    db.closeDatabase();

}