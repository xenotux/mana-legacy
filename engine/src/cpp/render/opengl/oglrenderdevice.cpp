#include <mana.hpp>
#include "oglrendertarget.hpp"

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

#include "render/opengl/oglrenderdevice.hpp"
#include "render/opengl/oglrendertexture.hpp"
#include "render/opengl/oglcheckerror.hpp"
#include "render/opengl/oglmeshobject.hpp"
#include "render/opengl/oglshaderprogram.hpp"

#include "render/opengl/oglrenderer.hpp"

namespace mana {
    namespace opengl {
        Renderer *OGLRenderDevice::createRenderer() {
            return new OGLRenderer();
        }

        RenderTarget *OGLRenderDevice::createRenderTarget(Vec2i size, int samples) {
            return new OGLRenderTarget(size, samples);
        }

        TextureBuffer *OGLRenderDevice::createTextureBuffer(TextureBuffer::Attributes attributes) {
            return new OGLRenderTexture(attributes);
        }

        MeshBuffer *OGLRenderDevice::createMeshBuffer(const Mesh &mesh) {
            if (mesh.primitive != TRI) {
                throw std::runtime_error("Unsupported primitive");
            }

            auto *ret = new OGLMeshObject();

            ret->indexed = mesh.indexed;
            ret->instanced = false;

            Mat4f offset(1); // Default instancing offset is identity.

            if (mesh.indexed) {
                ret->elementCount = mesh.indices.size() / 3;

                glGenVertexArrays(1, &ret->VAO);
                glGenBuffers(1, &ret->VBO);
                glGenBuffers(1, &ret->EBO);
                glGenBuffers(1, &ret->instanceVBO);

                glBindVertexArray(ret->VAO);

                glBindBuffer(GL_ARRAY_BUFFER, ret->VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh.vertices.size(), mesh.vertices.data(),
                             GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret->EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.indices.size(), mesh.indices.data(),
                             GL_STATIC_DRAW);

                // position attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
                glEnableVertexAttribArray(0);
                // normal attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (3 * sizeof(float)));
                glEnableVertexAttribArray(1);
                // uv attribute
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (6 * sizeof(float)));
                glEnableVertexAttribArray(2);

                glBindBuffer(GL_ARRAY_BUFFER, ret->instanceVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(Mat4f), &offset, GL_STATIC_DRAW);

                // instanceMatrix attribute
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void *) 0);
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void *) Mat4f::ROW_SIZE);
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(5,
                                      4,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(Mat4f),
                                      (void *) (2 * Mat4f::ROW_SIZE));
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(6,
                                      4,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(Mat4f),
                                      (void *) (3 * Mat4f::ROW_SIZE));

                glVertexAttribDivisor(3, 1);
                glVertexAttribDivisor(4, 1);
                glVertexAttribDivisor(5, 1);
                glVertexAttribDivisor(6, 1);

                glBindVertexArray(0);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            } else {
                ret->elementCount = mesh.vertices.size() / 3;

                glGenVertexArrays(1, &ret->VAO);
                glGenBuffers(1, &ret->VBO);
                glGenBuffers(1, &ret->instanceVBO);

                glBindVertexArray(ret->VAO);

                glBindBuffer(GL_ARRAY_BUFFER, ret->VBO);
                glBufferData(GL_ARRAY_BUFFER,
                             sizeof(Vertex) * mesh.vertices.size(),
                             mesh.vertices.data(),
                             GL_STATIC_DRAW);

                // position attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
                glEnableVertexAttribArray(0);
                // normal attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (3 * sizeof(float)));
                glEnableVertexAttribArray(1);
                // uv attribute
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (6 * sizeof(float)));
                glEnableVertexAttribArray(2);

                glBindBuffer(GL_ARRAY_BUFFER, ret->instanceVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(Mat4f), &offset, GL_STATIC_DRAW);

                // instanceMatrix attribute
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void *) 0);
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void *) Mat4f::ROW_SIZE);
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(5,
                                      4,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(Mat4f),
                                      (void *) (2 * Mat4f::ROW_SIZE));
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(6,
                                      4,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(Mat4f),
                                      (void *) (3 * Mat4f::ROW_SIZE));

                glVertexAttribDivisor(3, 1);
                glVertexAttribDivisor(4, 1);
                glVertexAttribDivisor(5, 1);
                glVertexAttribDivisor(6, 1);

                glBindVertexArray(0);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }

            checkGLError("OGLRenderDevice::createMeshBuffer");

            return ret;
        }

        MeshBuffer *
        OGLRenderDevice::createInstancedMeshBuffer(const Mesh &mesh, const std::vector<Transform> &offsets) {
            if (mesh.primitive != TRI) {
                throw std::runtime_error("Unsupported primitive");
            }

            auto *ret = new OGLMeshObject();

            ret->indexed = mesh.indexed;

            ret->instanced = true;
            ret->instanceCount = offsets.size();

            Mat4f off[offsets.size()];

            for (int i = 0; i < offsets.size(); i++) {
                off[i] = MatrixMath::translate(offsets.at(i).position)
                         * MatrixMath::scale(offsets.at(i).scale)
                         * MatrixMath::rotate(offsets.at(i).rotation);
            }

            if (mesh.indexed) {
                ret->elementCount = mesh.indices.size() / 3;

                glGenVertexArrays(1, &ret->VAO);
                glGenBuffers(1, &ret->VBO);
                glGenBuffers(1, &ret->EBO);
                glGenBuffers(1, &ret->instanceVBO);

                glBindVertexArray(ret->VAO);

                glBindBuffer(GL_ARRAY_BUFFER, ret->VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh.vertices.size(), mesh.vertices.data(),
                             GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret->EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                             sizeof(uint) * mesh.indices.size(),
                             mesh.indices.data(),
                             GL_STATIC_DRAW);

                // position attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
                glEnableVertexAttribArray(0);
                // normal attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (3 * sizeof(float)));
                glEnableVertexAttribArray(1);
                // uv attribute
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (6 * sizeof(float)));
                glEnableVertexAttribArray(2);

                glBindBuffer(GL_ARRAY_BUFFER, ret->instanceVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(Mat4f) * offsets.size(), &off[0], GL_STATIC_DRAW);

                // instanceMatrix attribute
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void *) 0);
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void *) Mat4f::ROW_SIZE);
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(5,
                                      4,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(Mat4f),
                                      (void *) (2 * Mat4f::ROW_SIZE));
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(6,
                                      4,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(Mat4f),
                                      (void *) (3 * Mat4f::ROW_SIZE));

                glVertexAttribDivisor(3, 1);
                glVertexAttribDivisor(4, 1);
                glVertexAttribDivisor(5, 1);
                glVertexAttribDivisor(6, 1);

                glBindVertexArray(0);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            } else {
                ret->elementCount = mesh.vertices.size() / 3;

                glGenVertexArrays(1, &ret->VAO);
                glGenBuffers(1, &ret->VBO);
                glGenBuffers(1, &ret->instanceVBO);

                glBindVertexArray(ret->VAO);

                glBindBuffer(GL_ARRAY_BUFFER, ret->VBO);
                glBufferData(GL_ARRAY_BUFFER,
                             sizeof(Vertex) * mesh.vertices.size(),
                             mesh.vertices.data(),
                             GL_STATIC_DRAW);

                // position attribute
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
                glEnableVertexAttribArray(0);
                // normal attribute
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (3 * sizeof(float)));
                glEnableVertexAttribArray(1);
                // uv attribute
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (6 * sizeof(float)));
                glEnableVertexAttribArray(2);

                glBindBuffer(GL_ARRAY_BUFFER, ret->instanceVBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(Mat4f) * offsets.size(), &off[0], GL_STATIC_DRAW);

                // instanceMatrix attribute
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void *) 0);
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4f), (void *) Mat4f::ROW_SIZE);
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(5,
                                      4,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(Mat4f),
                                      (void *) (2 * Mat4f::ROW_SIZE));
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(6,
                                      4,
                                      GL_FLOAT,
                                      GL_FALSE,
                                      sizeof(Mat4f),
                                      (void *) (3 * Mat4f::ROW_SIZE));

                glVertexAttribDivisor(3, 1);
                glVertexAttribDivisor(4, 1);
                glVertexAttribDivisor(5, 1);
                glVertexAttribDivisor(6, 1);

                glBindVertexArray(0);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }

            checkGLError("OGLRenderDevice::createMeshInstanced");

            return ret;
        }

        ShaderProgram *OGLRenderDevice::createShaderProgram(const std::string &vertexShader,
                                                            const std::string &fragmentShader,
                                                            const std::map<std::string, std::string> &macros,
                                                            const std::function<std::string(
                                                                    const char *)> &includeCallback) {
            return new OGLShaderProgram(vertexShader, fragmentShader, macros, includeCallback);
        }
    }
}