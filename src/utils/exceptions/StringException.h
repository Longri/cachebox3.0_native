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
 * Created by Longri on 19.07.18.
 */

#ifndef _GLIBCXX_USE_NOEXCEPT
#define _GLIBCXX_USE_NOEXCEPT noexcept
#endif

#ifndef CACHEBOX_STRINGEXCEPTION_H
#define CACHEBOX_STRINGEXCEPTION_H


class StringException : public std::exception {

protected :
    const char *message;
    const char *className;
    const char *file;
    const char *function;
    const long line;

public:
    explicit StringException(const char *message, const char *file,
                             const char *function, long line) : message(message), className(__func__),
                                                                file(file), function(function),
                                                                line(line) {}

    explicit StringException(const char *message, const char *clNa, const char *file,
                             const char *function, long line) : message(message), className(clNa),
                                                                file(file), function(function),
                                                                line(line) {}


    virtual const char *what() const _GLIBCXX_USE_NOEXCEPT override {
        return message;
    }

    virtual const char *getClassName() const {
        return className;
    }

    virtual const char *getFileName() const {
        return file;
    }

    virtual const char *getFunctionName() const {
        return function;
    }

    virtual long getLineNumber() const {
        return line;
    }

    static bool instanceOf(exception *ex) {
        if (StringException *v = dynamic_cast<StringException *>(ex)) {
            return true;
        }
        return false;
    }

};


#endif //CACHEBOX_STRINGEXCEPTION_H


