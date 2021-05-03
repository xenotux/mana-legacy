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

#ifndef MANA_MONOCPPOBJECT_HPP
#define MANA_MONOCPPOBJECT_HPP

#include <string>

#include "engine/script/mono/monocpparguments.hpp"
#include "engine/script/mono/monocppvalue.hpp"

namespace mana {
    /**
     * A monocpp object wraps a MonoObject instance.
     *
     * It optionally keeps the object pointer pinned to prevent the mono gc from moving / deleting the contained object.
     */
    class MonoCppObject {
    public:
        MonoCppObject();

        explicit MonoCppObject(void *objectPointer, bool pinned = false);

        MonoCppObject(MonoCppObject &&other) noexcept;

        MonoCppObject(const MonoCppObject &other);

        ~MonoCppObject();

        MonoCppObject &operator=(const MonoCppObject &other);

        MonoCppObject invokeMethod(const std::string &name, MonoCppArguments args = {}) const;

        void setField(const std::string &name, MonoCppValue value) const;

        MonoCppObject getField(const std::string &name) const;

        template<typename T>
        T getField(const std::string &name) const {
            T r;
            getFieldValuePtr(name, &r);
            return r;
        }

        void getFieldValuePtr(const std::string &name, void *data) const;

        bool isNull() const;

        bool isPinned() const;

        void *getObjectPointer() const;

        std::string getClassNamespace() const;

        std::string getClassName() const;

        void *unbox() const;

        template<typename T>
        T unbox() const {
            return *static_cast<T *>(unbox());
        }

    private:
        void *objectPointer;
        uint32_t gcHandle;
        bool pinned;
    };
}

#endif //MANA_MONOCPPOBJECT_HPP
