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

#include <map>
#include <stdexcept>

#include "glfwinput.hpp"

#include "glfwtypeconverter.hpp"

namespace mana {
    std::map<GLFWwindow *, GLFWInput *> windowMapping;

    void glfwKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (windowMapping.find(window) != windowMapping.end()) {
            windowMapping[window]->glfwKeyCallback(key, scancode, action, mods);
        } else {
            fprintf(stderr, "Received glfw callback with non registered window.");
        }
    }

    void glfwCursorCallback(GLFWwindow *window, double xpos, double ypos) {
        if (windowMapping.find(window) != windowMapping.end()) {
            windowMapping[window]->glfwCursorCallback(xpos, ypos);
        } else {
            fprintf(stderr, "Received glfw callback with non registered window.");
        }
    }

    void glfwMouseKeyCallback(GLFWwindow *window, int button, int action, int mods) {
        if (windowMapping.find(window) != windowMapping.end()) {
            windowMapping[window]->glfwMouseKeyCallback(button, action, mods);
        } else {
            fprintf(stderr, "Received glfw callback with non registered window.");
        }
    }

    GLFWInput::GLFWInput(GLFWwindow &wndH) : wndH(wndH) {
        windowMapping[&wndH] = this;
        glfwSetKeyCallback(&wndH, mana::glfwKeyCallback);
        glfwSetCursorPosCallback(&wndH, mana::glfwCursorCallback);
        glfwSetMouseButtonCallback(&wndH, mana::glfwMouseKeyCallback);
    }

    GLFWInput::~GLFWInput() {
        windowMapping.erase(&wndH);
    }

    void GLFWInput::glfwKeyCallback(int key, int scancode, int action, int mods) {
        Key k = GLFWTypeConverter::convertKey(key);
        bool kd = action != GLFW_RELEASE;
        keys[k] = kd;
        for (auto listener : listeners) {
            if (kd)
                listener->onKeyDown(k);
            else
                listener->onKeyUp(k);
        }
    }

    void GLFWInput::glfwCursorCallback(double xpos, double ypos) {
        mouse.position.x = xpos;
        mouse.position.y = ypos;
        for (auto listener : listeners) {
            listener->onMouseMove(xpos, ypos);
        }
    }

    void GLFWInput::glfwMouseKeyCallback(int button, int action, int mods) {
        switch (button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                mouse.leftButtonDown = action == GLFW_PRESS;
                for (auto listener : listeners) {
                    listener->onMouseKeyDown(MouseKey::MOUSE_LEFT);
                }
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                mouse.middleButtonDown = action == GLFW_PRESS;
                for (auto listener : listeners) {
                    listener->onMouseKeyDown(MouseKey::MOUSE_MIDDLE);
                }
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                mouse.rightButtonDown = action == GLFW_PRESS;
                for (auto listener : listeners) {
                    listener->onMouseKeyDown(MouseKey::MOUSE_RIGHT);
                }
                break;
        }
    }

    bool GLFWInput::getKeyDown(Key key) {
        return keys[key];
    }

    Mouse GLFWInput::getMouse() {
        return mouse;
    }

    void GLFWInput::registerListener(InputListener &listener) {
        if (listeners.find(&listener) != listeners.end())
            throw std::runtime_error("Input listener already registered");
        listeners.insert(&listener);
    }

    void GLFWInput::unregisterListener(InputListener &listener) {
        listeners.erase(&listener);
    }

    void GLFWInput::setClipboardText(std::string text) {
        throw std::runtime_error("Not Implemented");
    }

    std::string GLFWInput::getClipboardText() {
        throw std::runtime_error("Not Implemented");
    }
}