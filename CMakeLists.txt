cmake_minimum_required(VERSION 3.10)
project(Cachebox__)

set(C_LOG)
set(CMAKE_CXX_STANDARD 17)

set(Boost_USE_STATIC_LIBS ON) # only find static libs
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_LIBRARYDIR lib/boost_1_68_0_Date_extract)
set(BOOST_INCLUDEDIR lib/boost_1_68_0_Date_extract)
include_directories(${Boost_INCLUDE_DIR})

add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)


message(STATUS "start running cmake...")

find_package(Boost COMPONENTS)


if (Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif ()


set(SOURCE_FILES
        main.cpp
        src/Cache.h Tests/Cache_Test.cpp
        src/Coordinate.h Tests/Coordinate_Test.cpp
        src/Type.h Tests/Type_Test.cpp
        src/Waypoint.h Tests/Waypoint_Test.cpp
        src/utils/Array.h Tests/utils/Array_Test.cpp
        src/utils/PointerMap.h Tests/utils/PointerMap_Test.cpp
        src/logger/Logger.h
        src/logger/EmptyLogger.h
        src/logger/JNI_Logger.h
        src/logger/C__Logger.h
        src/logger/LoggerFactory.h Tests/logger/Logger_Test.cpp
        src/utils/AbstractStreamParser.h Tests/utils/JsonStreamParser_Test.cpp
        src/utils/Object.h Tests/MathUtils_Test.cpp
        Tests/utils/Object_Test.cpp
        src/utils/StringObject.h
        src/utils/exceptions/NumberFormatException.h
        src/utils/exceptions/StringException.h Tests/utils/exception/StringException_Test.cpp
        src/utils/exceptions/ArithmeticException.h
        src/utils/FileHandle.h Tests/utils/FileHandle_Test.cpp
        src/utils/Reader.h src/utils/FileReader.h Tests/utils/FileReader_Test.cpp
        src/utils/System.h Tests/utils/System_Test.cpp
        src/utils/JsonStreamParser.h
        src/utils/exceptions/IllegalStateException.h
        src/utils/exceptions/IndexOutOfBoundsException.h
        src/utils/AtomicBoolean.h
        src/utils/AtomicInteger.h Tests/utils/Atomic_Test.cpp
        Tests/utils/StopWatch.h
        src/sqlite_src/sqlite3.c src/sqlite_src/sqlite3.h src/sqlite_src/sqlite3ext.h
        src/utils/sqlite/Sqlite.h Tests/utils/sqlite/Sqlite_Test.cpp
        src/utils/sqlite/SqliteCursor.h
        src/utils/sqlite/SqlitePreparedStatement.h
        src/utils/sqlite/SqliteResult.h
        src/utils/sqlite/SqliteException.h
        src/utils/sqlite/SqliteValue.h Tests/utils/sqlite/SqliteValue_Test.cpp
        Tests/EXPERIMENT.cpp
        src/utils/String16.h Tests/String16_Test.cpp)
add_executable(Cachebox__ ${SOURCE_FILES})
target_link_libraries(Cachebox__ gtest gtest_main)

