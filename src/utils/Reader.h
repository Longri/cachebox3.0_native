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
 * Created by Longri on 25.07.18.
 */

#ifndef CACHEBOX_READER_H
#define CACHEBOX_READER_H


class Reader {
public:

    /**
    * Reads characters into a portion of an array.  This method will block
    * until some input is available, an I/O error occurs, or the end of the
    * stream is reached.
    *
    * @param      cbuf  Destination buffer
    * @param      off   Offset at which to start storing characters
    * @param      len   Maximum number of characters to read
    *
    * @return     The number of characters read, or -1 if the end of the
    *             stream has been reached
    */
    virtual long read(char16_t *cbuf, long off, long len)=0;
};


#endif //CACHEBOX_READER_H
