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

#ifndef MANA_RENDERMATERIAL_HPP
#define MANA_RENDERMATERIAL_HPP

#include <memory>

#include "engine/render/texturebuffer.hpp"
#include "engine/render/shaderprogram.hpp"

namespace engine {
    struct RenderMaterial {
        ColorRGBA diffuse{};
        ColorRGBA ambient{};
        ColorRGBA specular{};
        ColorRGBA emissive{};

        float shininess{32};

        //Optional textures if a texture buffer is nullptr the corresponding color is used instead.
        TextureBuffer *diffuseTexture = nullptr;
        TextureBuffer *ambientTexture = nullptr;
        TextureBuffer *specularTexture = nullptr;
        TextureBuffer *emissiveTexture = nullptr;

        TextureBuffer *shininessTexture = nullptr;

        //Optional normal texture if it is nullptr the vertex normals are used instead.
        TextureBuffer *normalTexture = nullptr;
    };
}

#endif //MANA_RENDERMATERIAL_HPP