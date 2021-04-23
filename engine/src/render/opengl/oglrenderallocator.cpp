#include "render/opengl/oglrenderallocator.hpp"
#include "render/opengl/oglrendertexture.hpp"
#include "render/opengl/ogluserframebuffer.hpp"
#include "render/opengl/oglcheckerror.hpp"
#include "render/opengl/ogltypeconverter.hpp"
#include "render/opengl/oglmeshobject.hpp"
#include "render/opengl/oglshaderprogram.hpp"

#include "engine/math/matrixmath.hpp"

#include "extern/glad.h"

using namespace mana;
using namespace mana::opengl;

FrameBuffer *OGLRenderAllocator::allocateFrameBuffer(int width, int height, TextureAttributes colorBufferProperties) {
    auto *ret = new OGLUserFrameBuffer(width, height);

    glGenFramebuffers(1, &ret->FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ret->FBO);

    glGenTextures(1, &ret->colorBuffer);
    glBindTexture(GL_TEXTURE_2D, ret->colorBuffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, OGLTypeConverter::convert(colorBufferProperties.texWrapping));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, OGLTypeConverter::convert(colorBufferProperties.texWrapping));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    OGLTypeConverter::convert(colorBufferProperties.texFilterMin));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    OGLTypeConverter::convert(colorBufferProperties.texFilterMag));

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 OGLTypeConverter::convert(colorBufferProperties.internalFormat),
                 width,
                 height,
                 0,
                 GL_RGB, GL_UNSIGNED_BYTE,
                 NULL);

    if (colorBufferProperties.generateMipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        OGLTypeConverter::convert(colorBufferProperties.mipmapFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        OGLTypeConverter::convert(colorBufferProperties.texFilterMag));
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ret->colorBuffer, 0);

    glGenRenderbuffers(1, &ret->renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, ret->renderBuffer);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ret->width, ret->height);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                              ret->renderBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Failed to setup framebuffer");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    checkGLError("OGLRenderAllocator::allocateFrameBuffer");

    return ret;
}

RenderTexture *OGLRenderAllocator::allocateTexture(int width, int height, TextureAttributes properties) {
    auto *ret = new OGLRenderTexture(width, height);

    glGenTextures(1, &ret->handle);
    glBindTexture(GL_TEXTURE_2D, ret->handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, OGLTypeConverter::convert(properties.texWrapping));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, OGLTypeConverter::convert(properties.texWrapping));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    OGLTypeConverter::convert(properties.texFilterMin));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    OGLTypeConverter::convert(properties.texFilterMag));

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 OGLTypeConverter::convert(properties.internalFormat),
                 width,
                 height,
                 0,
                 GL_RGB, GL_UNSIGNED_BYTE,
                 NULL);

    if (properties.generateMipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        OGLTypeConverter::convert(properties.mipmapFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        OGLTypeConverter::convert(properties.texFilterMag));
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    checkGLError("OGLRenderAllocator::allocateTexture");

    return ret;
}

RenderTexture *OGLRenderAllocator::allocateCubeMapTexture(int width, int height, TextureAttributes properties) {
    auto *ret = new OGLRenderTexture(width, height);

    ret->isCubeMap = true;

    glGenTextures(1, &ret->handle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ret->handle);

    for (unsigned int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0,
                     OGLTypeConverter::convert(properties.internalFormat),
                     width,
                     height,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, OGLTypeConverter::convert(properties.texWrapping));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, OGLTypeConverter::convert(properties.texWrapping));

    glTexParameteri(GL_TEXTURE_CUBE_MAP,
                    GL_TEXTURE_MIN_FILTER,
                    OGLTypeConverter::convert(properties.texFilterMin));
    glTexParameteri(GL_TEXTURE_CUBE_MAP,
                    GL_TEXTURE_MAG_FILTER,
                    OGLTypeConverter::convert(properties.texFilterMag));

    if (properties.generateMipmap) {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        glTexParameteri(GL_TEXTURE_CUBE_MAP,
                        GL_TEXTURE_MIN_FILTER,
                        OGLTypeConverter::convert(properties.mipmapFilter));
        glTexParameteri(GL_TEXTURE_CUBE_MAP,
                        GL_TEXTURE_MAG_FILTER,
                        OGLTypeConverter::convert(properties.texFilterMag));
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    checkGLError("OGLRenderAllocator::allocateTexture");

    return ret;
}

RenderMesh *OGLRenderAllocator::allocateMesh(const Mesh &mesh) {
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

    checkGLError("OGLRenderAllocator::allocateMesh");

    return ret;
}

RenderMesh *OGLRenderAllocator::allocateInstancedMesh(const Mesh &mesh, const std::vector<Transform> &offsets) {
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

    checkGLError("OGLRenderAllocator::allocateMeshInstanced");

    return ret;
}

ShaderProgram *OGLRenderAllocator::allocateShaderProgram(std::string vertexShader, std::string fragmentShader) {
    ShaderProgram *ret = new OGLShaderProgram(vertexShader, fragmentShader);
    checkGLError("OGLRenderAllocator::compileShaderProgram");
    return ret;
}
