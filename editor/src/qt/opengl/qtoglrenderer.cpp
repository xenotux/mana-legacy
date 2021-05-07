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

#include <string>
#include <set>

#include <QOpenGLVertexArrayObject>

#include "engine/math/matrixmath.hpp"
#include "engine/math/rotation.hpp"

#include "qtoglrenderer.hpp"

#include "qtoglshaderprogram.hpp"
#include "qtoglrendertexture.hpp"
#include "qtoglmeshobject.hpp"
#include "qtoglframebuffer.hpp"

#include "qtoglcheckerror.hpp"
#include "qtogltypeconverter.hpp"

namespace mana {
    namespace opengl {
        GLuint getTextureSlot(int slot) {
            switch (slot) {
                case 0:
                    return GL_TEXTURE0;
                case 1:
                    return GL_TEXTURE1;
                case 2:
                    return GL_TEXTURE2;
                case 3:
                    return GL_TEXTURE3;
                case 4:
                    return GL_TEXTURE4;
                case 5:
                    return GL_TEXTURE5;
                case 6:
                    return GL_TEXTURE6;
                case 7:
                    return GL_TEXTURE7;
                case 8:
                    return GL_TEXTURE8;
                case 9:
                    return GL_TEXTURE9;
                default:
                    throw std::runtime_error("Maximum 10 texture slots");
            }
        }

        void QtOGLRenderer::setViewport(Vec2i offset, Vec2i size) {
            this->viewportOffset = offset;
            this->viewportSize = size;
        }

        void QtOGLRenderer::setClear(bool cC, bool cD, bool cS) {
            this->clearColor = cC;
            this->clearDepth = cD;
            this->clearStencil = cS;
        }

        void QtOGLRenderer::setClearColor(ColorRGBA c) {
            this->clearColorValue = c;
        }

        void QtOGLRenderer::setMultiSample(bool s) {
            this->multiSample = s;
        }

        void QtOGLRenderer::renderBegin(const RenderTarget &target) {
            glClearColor((float) clearColorValue.r() / (float) 255,
                         (float) clearColorValue.g() / (float) 255,
                         (float) clearColorValue.b() / (float) 255,
                         (float) clearColorValue.a() / (float) 255);

            if (multiSample)
                glEnable(GL_MULTISAMPLE);
            else
                glDisable(GL_MULTISAMPLE);

            auto &fb = dynamic_cast<const QtOGLFrameBuffer &>(target);

            GLint vpData[4];
            glGetIntegerv(GL_VIEWPORT, vpData);

            glViewport(viewportOffset.x, viewportOffset.y, viewportSize.x, viewportSize.y);

            glBindFramebuffer(GL_FRAMEBUFFER, fb.getFBO());

            GLbitfield clearMask = 0;
            if (clearColor) {
                clearMask |= GL_COLOR_BUFFER_BIT;
            }

            if (clearDepth) {
                clearMask |= GL_DEPTH_BUFFER_BIT;
            }

            if (clearStencil) {
                clearMask |= GL_STENCIL_BUFFER_BIT;
            }

            glClear(clearMask);
        }

        void QtOGLRenderer::addCommand(const RenderCommand &command) {
            //Bind textures
            for (int i = 0; i < command.textures.size(); i++) {
                auto *textureObject = command.textures.at(i);
                if (textureObject == nullptr) {
                    throw std::runtime_error("nullptr texture");
                }
                auto &texture = dynamic_cast<const QtOGLRenderTexture &>(*textureObject);
                glActiveTexture(getTextureSlot(i));
                glBindTexture(QtOGLTypeConverter::convert(texture.attributes.textureType), texture.handle);
            }

            //Bind shader program
            ShaderProgram *sp = command.shader;
            if (sp == nullptr) {
                throw std::runtime_error("nullptr shaderprogram");
            }
            auto &shader = dynamic_cast<QtOGLShaderProgram &>(*sp);
            shader.activate();

            glDepthFunc(QtOGLTypeConverter::convert(command.properties.depthTestMode));
            if (command.properties.depthTestWrite)
                glDepthMask(GL_TRUE);
            else
                glDepthMask(GL_FALSE);

            //Setup per model depth, stencil, culling and blend states
            if (command.properties.enableDepthTest) {
                glEnable(GL_DEPTH_TEST);
            } else {
                glDisable(GL_DEPTH_TEST);
            }

            glStencilMask(QtOGLTypeConverter::convertPrimitive(command.properties.stencilTestMask));
            glStencilFunc(QtOGLTypeConverter::convert(command.properties.stencilMode),
                          QtOGLTypeConverter::convertPrimitive(command.properties.stencilReference),
                          QtOGLTypeConverter::convertPrimitive(command.properties.stencilFunctionMask));
            glStencilOp(QtOGLTypeConverter::convert(command.properties.stencilFail),
                        QtOGLTypeConverter::convert(command.properties.stencilDepthFail),
                        QtOGLTypeConverter::convert(command.properties.stencilPass));

            if (command.properties.enableStencilTest) {
                glEnable(GL_STENCIL_TEST);
            } else {
                glDisable(GL_STENCIL_TEST);
            }

            glCullFace(QtOGLTypeConverter::convert(command.properties.faceCullMode));
            if (command.properties.faceCullClockwiseWinding)
                glFrontFace(GL_CW);
            else
                glFrontFace(GL_CCW);

            if (command.properties.enableFaceCulling) {
                glEnable(GL_CULL_FACE);
            } else {
                glDisable(GL_CULL_FACE);
            }

            glBlendFunc(QtOGLTypeConverter::convert(command.properties.blendSourceMode),
                        QtOGLTypeConverter::convert(command.properties.blendDestinationMode));

            if (command.properties.enableBlending) {
                glEnable(GL_BLEND);
            } else {
                glDisable(GL_BLEND);
            }

            //Bind VAOs and draw.
            for (auto *meshBuffer : command.meshBuffers) {
                if (meshBuffer == nullptr) {
                    throw std::runtime_error("nullptr mesh");
                }
                auto &mesh = dynamic_cast<const QtOGLMeshObject &>(*meshBuffer);

                glBindVertexArray(mesh.VAO);
                if (mesh.indexed) {
                    if (mesh.instanced)
                        glDrawElementsInstanced(GL_TRIANGLES, mesh.elementCount * 3, GL_UNSIGNED_INT, 0,
                                                mesh.instanceCount);
                    else
                        glDrawElements(GL_TRIANGLES, mesh.elementCount * 3, GL_UNSIGNED_INT, 0);
                } else {
                    if (mesh.instanced)
                        glDrawArraysInstanced(GL_TRIANGLES, 0, mesh.elementCount * 3, mesh.instanceCount);
                    else
                        glDrawArrays(GL_TRIANGLES, 0, mesh.elementCount * 3);
                }
                glBindVertexArray(0);
            }

            //Unbind textures
            for (int y = 0; y < 10; y++) {
                glActiveTexture(getTextureSlot(y));
                glBindTexture(GL_TEXTURE_2D, 0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
            }

            //Reset Stencil mask
            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);

            checkQtGLError("QtOGLRenderer::addCommand");
        }

        void QtOGLRenderer::renderFinish() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            checkQtGLError("QtOGLRenderer::renderFinish");
        }
    }
}