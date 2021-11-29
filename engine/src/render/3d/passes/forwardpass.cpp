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

#include "engine/render/3d/passes/forwardpass.hpp"

#include "engine/math/rotation.hpp"

namespace engine {
    const char *ForwardPass::COLOR = "forward";
    const char *ForwardPass::DEPTH = "forward_depth";

    ForwardPass::ForwardPass(RenderDevice &device)
            : device(device) {}

    ForwardPass::~ForwardPass() = default;

    void ForwardPass::prepareBuffer(GeometryBuffer &gBuffer) {
        gBuffer.addBuffer(COLOR, TextureBuffer::ColorFormat::RGBA);
        gBuffer.addBuffer(DEPTH, TextureBuffer::ColorFormat::DEPTH_STENCIL);
    }

    void ForwardPass::render(GeometryBuffer &gBuffer, Scene &scene) {
        gBuffer.attachColor({COLOR});
        gBuffer.attachDepthStencil(DEPTH);

        Mat4f model, view, projection, cameraTranslation;
        view = scene.camera.view();
        projection = scene.camera.projection();
        cameraTranslation = MatrixMath::translate(scene.camera.transform.getPosition());

        auto &ren = device.getRenderer();

        // Clear forward color and depth textures
        ren.renderBegin(gBuffer.getRenderTarget(), RenderOptions({},
                                                                 gBuffer.getRenderTarget().getSize(),
                                                                 true,
                                                                 {},
                                                                 1,
                                                                 true, true, true));

        for (auto &unit: scene.forward) {
            model = unit.transform.model();

            ShaderProgram &shader = unit.command.shader;

            shader.setMat4("MANA_M", model);
            shader.setMat4("MANA_V", view);
            shader.setMat4("MANA_P", projection);
            shader.setMat4("MANA_MVP", projection * view * model);
            shader.setMat4("MANA_M_INVERT", MatrixMath::inverse(model));
            shader.setMat4("MANA_VIEW_TRANSLATION", cameraTranslation);

            int dirCount = 0;
            int pointCount = 0;
            int spotCount = 0;

            int i = 0;
            for (auto &light: scene.lights) {
                std::string name;
                switch (light.type) {
                    case LIGHT_DIRECTIONAL:
                        name = "DIRECTIONAL_LIGHTS[" + std::to_string(i++) + "].";
                        shader.setVec3(name + "direction", light.direction);
                        shader.setVec3(name + "ambient", light.ambient);
                        shader.setVec3(name + "diffuse", light.diffuse);
                        shader.setVec3(name + "specular", light.specular);
                        dirCount++;
                        break;
                    case LIGHT_POINT:
                        name = "POINT_LIGHTS[" + std::to_string(i++) + "].";
                        shader.setVec3(name + "position", light.transform.getPosition());
                        shader.setFloat(name + "constantValue", light.constant);
                        shader.setFloat(name + "linearValue", light.linear);
                        shader.setFloat(name + "quadraticValue", light.quadratic);
                        shader.setVec3(name + "ambient", light.ambient);
                        shader.setVec3(name + "diffuse", light.diffuse);
                        shader.setVec3(name + "specular", light.specular);
                        pointCount++;
                        break;
                    case LIGHT_SPOT:
                        name = "SPOT_LIGHTS[" + std::to_string(i++) + "].";
                        shader.setVec3(name + "position", light.transform.getPosition());
                        shader.setVec3(name + "direction", light.direction);
                        shader.setFloat(name + "cutOff", cosf(degreesToRadians(light.cutOff)));
                        shader.setFloat(name + "outerCutOff", cosf(degreesToRadians(light.outerCutOff)));
                        shader.setFloat(name + "constantValue", light.constant);
                        shader.setFloat(name + "linearValue", light.linear);
                        shader.setFloat(name + "quadraticValue", light.quadratic);
                        shader.setVec3(name + "ambient", light.ambient);
                        shader.setVec3(name + "diffuse", light.diffuse);
                        shader.setVec3(name + "specular", light.specular);
                        spotCount++;
                        break;
                }
            }

            shader.setInt("DIRECTIONAL_LIGHTS_COUNT", dirCount);
            shader.setInt("POINT_LIGHTS_COUNT", pointCount);
            shader.setInt("SPOT_LIGHTS_COUNT", spotCount);

            shader.setVec3("MANA_VIEWPOS", scene.camera.transform.getPosition());

            ren.addCommand(unit.command);
        }

        ren.renderFinish();
    }
}