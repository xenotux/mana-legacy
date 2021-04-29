/**
 *  Mana - 3D Engine
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

#ifndef MANA_OGLRENDERALLOCATOR_HPP
#define MANA_OGLRENDERALLOCATOR_HPP

#include "engine/render/renderallocator.hpp"

namespace mana {
    namespace opengl {
        class OGLRenderAllocator : public RenderAllocator {
        public:
            RenderTarget *allocateRenderTarget(int width, int height) override;

            RenderTexture *allocateTexture(RenderTexture::Attributes attributes) override;

            RenderMesh *allocateMesh(const Mesh &mesh) override;

            RenderMesh *allocateInstancedMesh(const Mesh &mesh,
                                              const std::vector<Transform> &offsets) override;

            ShaderProgram *allocateShaderProgram(std::string vertexShader, std::string fragmentShader) override;
        };
    }
}

#endif //MANA_OGLRENDERALLOCATOR_HPP
