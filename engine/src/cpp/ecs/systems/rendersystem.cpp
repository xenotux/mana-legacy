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

#include <algorithm>

#include "engine/ecs/systems/rendersystem.hpp"

#include "engine/ecs/components.hpp"

#include "engine/render/camera/perspectivecamera.hpp"
#include "engine/render/camera/orthographiccamera.hpp"

namespace mana {
    RenderSystem::RenderSystem(const RenderTarget &scr, Renderer3D &ren, Resources &res)
            : screenTarget(scr), ren(ren), res(res) {
    }

    void RenderSystem::start() {

    }

    void RenderSystem::stop() {

    }

    void RenderSystem::update(float deltaTime, Scene &scene) {
        Renderer3D::RenderScene scene3d;

        auto nodes = scene.findNodesWithComponent<LightComponent>();
        for (auto *nodePointer : nodes) {
            auto &node = *nodePointer;
            if (!node.enabled)
                continue;

            auto comp = node.getComponent<LightComponent>();
            if (!comp.enabled)
                continue;

            if (comp.lightType == LIGHT_DIRECTIONAL) {
                DirectionalLight light;
                light.ambient = comp.ambient;
                light.diffuse = comp.diffuse;
                light.specular = comp.specular;

                light.direction = comp.direction;

                scene3d.dir.emplace_back(light);
            } else if (comp.lightType == LIGHT_POINT) {
                PointLight light;
                light.ambient = comp.ambient;
                light.diffuse = comp.diffuse;
                light.specular = comp.specular;

                light.constant = comp.constant;
                light.linear = comp.linear;
                light.quadratic = comp.quadratic;

                scene3d.point.emplace_back(light);
            } else if (comp.lightType == LIGHT_SPOT) {
                SpotLight light;
                light.ambient = comp.ambient;
                light.diffuse = comp.diffuse;
                light.specular = comp.specular;

                light.cutOff = comp.cutOff;
                light.outerCutOff = comp.outerCutOff;
                light.constant = comp.constant;
                light.linear = comp.linear;
                light.quadratic = comp.quadratic;

                scene3d.spot.emplace_back(light);
            }
        }

        std::map<RenderComponent *, TransformComponent *> mapping;
        std::vector<RenderComponent *> renderComponents;
        nodes = scene.findNodesWithComponent<RenderComponent>();
        for (auto &nodePointer : nodes) {
            auto &node = *nodePointer;
            if (!node.enabled)
                continue;

            auto &comp = node.getComponent<RenderComponent>();
            if (!comp.enabled)
                continue;

            auto &tcomp = node.getComponent<TransformComponent>();
            if (!tcomp.enabled)
                continue;

            renderComponents.emplace_back(&comp);
            mapping[&comp] = &tcomp;
        }

        std::sort(renderComponents.begin(), renderComponents.end(),
                  [](const RenderComponent *a, const RenderComponent *b) -> bool {
                      return a->renderOrder < b->renderOrder;
                  });

        for (auto *comp : renderComponents) {
            Renderer3D::Unit unit;

            unit.transform = mapping[comp]->transform;

            unit.command.shader = res.getResource<ShaderResource>(comp->shaderResourceName).getShader();
            for (auto &m : comp->textureMapping) {
                unit.command.shader->setTexture(m.first, m.second);
            }

            for (auto &t : comp->textureResourceNames) {
                unit.command.textures.emplace_back(res.getResource<TextureResource>(t).getTexture());
            }

            for (auto &m : comp->meshResourceNames) {
                unit.command.meshBuffers.emplace_back(res.getResource<MeshBufferResource>(m).getRenderMesh());
            }

            unit.command.properties.enableDepthTest = comp->renderProperties.enableDepthTest;
            unit.command.properties.depthTestWrite = comp->renderProperties.depthTestWrite;
            unit.command.properties.depthTestMode = comp->renderProperties.depthTestMode;

            unit.command.properties.enableStencilTest = comp->renderProperties.enableStencilTest;
            unit.command.properties.stencilTestMask = comp->renderProperties.stencilTestMask;
            unit.command.properties.stencilMode = comp->renderProperties.stencilMode;
            unit.command.properties.stencilReference = comp->renderProperties.stencilReference;
            unit.command.properties.stencilFunctionMask = comp->renderProperties.stencilFunctionMask;
            unit.command.properties.stencilFail = comp->renderProperties.stencilFail;
            unit.command.properties.stencilDepthFail = comp->renderProperties.stencilDepthFail;
            unit.command.properties.stencilPass = comp->renderProperties.stencilPass;

            unit.command.properties.enableFaceCulling = comp->renderProperties.enableFaceCulling;
            unit.command.properties.faceCullMode = comp->renderProperties.faceCullMode;
            unit.command.properties.faceCullClockwiseWinding = comp->renderProperties.faceCullClockwiseWinding;

            unit.command.properties.enableBlending = comp->renderProperties.enableBlending;
            unit.command.properties.blendSourceMode = comp->renderProperties.blendSourceMode;
            unit.command.properties.blendDestinationMode = comp->renderProperties.blendDestinationMode;

            scene3d.units.emplace_back(unit);
        }

        nodes = scene.findNodesWithComponent<CameraComponent>();
        Node *cameraNode;
        for (auto &node : nodes) {
            if (!node->enabled)
                continue;
            auto &comp = node->getComponent<CameraComponent>();
            if (!comp.enabled)
                continue;
            auto &tcomp = node->getComponent<TransformComponent>();
            if (!tcomp.enabled)
                continue;
            cameraNode = node;
            break;
        }

        auto &comp = cameraNode->getComponent<CameraComponent>();
        auto &tcomp = cameraNode->getComponent<TransformComponent>();
        if (comp.cameraType == PERSPECTIVE) {
            perspectiveCamera.transform = tcomp.transform;
            perspectiveCamera.nearClip = comp.nearClip;
            perspectiveCamera.farClip = comp.farClip;
            perspectiveCamera.fov = comp.fov;
            perspectiveCamera.aspectRatio = comp.aspectRatio;
            scene3d.camera = &perspectiveCamera;
        } else {
            orthoCamera.transform = tcomp.transform;
            orthoCamera.nearClip = comp.nearClip;
            orthoCamera.farClip = comp.farClip;
            orthoCamera.left = comp.left;
            orthoCamera.top = comp.top;
            orthoCamera.right = comp.right;
            orthoCamera.bottom = comp.bottom;
            scene3d.camera = &orthoCamera;
        }
        ren.render(screenTarget, scene3d);
    }
}