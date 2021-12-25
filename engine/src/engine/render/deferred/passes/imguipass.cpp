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

#ifdef BUILD_ENGINE_COMPAT_IMGUI

#include "engine/render/deferred/passes/imguipass.hpp"

#include "engine/compat/imgui/imguicompat.hpp"

#include "imgui.h"

namespace engine {
    const char *ImGuiPass::COLOR = "imgui";

    ImGuiPass::ImGuiPass(Window &window, std::vector<std::reference_wrapper<Widget>> commands)
            : window(window), widgets(std::move(commands)) {}

    ImGuiPass::~ImGuiPass() {}

    void ImGuiPass::prepareBuffer(GeometryBuffer &gBuffer) {
        gBuffer.addBuffer(COLOR, TextureBuffer::RGBA);
    }

    void ImGuiPass::render(GeometryBuffer &gBuffer, Scene &scene, AssetRenderManager &assetRenderManager) {
        ImGuiCompat::NewFrame(window);
        ImGui::NewFrame();

        for (auto &command: widgets) {
            command.get().draw(scene);
        }

        gBuffer.attachColor({COLOR});
        gBuffer.detachDepthStencil();

        ImGui::Render();
        ImGuiCompat::DrawData(window, gBuffer.getRenderTarget());
    }

    void ImGuiPass::setWidgets(const std::vector<std::reference_wrapper<Widget>> &w) {
        widgets = w;
    }
}

#endif