/**
 *  Mana - 3D Engine
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

#include "render/opengl/oglcheckerror.hpp"

#include "extern/glad.h"

void checkGLError(const std::string &source) {
    GLenum er = glGetError();
    if (er != GL_NO_ERROR) {
        std::string error = source + " GLERROR: ";
        error += std::to_string(er);
        fprintf(stderr, "%s\n", error.c_str());
    }
}