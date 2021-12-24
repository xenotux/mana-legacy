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

#ifndef MANA_ECSSCHEMA_HPP
#define MANA_ECSSCHEMA_HPP

#include "engine/io/message.hpp"

#include "engine/ecs/components/audio/audiosourcecomponent.hpp"
#include "engine/ecs/components/audio/audiolistenercomponent.hpp"

#include "engine/ecs/components.hpp"
#include "engine/ecs/entitymanager.hpp"

namespace engine {
    //TODO: Design better schema generation method
    CameraType &operator<<(CameraType &value, const Message &message);

    Message &operator<<(Message &message, CameraType type);

    LightType &operator<<(LightType &value, const Message &message);

    Message &operator<<(Message &message, LightType type);

    AssetPath &operator<<(AssetPath &value, const Message &message);

    Message &operator<<(Message &message, const AssetPath &path);

    CameraComponent &operator<<(CameraComponent &component, const Message &message);

    Message &operator<<(Message &message, const CameraComponent &component);

    TransformComponent &operator<<(TransformComponent &component, const Message &message);

    Message &operator<<(Message &message, const TransformComponent &component);

    MeshRenderComponent &operator<<(MeshRenderComponent &component, const Message &message);

    Message &operator<<(Message &message, const MeshRenderComponent &component);

    SkyboxComponent &operator<<(SkyboxComponent &component, const Message &message);

    Message &operator<<(Message &message, const SkyboxComponent &component);

    LightComponent &operator<<(LightComponent &component, const Message &message);

    Message &operator<<(Message &message, const LightComponent &component);

    MonoScriptComponent &operator<<(MonoScriptComponent &component, const Message &message);

    Message &operator<<(Message &message, const MonoScriptComponent &component);

    MonoSyncComponent &operator<<(MonoSyncComponent &component, const Message &message);

    Message &operator<<(Message &message, const MonoSyncComponent &component);

    AudioSourceComponent &operator<<(AudioSourceComponent &component, const Message &message);

    Message &operator <<(Message &message, const AudioSourceComponent &component);

    AudioListenerComponent &operator<<(AudioListenerComponent &component, const Message &message);

    Message &operator <<(Message &message, const AudioListenerComponent &component);

    EntityManager &operator<<(EntityManager &manager, const Message &message);

    Message &operator<<(Message &message, const EntityManager &manager);
}

#endif //MANA_ECSSCHEMA_HPP
