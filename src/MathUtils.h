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
#ifndef M_PI
#define M_PI        3.14159265358979323846264338327950288   /* pi             */
#define M_PI_2      1.57079632679489661923132169163975144   /* pi/2           */
#define M_PI_4      0.785398163397448309615660845819875721  /* pi/4           */
#define M_1_PI      0.318309886183790671537767526745028724  /* 1/pi           */
#endif

#ifndef CACHEBOX_MATHUTILS_H
#define CACHEBOX_MATHUTILS_H

class MathUtils {


public:

    /**
     * WGS84 major axis = 6378137.0
     */
    static constexpr double WGS84_MAJOR_AXIS = 6378137.0; // WGS84 major axis

    /**
     * WGS84 semi-major axis = 6356752.3142
     */
    static constexpr double WGS84_SEMI_MAJOR_AXIS = 6356752.3142; // WGS84 semi-major axis


    /**
    * precalculation of Math.PI / 180.0
    */
    static constexpr double DEG_RAD = M_PI / 180.0;

    /**
     * precalculation of 180.0 / Math.PI
     */
    static constexpr double RAD_DEG = 180.0 / M_PI;


    /**
     * Returns the greater of two {@code long} values.
     *
     * @param   a   an argument.
     * @param   b   another argument.
     * @return  the larger of {@code a} and {@code b}.
     */
    static long max(long a, long b) {
        return (a >= b) ? a : b;
    }

    static long min(long a, long b) {
        return (a <= b) ? a : b;
    }

    static const char *getHexString(long long int value) {
        static const char *digits = "0123456789ABCDEF";
        static const size_t hex_len = sizeof(long long int) << 1;
        char *temp = new char[hex_len];
        int digitCount = 0;
        for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4) {
            int digit = (value >> j) & 0x0f;
            temp[i] = digits[digit];
            if (digit > 0 & digitCount == 0) {
                digitCount = hex_len - i;
            }
        }

        char *chars = new char[digitCount + 3];
        std::fill(chars,chars+digitCount+3,'\0');
        for (int i = digitCount; i > 0; i--) {
            chars[digitCount - (i - 2)] = temp[hex_len - i];
        }

        chars[0] = '0';
        chars[1] = 'x';

        //destroy tmp char[]
        delete[](temp);

        return chars;
    }

};


#endif //CACHEBOX_MATHUTILS_H
