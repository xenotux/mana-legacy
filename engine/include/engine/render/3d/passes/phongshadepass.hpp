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

#ifndef MANA_PHONGSHADEPASS_HPP
#define MANA_PHONGSHADEPASS_HPP

#include "engine/render/3d/renderpass.hpp"

namespace engine {
    class PhongShadePass : public RenderPass {
    public:
        static const char *AMBIENT;
        static const char *DIFFUSE;
        static const char *SPECULAR;
        static const char *COMBINED;

        explicit PhongShadePass(RenderDevice &device);

        ~PhongShadePass() override = default;

        void prepareBuffer(GeometryBuffer &gBuffer) override;

        void render(GeometryBuffer &gBuffer, Scene &scene) override;

    private:
        RenderDevice &renderDevice;

        std::unique_ptr<ShaderProgram> shader;
    };
}

#endif //MANA_PHONGSHADEPASS_HPP