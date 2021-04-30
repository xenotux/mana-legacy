/**
 *  Mana - 3D Game Engine
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MANA_IMAGEWRITER_HPP
#define MANA_IMAGEWRITER_HPP

#include <string>

#include "engine/common/imagebuffer.hpp"

namespace mana {
    class ImageWriter {
    public:
        static void write(std::string filepath, const ImageBuffer<ColorRGB> &buffer);

        static void write(std::string filepath, const ImageBuffer<ColorRGBA> &buffer);
    };
}

#endif //MANA_IMAGEWRITER_HPP
