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

#ifndef CACHEBOX_SQLITEEXCEPTION_H
#define CACHEBOX_SQLITEEXCEPTION_H


#include "../exceptions/StringException.h"

class SqliteException : public StringException {
public:
    explicit SqliteException(const char *string, const char *file,
                                   const char *function, long line)
            : StringException(string, "SqliteException", file, function, line) {}
};


#endif //CACHEBOX_SQLITEEXCEPTION_H
