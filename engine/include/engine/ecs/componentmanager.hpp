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

#ifndef MANA_COMPONENTMANAGER_HPP
#define MANA_COMPONENTMANAGER_HPP

#include <map>
#include <vector>
#include <typeindex>

#include "engine/ecs/entity.hpp"

#include "engine/ecs/componentpool.hpp"

namespace engine {
    /**
     * Any default constructable type can be used as a component.
     *
     * The engine defines some default components which are used by provided default systems (Rendering, Physics etc.)
     * The user can define custom component types and systems which use these types.
     */
    class ComponentManager {
    public:
        ComponentManager() = default;

        ~ComponentManager() {
            for (auto &p : pools) {
                delete p.second;
            }
        }

        ComponentManager(const ComponentManager &other) {
            for (auto &p : other.pools) {
                pools[p.first] = p.second->clone();
            }
        }

        ComponentManager(ComponentManager &&other) noexcept = default;

        ComponentManager &operator=(const ComponentManager &other) = default;

        ComponentManager &operator=(ComponentManager &&other)  noexcept = default;

        template<typename T>
        ComponentPool<T> &getPool() {
            auto it = pools.find(typeid(T));
            if (it == pools.end()) {
                pools[typeid(T)] = new ComponentPool<T>();
            }
            return dynamic_cast<ComponentPool<T> &>(*pools[typeid(T)]);
        }

        template<typename T>
        typename std::map<Entity, T>::iterator begin() {
            return getPool<T>().begin();
        }

        template<typename T>
        typename std::map<Entity, T>::iterator begin() const {
            return getPool<T>().begin();
        }

        template<typename T>
        typename std::map<Entity, T>::iterator end() {
            return getPool<T>().end();
        }

        template<typename T>
        typename std::map<Entity, T>::iterator end() const {
            return getPool<T>().end();
        }

        template<typename T>
        T &create(const Entity &entity) {
            return getPool<T>().create(entity);
        }

        template<typename T>
        void destroy(const Entity &entity) {
            getPool<T>()->destroy(entity);
        }

        void destroy(const Entity &entity) {
            for (auto &p : pools) {
                p.second->destroy(entity);
            }
        }

        void clear() {
            for (auto &p : pools) {
                delete p.second;
            }
            pools.clear();
        }

        template<typename T>
        T &lookup(const Entity &entity) {
            return getPool<T>().lookup(entity);
        }

        template<typename T>
        bool check(const Entity &entity) const {
            return getPool<T>().check(entity);
        }

    private:
        std::map<std::type_index, ComponentPoolBase *> pools;
    };
}

#endif //MANA_COMPONENTMANAGER_HPP