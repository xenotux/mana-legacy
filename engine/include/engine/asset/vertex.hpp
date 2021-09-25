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

#ifndef MANA_VERTEX_HPP
#define MANA_VERTEX_HPP

namespace engine {
    struct Vertex {
        float data[14];

        Vertex(Vec3f position, Vec3f normal, Vec2f uv, Vec3f tangent, Vec3f bitangent) : data() {
            data[0] = position.x;
            data[1] = position.y;
            data[2] = position.z;
            data[3] = normal.x;
            data[4] = normal.y;
            data[5] = normal.z;
            data[6] = uv.x;
            data[7] = uv.y;
            data[8] = tangent.x;
            data[9] = tangent.y;
            data[10] = tangent.z;
            data[11] = bitangent.x;
            data[12] = bitangent.y;
            data[13] = bitangent.z;
        }

        Vertex(Vec3f position, Vec2f uv) : Vertex(position, {}, uv, {}, {}) {}

        explicit Vertex(Vec3f position) : Vertex(position, {}, {}, {}, {}) {}

        Vertex() = default;
    };
}

#endif //MANA_VERTEX_HPP
