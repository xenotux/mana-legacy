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

#ifndef MANA_OGLRENDERER_HPP
#define MANA_OGLRENDERER_HPP

#include "engine/render/renderer.hpp"

#include "render/opengl/oglrenderdevice.hpp"

namespace mana {
    namespace opengl {
        class OGLRenderer : public Renderer {
        public:
            void setViewport(Vec2i offset, Vec2i size) override;

            void setClear(bool clearColor, bool clearDepth, bool clearStencil) override;

            void setClearColor(ColorRGBA clearColor) override;

            void setMultiSample(bool multiSample) override;

            void renderBegin(RenderTarget &target) override;

            void addCommand(RenderCommand &command) override;

            void renderFinish() override;

        private:
            Vec2i viewportOffset = {};
            Vec2i viewportSize = {};
            ColorRGBA clearColorValue = {};
            bool clearColor = true;
            bool clearDepth = true;
            bool clearStencil = true;
            bool multiSample = false;
        };
    }
}

#endif //MANA_OGLRENDERER_HPP
