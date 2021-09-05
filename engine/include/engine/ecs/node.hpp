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

#ifndef MANA_NODE_HPP
#define MANA_NODE_HPP

#include <vector>
#include <map>
#include <string>
#include <stdexcept>
#include <typeindex>
#include <functional>

#include "engine/ecs/components.hpp"

namespace mana {
    /**
     * The basic building block of the scene (Entity).
     * A node contains components.
     *
     * Only a single instance per component type may be attached to a node.
     *
     * Uses c++11 typeindex feature.
     */
    struct Node {
        bool enabled = true;
        std::map<std::type_index, Component *> components;

        Node() : enabled(true), components() {}

        ~Node() {
            for (auto &p : components) {
                delete p.second;
            }
        }

        Node(const Node &other) {
            enabled = other.enabled;
            for (auto &p : other.components) {
                components.insert({p.first, p.second->clone()});
            }
        }

        Node &operator=(const Node &other) {
            enabled = other.enabled;
            for (auto &p : other.components) {
                components.insert({p.first, p.second->clone()});
            }
            return *this;
        }

        Node(Node &&other) noexcept {
            enabled = other.enabled;
            for (auto &p : other.components) {
                components[p.first] = p.second;
            }
            other.enabled = true;
            other.components.clear();
        }

        Node &operator=(Node &&other) noexcept {
            enabled = other.enabled;
            for (auto &p : other.components) {
                components[p.first] = p.second;
            }
            other.enabled = true;
            other.components.clear();
            return *this;
        }

        static std::type_index getComponentTypeIndex(ComponentType type) {
            switch (type) {
                case TRANSFORM:
                    return typeid(TransformComponent);
                case CAMERA:
                    return typeid(CameraComponent);
                case LIGHT:
                    return typeid(LightComponent);
                case SCRIPT:
                    return typeid(ScriptComponent);
                case MESH_RENDER:
                    return typeid(MeshRenderComponent);
                case SKYBOX:
                    return typeid(SkyboxComponent);
                default:
                    throw std::runtime_error("Unrecognized component type " + std::to_string(type));
            }
        }

        static ComponentType getComponentType(std::type_index index) {
            if (index == typeid(TransformComponent)) {
                return TRANSFORM;
            } else if (index == typeid(CameraComponent)) {
                return CAMERA;
            } else if (index == typeid(LightComponent)) {
                return LIGHT;
            } else if (index == typeid(ScriptComponent)) {
                return SCRIPT;
            } else if (index == typeid(MeshRenderComponent)) {
                return MESH_RENDER;
            } else if (index == typeid(SkyboxComponent)) {
                return SKYBOX;
            }
        }

        template<typename T>
        T &getComponent() const {
            const std::type_info &typeInfo = typeid(T);
            if (components.find(typeInfo) == components.end())
                throw std::runtime_error("Component of type " + std::string(typeInfo.name()) + " not found");
            return dynamic_cast<T &>(*components.at(typeInfo));
        }

        template<typename T>
        void addComponent(const T &component) {
            if (component == nullptr)
                throw std::runtime_error("null component");
            const std::type_info &typeInfo = typeid(T);
            if (components.find(typeInfo) != components.end())
                throw std::runtime_error("Component of type " + std::string(typeInfo.name()) + " already exists");
            components[typeInfo] = new T(component);
            components[typeInfo]->node = this;
        }

        /**
         * Add a component by pointer, the node takes ownership of the passed pointer.
         *
         * @param component
         */
        void addComponentPointer(Component *const component) {
            if (component == nullptr)
                throw std::runtime_error("null component");
            const std::type_info &typeInfo = component->getTypeInfo();
            if (components.find(typeInfo) != components.end())
                throw std::runtime_error("Component of type " + std::string(typeInfo.name()) + " already exists");
            components[typeInfo] = component;
            components[typeInfo]->node = this;
        }

        template<typename T>
        T *removeComponent() {
            const std::type_info &typeInfo = typeid(T);
            if (components.find(typeInfo) == components.end())
                throw std::runtime_error("Component of type " + std::string(typeInfo.name()) + " not found");
            T *ret = dynamic_cast<T *>(components.at(typeInfo));
            components.erase(typeInfo);
            return ret;
        }

        template<typename T>
        bool hasComponent() {
            const std::type_info &typeInfo = typeid(T);
            return components.find(typeInfo) != components.end();
        }
    };
}
#endif //MANA_NODE_HPP
