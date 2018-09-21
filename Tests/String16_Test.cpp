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

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/utils/String16.h"
#include "utils/StopWatch.h"
#include <cstring>

using testing::Eq;

namespace {
    class String16_Test : public testing::Test {
    public:

    };
}


TEST_F(String16_Test, testConstructor) {
    String16 str(u"TestString");
    ASSERT_EQ(10, str.length());
    str.append(u'A');
    ASSERT_EQ(11, str.length());
    ASSERT_EQ(String16(u"TestStringA"), str);

    String16 subStr(str, 4, 3);
    ASSERT_EQ(String16(u"Str"), subStr);

    String16 str2 = String16("Hallo World");
    ASSERT_EQ(String16(u"Hallo World"), str2);

}

TEST_F(String16_Test, testAppend) {
    String16 str = String16();
    ASSERT_EQ(0, str.length());
    str.append('C');
    str.append('a');
    str.append('c');
    str.append('h');
    str.append('e');
    str.append('b');
    str.append('o');
    str.append('x');
    ASSERT_EQ(8, str.length());
    str.append(u' ');
    str.append(u"to many secrets");

    String16 str2 = String16(" secrets! But not with me and the right code");
    str.append(&str2, 8, 25);

    ASSERT_EQ(41, str.length());
    ASSERT_EQ(String16("Cachebox to many secrets! But not with me"), str);
}

TEST_F(String16_Test, testIndexOf) {
    String16 str = String16(u"Hello World");
    String16 search = String16(u"World");
    String16 search2 = String16(u"llo");
    long idx = str.indexOf(search);
    ASSERT_EQ(6, idx);
    idx = str.indexOf(search2);
    ASSERT_EQ(2, idx);
    idx = str.indexOf(u"ld");
    ASSERT_EQ(9, idx);
    idx = str.indexOf('l');
    ASSERT_EQ(2, idx);
    idx = str.indexOf('l', 4);
    ASSERT_EQ(9, idx);

    String16 str3 = String16(u"  Hallo  Cachebox  to     many secrets  ");
    idx = str3.indexOf(' ', 6);
    ASSERT_EQ(7, idx);
}

TEST_F(String16_Test, testSingleCharReplace) {
    String16 str = String16(u"Hello, World,");
    String16 point = String16(u".");
    String16 comma = String16(u",");
    str.replaceAll(comma, point);
    ASSERT_EQ(String16(u"Hello. World."), str);
    str.replaceAll(point, comma);
    str.replace(comma, point);
    ASSERT_EQ(String16(u"Hello. World,"), str);
    str.replace(point, comma);
    str.replace(comma, point, 6);
    ASSERT_EQ(String16(u"Hello, World."), str);
    str.replace(',', '.');
    ASSERT_EQ(String16(u"Hello. World."), str);
    str.replace(u".", u",");
    ASSERT_EQ(String16(u"Hello, World."), str);
    str.replace(u",", u".");
    ASSERT_EQ(String16(u"Hello. World."), str);
    str.replaceAll(u".", u",");
    ASSERT_EQ(String16(u"Hello, World,"), str);
    str.replace(u",", u".", 6);
    ASSERT_EQ(String16(u"Hello, World."), str);
    str.replaceAll(u".", u",");
    ASSERT_EQ(String16(u"Hello, World,"), str);
    str.replace(',', '.', 6);
    ASSERT_EQ(String16(u"Hello, World."), str);
}

TEST_F(String16_Test, testMultiCharReplace) {
    String16 str = String16(u"Hello World ");
    String16 cache = String16(u"Cache");
    String16 world = String16(u"World");
    str.replaceAll(world, cache);
    ASSERT_EQ(String16(u"Hello Cache "), str);
}

TEST_F(String16_Test, testMultiCharReplaceMore) {
    String16 str = String16(u"Hello World");
    String16 search = String16(u"Hello");
    String16 replace = String16(u"Hello my");
    str.replaceAll(search, replace);
    ASSERT_EQ(String16(u"Hello my World"), str);
}

TEST_F(String16_Test, testMultiCharReplaceLess) {
    String16 str = String16(u"Hello World");
    String16 search = String16(u"Hello");
    String16 replace = String16(u"my");
    str.replaceAll(search, replace);
    ASSERT_EQ(String16(u"my World"), str);
}

TEST_F(String16_Test, testMultiCharRemove) {
    String16 str = String16(u"Hello World");
    String16 search = String16(u"Hello ");
    String16 replace = String16(u"");
    str.replaceAll(search, replace);
    ASSERT_EQ(String16(u"World"), str);
}

TEST_F(String16_Test, testSingleCharRemove) {
    String16 str = String16(u"Hello World");
    str.replace(u"o", u"");
    ASSERT_EQ(String16(u"Hell World"), str);
    String16 str2 = String16(u"Hello World");
    str2.replaceAll(u"o", u"");
    ASSERT_EQ(String16(u"Hell Wrld"), str2);
}

TEST_F(String16_Test, testTrim) {
    String16 str = String16(u" Hello World ");
    str.trim();
    ASSERT_EQ(String16(u"Hello World"), str);
    String16 str2 = String16(u"       Hello World      ");
    str2.trim();
    ASSERT_EQ(String16(u"Hello World"), str2);
}

TEST_F(String16_Test, testToUpperCase) {
    String16 str = String16(u" Hello World ");
    str.toUpperCase();
    str.trim();
    ASSERT_EQ(String16(u"HELLO WORLD"), str);
}

TEST_F(String16_Test, testToLowerCase) {
    String16 str = String16(u" Hello World ");
    str.toLowerCase();
    str.trim();
    ASSERT_EQ(String16(u"hello world"), str);
}

TEST_F(String16_Test, testSplitSingleChar) {
    String16 str = String16(u"Hello World");
    String16 noChangedStr = String16(str);
    std::vector<String16> split = str.split(' ');
    ASSERT_EQ(str, noChangedStr);
    ASSERT_EQ(2, split.size());
    ASSERT_EQ(String16(u"Hello"), split[0]);
    ASSERT_EQ(String16(u"World"), split[1]);

    String16 str2 = String16(u"  Hallo  World  ");
    String16 noChangedStr2 = String16(str2);
    std::vector<String16> split2 = str.split(' ');
    ASSERT_EQ(str2, noChangedStr2);
    ASSERT_EQ(2, split2.size());
    ASSERT_EQ(String16(u"Hello"), split2[0]);
    ASSERT_EQ(String16(u"World"), split2[1]);

    String16 str3 = String16(u"  Hello  Cachebox  to     many secrets  ");
    String16 noChangedStr3 = String16(str3);
    std::vector<String16> split3 = str3.split(u" ");
    ASSERT_EQ(str3, noChangedStr3);
    ASSERT_EQ(5, split3.size());
    ASSERT_EQ(String16(u"Hello"), split3[0]);
    ASSERT_EQ(String16(u"Cachebox"), split3[1]);
    ASSERT_EQ(String16(u"to"), split3[2]);
    ASSERT_EQ(String16(u"many"), split3[3]);
    ASSERT_EQ(String16(u"secrets"), split3[4]);

    String16 str4 = String16(u" 009 23.470");
    String16 noChangedStr4 = String16(str4);
    std::vector<String16> split4 = str4.split(u" ");
    ASSERT_EQ(str4, noChangedStr4);
    ASSERT_EQ(2, split4.size());
    ASSERT_EQ(String16(u"009"), split4[0]);
    ASSERT_EQ(String16(u"23.470"), split4[1]);
}

TEST_F(String16_Test, testSplitMultiChar) {
    String16 str = String16(u"#/!Hello#/!Cachebox#/!#/!to#/!many#/!secrets#/!");
    std::vector<String16> split = str.split(u"#/!");
    ASSERT_EQ(5, split.size());
    ASSERT_EQ(String16(u"Hello"), split[0]);
    ASSERT_EQ(String16(u"Cachebox"), split[1]);
    ASSERT_EQ(String16(u"to"), split[2]);
    ASSERT_EQ(String16(u"many"), split[3]);
    ASSERT_EQ(String16(u"secrets"), split[4]);
}

TEST_F(String16_Test, testParseBool) {
    ASSERT_TRUE(String16(u"true").parseBool());
    ASSERT_TRUE(String16(u"True").parseBool());
    ASSERT_TRUE(String16(u"TRUE").parseBool());
    ASSERT_FALSE(String16(u"false").parseBool());
    ASSERT_FALSE(String16(u"False").parseBool());
    ASSERT_FALSE(String16(u"FALSE").parseBool());
}

TEST_F(String16_Test, testParseLong) {
    String16 str(u"3810940");
    long i = str.parseLong();
    ASSERT_EQ(i, 3810940);
}

TEST_F(String16_Test, testParseLongLong) {
    String16 str(u"219011721901171232");
    long long l = str.parseLongLong();
    ASSERT_EQ(l, 219011721901171232);
}

TEST_F(String16_Test, testParseDouble) {
    String16 str(u"49.349817");
    double d = str.parseDouble();
    ASSERT_EQ(d, 49.349817);
}

TEST_F(String16_Test, testSubString) {

    String16 str = String16(u"®Hällo World˷");
    String16 str2 = str.subString(7, 12);
    std::cout << str << "\n";
    ASSERT_EQ(str2, String16(u"World"));

    String16 str3 = str.subString(7);
    ASSERT_EQ(str3, String16(u"World˷"));

}

TEST_F(String16_Test, testToUtf8Chars) {
//    ASSERT_TRUE(false)<<"must reimplement for String16";
    String16 str = String16();
    char32_t c = 128512;
    char16_t c16h = 0xD83D;
    char16_t c16l = 0xDE00;
    str.append(c16h);
    str.append(c16l);
    str.append(u"߷®Hällo World˷ꁾ");

    auto *u8 = const_cast<char *>("߷®Hällo World˷ꁾ");
    char u8Chars[] = {(char) 0xF0, (char) 0x9F, (char) 0x98, (char) 0x80, u8[0], u8[1], u8[2], u8[3], u8[4], u8[5],
                      u8[6], u8[7], u8[8], u8[9], u8[10], u8[11], u8[12], u8[13],
                      u8[14], u8[15], u8[16], u8[17], u8[18], u8[19], u8[20],
                      u8[21]};


    char *chars = String16::toUtf8_Chars(str);
    long length = std::strlen(u8Chars);
    for (long i = 0; i < length; i++)
        ASSERT_EQ(u8Chars[i], chars[i]) << "Char at idx " << i << " is wrong!";
}

TEST_F(String16_Test, testUtf8ToChar32Vector) {
    String16 str = String16();
    char32_t c = 128512;
    char16_t c16h = 0xD83D;
    char16_t c16l = 0xDE00;
    str.append(c16h);
    str.append(c16l);
    str.append(u"߷®Hällo World˷ꁾ");
    auto *u8 = const_cast<char *>("߷®Hällo World˷ꁾ");
    char u8Chars[] = {(char) 0xF0, (char) 0x9F, (char) 0x98, (char) 0x80, u8[0], u8[1], u8[2], u8[3], u8[4], u8[5],
                      u8[6], u8[7], u8[8], u8[9], u8[10], u8[11], u8[12], u8[13],
                      u8[14], u8[15], u8[16], u8[17], u8[18], u8[19], u8[20],
                      u8[21]};

    std::cout << str << "\n";
    std::vector<char16_t> vec = String16::utf8ToChar16Vector(u8Chars);
    ASSERT_EQ(vec.size(), 17);
    for (long i = 0; i < vec.size(); i++) {
        ASSERT_EQ(str.charAt(i), vec[i]) << "Char at idx " << i << " is wrong!";
    }

}

TEST_F(String16_Test, testAllUtf8Convert) {
    std::vector<char16_t> vec;
    unsigned long testCount = 0xFFFF;

    for (unsigned long i = 32; i < 0xD800; i++) {
        vec.push_back(i);
    }

    for (unsigned long i = 0xDBFF; i < 0xFFFF; i++) {
        vec.push_back(i);
    }

    char *chars = String16::toUtf8_Chars(vec.data(), vec.size());
    std::vector<char16_t> vecResult = String16::utf8ToChar16Vector(chars);

    for (unsigned long i = 0; i < vec.size(); i++) {
        ASSERT_EQ(vec[i], vecResult[i])<< "Char at idx " << i << " is wrong!";
    }
}

TEST_F(String16_Test, testIntConstructor) {

    String16 str1 = String16(1234);
    String16 exspectedStr1 = String16("1234");
    ASSERT_EQ(str1, exspectedStr1);

    String16 str2 = String16(123456789);
    String16 exspectedStr2 = String16("123456789");
    ASSERT_EQ(str2, exspectedStr2);

    String16 str3 = String16(123456789012345);
    String16 exspectedStr3 = String16("123456789012345");
    ASSERT_EQ(str3, exspectedStr3);


    String16 str4 = String16(1234L);
    String16 exspectedStr4 = String16("1234");
    ASSERT_EQ(str4, exspectedStr4);

    String16 str5 = String16(123456789L);
    String16 exspectedStr5 = String16("123456789");
    ASSERT_EQ(str5, exspectedStr5);

    String16 str6 = String16(123456789012345L);
    String16 exspectedStr6 = String16("123456789012345");
    ASSERT_EQ(str6, exspectedStr6);

}

TEST_F(String16_Test, testDateFormatter) {
    String16 FORMAT = String16("yyyy-MM-dd hh:mm:ss.SSSSS");
    std::string timeString("2018-07-21 12:17:23.45678");
    String16 timeString2("2018-07-21 12:17:23.45678");

    boost::posix_time::ptime testTime = String16::getPtime(&timeString2, FORMAT);
    const boost::posix_time::time_duration &timeDuration = testTime.time_of_day();
    const boost::gregorian::date &date = testTime.date();

    ASSERT_EQ(date.year(), 2018);
    ASSERT_EQ(date.month(), 7);
    ASSERT_EQ(date.day(), 21);

    ASSERT_EQ(timeDuration.hours(), 12);
    ASSERT_EQ(timeDuration.minutes(), 17);
    ASSERT_EQ(timeDuration.seconds(), 23);
    ASSERT_EQ(timeDuration.fractional_seconds(), 45678);


    String16 FORMAT2 = String16("dd-MM-yyyy hh:mm:ss.SSS");
    String16 EXPECTED2 = String16("21-07-2018 12:17:23.456");
    String16 FORMATED_DATE_2 = String16::getFormattedDate(testTime, FORMAT2);
    ASSERT_EQ(EXPECTED2, FORMATED_DATE_2);

    String16 FORMAT3 = String16("dd-MM-yyyy hh:mm:ss.SSSSSSS");
    String16 EXPECTED3 = String16("21-07-2018 12:17:23.4567800");
    String16 FORMATED_DATE_3 = String16::getFormattedDate(testTime, FORMAT3);
    ASSERT_EQ(EXPECTED3, FORMATED_DATE_3);
}

TEST_F(String16_Test, testLastIndexOf) {
    String16 testString("Hallo World, HalloWorld");
    long idx = testString.lastIndexOf(u'W');
    ASSERT_EQ(18, idx);
}

TEST_F(String16_Test, testLastIndexOf2) {
    String16 testString("Hallo World, HalloWorld");
    long idx = testString.lastIndexOf(u'W', 16);
    ASSERT_EQ(6, idx);
}

TEST_F(String16_Test, testEqualsoperator) {
    String16 testString16("Hallo World, HalloWorld");
    String16 testString2("Hallo World, HalloWorld");
    ASSERT_TRUE(testString16 == "Hallo World, HalloWorld");
    ASSERT_TRUE(testString16 == testString2);
}

TEST_F(String16_Test, testReadUtf8File) {


}