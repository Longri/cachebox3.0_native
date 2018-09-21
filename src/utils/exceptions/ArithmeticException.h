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

#ifndef CACHEBOX_ARITHMETICEXCEPTION_H
#define CACHEBOX_ARITHMETICEXCEPTION_H

#include <exception>
#include "StringException.h"

class ArithmeticException : public StringException {

protected:

public:
    explicit ArithmeticException(const char *string, const char *file,
                                 const char *function, long line)
            : StringException(string, "ArithmeticException", file, function, line) {}
};


#endif //CACHEBOX_ARITHMETICEXCEPTION_H
