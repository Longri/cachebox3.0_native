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
 * Created by Longri on 04.08.18.
 */

#ifndef CACHEBOX_JSONSTREAMPARSER_H
#define CACHEBOX_JSONSTREAMPARSER_H


#include "AbstractStreamParser.h"
#include "String16.h"
#include "AtomicInteger.h"
#include "Array.h"

//static const Logger *log = LoggerFactory::getLogger(String16("JsonStreamParser"));
static const String16 NULL_STR = String16("null");
static const String16 AN = String16("\"");

class JsonStreamParser : public AbstractStreamParser {

private:


    Array<String16 *> arrayNameStack;
    long lastPeek;
    long lastNameStart = -1;
    Array<String16> exclude;
    AtomicBoolean isExclude = AtomicBoolean(false);
    AtomicInteger isExcludeCount = AtomicInteger(0);
    AtomicBoolean noHandleForNextValue = AtomicBoolean(false);


public:
    /**
    * Parse the data and return offset of last processed item
    *
    * @param data
    */
    long parseData(const char16_t *data, long dataLength) override {
        if (data[dataLength - 1] == '\\') {
            return 0;
        }

        String16 *actName = nullptr;
        lastNameStart = -1;
        lastPeek = -1;
        long offset = 0;
        bool incommingExclude = isExclude.get();
        noHandleForNextValue.set(false);
        while (!CANCELD.get() && offset < dataLength) {
            long peek = searchPeek(data, dataLength, offset);
            if (peek == -1) return offset;

            if (isExclude.get()) {
                offset = peek + 1;
                lastPeek = peek;

                if (data[peek] == '[') {
                    isExcludeCount.incrementAndGet();
                } else if (data[peek] == ']') {
                    if (isExcludeCount.decrementAndGet() < 0) {
                        isExclude.set(false);
                        return peek;

                    }
                }

            } else {
                long nameStart = searchNameBefore(data, peek);
                if (nameStart == -1)
                    actName = nullptr;
                else {
                    actName = getName(data, dataLength, nameStart);
                }

                bool noValue = false;
                switch (data[peek]) {
                    case '{':
                    case '[':
                        noValue = true;
                }
                if (!noValue) {
                    if (!(actName == nullptr && lastPeek == -1)) {
                        String16 *valueString = getValue(data,
                                                       nameStart +
                                                       (actName != nullptr ? actName->length() : lastPeek + 2),
                                                       peek);
                        if (valueString != nullptr) {
                            if (!noHandleForNextValue.get() && !isExclude.get()) {
                                try {
                                    handleValue(actName, valueString);
                                } catch (...) {
//                                    Object *nameObj=new StringObject(*actName);
//                                    Object *valueObj=new StringObject(*valueString);
//                                    log->error("Error with parse value near {} value;'{}'", nameObj, valueObj);
                                }
                            } else {
                                noHandleForNextValue.set(false);
                            }
                            offset = peek + 1;
                        }
                    }
                }

                if (peek >= 0) {
                    switch (data[peek]) {
                        case '{':
                            startObject(actName);
                            break;
                        case '[':
                            arrayNameStack.add(actName);
                            if (ifExclude()) {
                                isExclude.set(true);
                                isExcludeCount.set(0);
                                break;
                            }
                            startArray(actName);

                            break;
                        case '}':
                            pop();
                            break;
                        case ']':
                            if (ifExclude()) {
                                arrayNameStack.pop();
                            } else {
                                pop();
                                endArray(arrayNameStack.pop());
                            }
                            break;
                        case ',':
                            break;
                    }
                    offset = peek + 1;
                    lastPeek = peek;
                } else {
                    break;
                }
            }
        }
        return offset;
    }

    long searchPeek(const char16_t *data, long dataLength, long offset) {

        bool isString = false;

        for (long i = offset, n = dataLength; i < n; i++) {
            if (isString) {
                switch (data[i]) {
                    case '"':
                        // check if escaped
                        if (i >= 1 && data[i - 1] == '\\')
                            continue;
                        isString = false;
                        break;
                }
                continue;
            }

            switch (data[i]) {
                case '"':
                    // check if escaped
                    if (i >= 1 && data[i - 1] == '\\')
                        continue;
                    isString = true;
                    break;
                case '{':
                case '}':
                case '[':
                case ']':
                case ',':
                    return i;
            }
        }
        return -1;
    }

    long searchNameBefore(const char16_t *data, long peek) {
        long found = -1;
        bool first = true;
        bool end = true;
        bool isString = false;
        for (int i = peek; i >= 0; i--) {
            if (isString) {
                switch (data[i]) {
                    case '"':
                        // check if escaped
                        if (i >= 1 && data[i - 1] == '\\')
                            continue;
                        isString = false;
                        break;
                }
                continue;
            }

            switch (data[i]) {
                case ':':
                    end = false;
                    break;
                case '"':
                    if (end) {
                        // check if escaped
                        if (i >= 1 && data[i - 1] == '\\')
                            continue;
                        isString = true;
                        break;
                    }
                    if (first) {
                        first = false;
                    } else {
                        found = i;
                        i = 0; //break for
                    }
                    break;
            }
        }
        if (lastNameStart == found) return -1;
        lastNameStart = found;
        return found;
    }

    String16 *getName(const char16_t *data, long dataLength, long nameStart) {
        long found = -1;
        for (long i = nameStart + 1, n = dataLength; i < n; i++) {
            switch (data[i]) {
                case U'"':
                    found = i;
                    i = dataLength; //break for
                    break;
            }
        }
        if (found > nameStart) {
            String16 *name = new String16(data, nameStart + 1L, found - nameStart - 1L);
            if (DEBUG) log->debug("Found name: {}", name);
            return name;
        }
        return nullptr;
    }

    String16 *getValue(const char16_t *data, long start, long end) {
        long found = -1;
        if (start < 0) return nullptr;
        for (long i = start; i < end; i++) {
            switch (data[i]) {
                case ':':
                    found = i + 1;
                    i = end;//break for
                    break;
            }
        }
        if (found > start && found != end) {
            String16 *value = nullptr;
            //try {
            value = new String16(data, found, end - found);
//            } catch (Exception e) {
//                log->error("found: {} end: {}", found, end, e);
//            }
            if (DEBUG) log->debug("Found Value: {}", value);
            return value;
        }

        //try to trimmed value
        if (end - start > 0) {
            String16 *value = new String16(data, start, end - start);

            if (value->startsWith(AN) && value->endsWith(AN)) return value;
        }
        return nullptr;
    }

    void handleValue(String16 *actName, String16 *valueString) {
        valueString->trim();
        unescape(valueString);

        if (NULL_STR.equals(valueString)) {
            string(actName, NULL);
        } else if (valueString->startsWith(AN)) {
            String16 *unes = new String16(valueString->subString(1, valueString->length() - 1));
            unescape(unes);
            string(actName, unes);
        } else if (valueString->contains(u".")) {
            // parse double
            double dblValue = valueString->parseDouble();
            number(actName, dblValue, valueString);
        } else if (valueString->toLowerCase()->equals(u"true") ||
                   valueString->toLowerCase()->equals(u"false")) {
            boolean(actName, valueString->toLowerCase()->equals(u"true"));
        } else {
            //parse long
            long value = valueString->parseLong();
            number(actName, value, valueString);
        }
    }

    bool ifExclude() {
        if (exclude.isEmpty()) return false;
        String16 stack = arrayNameStack.toString16();
        for (long i = 0, n = exclude.size(); i < n; i++) {
            String16 exc = exclude.get(i);
            if (stack.equals(&exc)) {
                return true;
            }
        }
        return false;
    }

    /**
     * Unescape function from com.badlogic.gdx.utils.JsonReader!
     *
     * @param value
     * @return
     * @author Nathan Sweet
     */
    void unescape(String16 *value) {
        long length = value->length();
        String16 buffer;
        for (int i = 0; i < length;) {
            char32_t c = value->charAt(i++);
            if (c != '\\') {
                buffer.append(c);
                continue;
            }
            if (i == length) break;
            c = value->charAt(i++);
            if (c == 'u') {
                buffer.append(static_cast<char32_t>(String16::parseLong(value, i, i + 4)));
                i += 4;
                continue;
            }
            switch (c) {
                case '"':
                case '\\':
                case '/':
                    break;
                case 'b':
                    c = '\b';
                    break;
                case 'f':
                    c = '\f';
                    break;
                case 'n':
                    c = '\n';
                    break;
                case 'r':
                    c = '\r';
                    break;
                case 't':
                    c = '\t';
                    break;
                default:
                    buffer.append('\\');
            }
            buffer.append(c);
        }
        value->set(buffer);
    }

    void cancel() {
        CANCELD.set(true);
    }

    virtual void startArray(String16 *name) {
    }

    virtual void endArray(String16 *name) {
    }

    virtual void startObject(String16 *name) {
    }

    virtual void pop() {
    }

    virtual void string(String16 *name, String16 *value) {
    }

    virtual void number(String16 *name, double value, String16 *stringValue) {
    }

    virtual void number(String16 *name, long value, String16 *stringValue) {
    }

    virtual void boolean(String16 *name, bool value) {
    }

};


#endif //CACHEBOX_JSONSTREAMPARSER_H
