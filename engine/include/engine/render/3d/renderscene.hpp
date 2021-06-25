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

#ifndef MANA_RENDERSCENE_HPP
#define MANA_RENDERSCENE_HPP

#include "engine/render/3d/deferredcommand.hpp"
#include "engine/render/3d/forwardcommand.hpp"
#include "engine/render/3d/camera.hpp"
#include "engine/render/3d/light.hpp"

namespace mana {
    struct RenderScene {
        Camera camera;

        std::vector<Light> lights;

        std::vector<DeferredCommand> deferred; //The commands to draw in the deferred pipeline

        // The commands to draw in the forward pipeline,
        // the forward pipeline is executed after the deferred pipeline.
        // The depth information from the deferred pass is preserved in the depth buffer.
        std::vector<ForwardCommand> forward;

        TextureBuffer* skybox; //The cubemap texture of the skybox if not null it is drawn after the forward pipeline.
    };
}

#endif //MANA_RENDERSCENE_HPP
