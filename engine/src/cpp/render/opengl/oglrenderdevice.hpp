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

#ifndef MANA_OGLRENDERDEVICE_HPP
#define MANA_OGLRENDERDEVICE_HPP

#include <mana.hpp>

#include "engine/render/renderdevice.hpp"

namespace mana {
    namespace opengl {
        class OGLRenderDevice : public RenderDevice {
        public:
            ~OGLRenderDevice() override = default;

            Renderer *createRenderer() override;

            RenderTarget *createRenderTarget(Vec2i size, int samples) override;

            TextureBuffer *createTextureBuffer(TextureBuffer::Attributes attributes) override;

            MeshBuffer *createMeshBuffer(const Mesh &mesh) override;

            MeshBuffer *createInstancedMeshBuffer(const Mesh &mesh, const std::vector<Transform> &offsets) override;

            ShaderProgram *createShaderProgram(const std::string &vertexShader,
                                               const std::string &fragmentShader,
                                               const std::map<std::string, std::string> &macros,
                                               const std::function<std::string(
                                                       const char *)> &includeCallback) override;
        };
    }
}

#endif //MANA_OGLRENDERDEVICE_HPP
