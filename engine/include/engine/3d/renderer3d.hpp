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

#ifndef MANA_RENDERER3D_HPP
#define MANA_RENDERER3D_HPP

#include <utility>
#include <map>
#include <functional>

#include "engine/render/renderer.hpp"
#include "engine/render/renderdevice.hpp"
#include "engine/render/shadercompiler.hpp"

#include "engine/3d/scene.hpp"
#include "engine/3d/renderpass.hpp"
#include "engine/3d/compositor.hpp"

namespace engine {
    class Renderer3D {
    public:
        static const std::map<std::string, std::string> &getShaderMacros(ShaderCompiler::ShaderLanguage lang);

        static const std::function<std::string(const char *)> &getShaderIncludeCallback();

        Renderer3D(RenderDevice &device,
                   std::vector<std::unique_ptr<RenderPass>> passes,
                   std::vector<Compositor::Layer> layers);

        ~Renderer3D();

        void render(RenderTarget &target, Scene &scene);

        GeometryBuffer &getGeometryBuffer();

        Compositor &getCompositor();

    private:
        RenderDevice &device;
        std::vector<std::unique_ptr<RenderPass>> passes;
        GeometryBuffer geometryBuffer;
        Compositor compositor;
    };
}

#endif //MANA_RENDERER3D_HPP