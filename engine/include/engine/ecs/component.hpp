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

#ifndef MANA_COMPONENT_HPP
#define MANA_COMPONENT_HPP

#include "engine/ecs/componenttype.hpp"

#include <memory>

namespace mana {
    struct Node;

    /**
     * A Component provides state.
     */
    struct Component {
        const ComponentType componentType;

        explicit Component(ComponentType type) : componentType(type) {}

        virtual ~Component() = default;

        virtual Component *clone() = 0;

        virtual const std::type_info& getTypeInfo() = 0;

        bool enabled = true;
        Node *node = nullptr;

        std::shared_ptr<void> userData = nullptr;
    };
}

#endif //MANA_COMPONENT_HPP
