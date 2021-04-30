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

#ifndef MANA_GLFWMONITOR_HPP
#define MANA_GLFWMONITOR_HPP

#include <GLFW/glfw3.h>

#include "engine/display/monitor.hpp"

namespace mana {
    namespace glfw {
        class GLFWMonitor : public Monitor {
        public:
            ~GLFWMonitor() override = default;

            Vec2i getVirtualPosition() override {
                int posx, posy;
                glfwGetMonitorPos(monH, &posx, &posy);
                return {posx, posy};
            }

            Recti getWorkArea() override {
                int posx, posy, width, height;
                glfwGetMonitorWorkarea(monH, &posx, &posy, &width, &height);
                return {Vec2i(posx, posy), Vec2i(width, height)};
            }

            Vec2i getPhysicalSize() override {
                int sizex, sizey;
                glfwGetMonitorPhysicalSize(monH, &sizex, &sizey);
                return {sizex, sizey};
            }

            Vec2f getContentScale() override {
                float scalex, scaley;
                glfwGetMonitorContentScale(monH, &scalex, &scaley);
                return {scalex, scaley};
            }

            std::string getName() override {
                return std::string(glfwGetMonitorName(monH));
            }

            VideoMode getVideoMode() override {
                const GLFWvidmode *v = glfwGetVideoMode(monH);
                VideoMode ret{};
                ret.width = v->width;
                ret.height = v->height;
                ret.redBits = v->redBits;
                ret.greenBits = v->greenBits;
                ret.blueBits = v->blueBits;
                ret.refreshRate = v->refreshRate;
                return ret;
            }

            std::vector<VideoMode> getSupportedVideoModes() override {
                int count;
                const GLFWvidmode *modes = glfwGetVideoModes(monH, &count);

                std::vector<VideoMode> ret;
                for (int i = 0; i < count; i++) {
                    VideoMode r{};
                    const GLFWvidmode *v = &modes[i];

                    r.width = v->width;
                    r.height = v->height;
                    r.redBits = v->redBits;
                    r.greenBits = v->greenBits;
                    r.blueBits = v->blueBits;
                    r.refreshRate = v->refreshRate;

                    ret.emplace_back(r);
                }

                return ret;
            }

            GLFWmonitor *monH = nullptr;
        };
    }
}

#endif //MANA_GLFWMONITOR_HPP
