#ifndef MANA_SAMPLE0_HPP
#define MANA_SAMPLE0_HPP

#include "game.hpp"

#include "shadersource.hpp"

class Sample0 : public Game {
public:
    void onFramebufferResize(Vec2i size) override {
        camera.aspectRatio = (float) size.x / (float) size.y;
    }

protected:
    void update(float deltaTime, Window &window, Renderer &ren, RenderAllocator &alloc, Input &input) override {
        Mouse mouse = input.getMouse();
        Vec2d mouseDiff = mouse.position - mouseLastFrame.position;
        mouseLastFrame = mouse;

        double rotationSpeed = cameraRotationSpeed * deltaTime;

        if (mouse.leftButtonDown && (mouseDiff.x != 0 || mouseDiff.y != 0)) {
            cube->transform.rotation.x += mouseDiff.y * rotationSpeed;
            cube->transform.rotation.y += mouseDiff.x * rotationSpeed;
        }

        if (input.getKeyDown(KEY_UP)) {
            camera.transform.rotation.x += rotationSpeed;
        } else if (input.getKeyDown(KEY_DOWN)) {
            camera.transform.rotation.x -= rotationSpeed;
        }

        if (input.getKeyDown(KEY_LEFT)) {
            camera.transform.rotation.y -= rotationSpeed;
        } else if (input.getKeyDown(KEY_RIGHT)) {
            camera.transform.rotation.y += rotationSpeed;
        }

        Vec3f inputMovement = Vec3f(0);
        if (input.getKeyDown(KEY_A)) {
            inputMovement.x = -1;
        } else if (input.getKeyDown(KEY_D)) {
            inputMovement.x = 1;
        }

        if (input.getKeyDown(KEY_W)) {
            inputMovement.z = -1;
        } else if (input.getKeyDown(KEY_S)) {
            inputMovement.z = 1;
        }

        if (input.getKeyDown(KEY_E)) {
            inputMovement.y = 1;
        } else if (input.getKeyDown(KEY_Q)) {
            inputMovement.y = -1;
        }

        //Translate direction vectors with desired length into world space and add them to the current camera position.
        Mat4f view = camera.view();

        view = MatrixMath::inverse(view);

        float movementSpeed = cameraMovementSpeed * deltaTime;

        Vec4f left = (view) * Vec4f(inputMovement.x * movementSpeed, 0, 0, 0);
        Vec4f up = (view) * Vec4f(0, inputMovement.y * movementSpeed, 0, 0);
        Vec4f forward = (view) * Vec4f(0, 0, inputMovement.z * movementSpeed, 0);

        camera.transform.position += toVec3(left + up + forward);

        Vec3f lightPos = scene.pointLights.at(0).transform.position;

        Mat4f rot = MatrixMath::rotate(Vec3f(0, rotationSpeed, 0));

        lightPos = toVec3(rot * toVec4(lightPos, 1));

        if (incrementLight) {
            if (lightPos.y < 2) {
                lightPos.y += lightMovementSpeed * deltaTime;
            } else {
                lightPos.y = 2;
                incrementLight = false;
            }
        } else {
            if (lightPos.y > 0.5f) {
                lightPos.y -= lightMovementSpeed * deltaTime;
            } else {
                lightPos.y = 0.5f;
                incrementLight = true;
            }
        }

        scene.pointLights.at(0).transform.position = lightPos;
        sphere->transform.position = lightPos;

        sphere->transform.rotation += Vec3f(rotationSpeed, rotationSpeed / 2, rotationSpeed);

        skybox->transform.position = camera.transform.position;

        forward = view * Vec4f(0, 0, -1, 0);

        scene.spotLights.at(0).direction = toVec3(forward);
        scene.spotLights.at(0).transform.position = camera.transform.position;

        ren.render(window.getFrameBuffer(), scene, clearColor, true, true, true, false);

        window.swapBuffers();
        window.update();
    }

    void loadScene(RenderAllocator &alloc) override {
        Vec3f lightPos = Vec3f(2, 1, 0);

        PointLight light;
        light.transform.position = lightPos;
        light.constant = 1.0f;
        light.linear = 1.0f;
        light.quadratic = 1.0f;
        light.ambient = Vec3f(0);
        scene.pointLights.emplace_back(light);

        Vec3f lightPos0 = Vec3f(-4, 0.73, 0);
        SpotLight light0 = SpotLight();
        light0.transform.position = lightPos0;
        light0.cutOff = 15;
        light0.outerCutOff = 25;
        scene.spotLights.emplace_back(light0);

        Mesh planeMesh = MeshLoader::load("./assets/plane.obj");
        Mesh curveCubeMesh = MeshLoader::load("./assets/curvecube.obj");
        Mesh sphereMesh = MeshLoader::load("./assets/icosphere.obj");
        Mesh cubeMesh = MeshLoader::load("./assets/cube.obj");

        ShaderProgram *skyboxShader = alloc.allocateShaderProgram(skyboxVertexShader, skyboxFragmentShader);
        objects.emplace_back(skyboxShader);

        skyboxShader->setInt("skybox", 0);

        ShaderProgram *shader = alloc.allocateShaderProgram(vertexShader, fragmentShader);
        objects.emplace_back(shader);

        shader->setInt("diffuse", 0);
        shader->setInt("specular", 1);

        ShaderProgram *lightShader = alloc.allocateShaderProgram(vertexShader, lightFragmentShader);
        objects.emplace_back(lightShader);

        auto colorMapImage = ImageLoader::load("./assets/colormap.png");
        auto colorMapTexture = alloc.allocateTexture(colorMapImage.getWidth(), colorMapImage.getHeight());

        colorMapTexture->upload(colorMapImage, RGB);

        auto skyboxImage = ImageLoader::load("./assets/deepbluespace-skybox_maintex.png");
        auto skyboxTexture = alloc.allocateCubeMapTexture(2048, 2048);

        skyboxTexture->upload(RenderTexture::RIGHT,
                              skyboxImage.slice(Recti(Vec2i(0, 0), Vec2i(2048, 2048))));

        skyboxTexture->upload(RenderTexture::LEFT,
                              skyboxImage.slice(Recti(Vec2i(2048 * 1, 0), Vec2i(2048, 2048))));

        skyboxTexture->upload(RenderTexture::TOP,
                              skyboxImage.slice(Recti(Vec2i(2048 * 2, 0), Vec2i(2048, 2048))));

        skyboxTexture->upload(RenderTexture::BOTTOM,
                              skyboxImage.slice(Recti(Vec2i(2048 * 3, 0), Vec2i(2048, 2048))));

        skyboxTexture->upload(RenderTexture::FRONT,
                              skyboxImage.slice(Recti(Vec2i(2048 * 4, 0), Vec2i(2048, 2048))));

        skyboxTexture->upload(RenderTexture::BACK,
                              skyboxImage.slice(Recti(Vec2i(2048 * 5, 0), Vec2i(2048, 2048))));

        objects.emplace_back(colorMapTexture);
        objects.emplace_back(skyboxTexture);

        RenderMesh *cubePtr = alloc.allocateMesh(cubeMesh);
        RenderMesh *curveCubePtr = alloc.allocateMesh(curveCubeMesh);
        RenderMesh *planePtr = alloc.allocateMesh(planeMesh);
        RenderMesh *spherePtr = alloc.allocateInstancedMesh(sphereMesh, {Transform({0, 0, 0}, {}, {1, 1, 1}),
                                                                         Transform({0, 1, 0}, {}, {1, 1, 1}),
                                                                         Transform({0, -1, 0}, {}, {1, 1, 1}),
                                                                         Transform({1, 0, 0}, {}, {1, 1, 1}),
                                                                         Transform({-1, 0, 0}, {}, {1, 1, 1}),
                                                                         Transform({0, 0, 1}, {}, {1, 1, 1}),
                                                                         Transform({0, 0, -1}, {}, {1, 1, 1})});

        objects.emplace_back(cubePtr);
        objects.emplace_back(curveCubePtr);
        objects.emplace_back(planePtr);
        objects.emplace_back(spherePtr);

        RenderCommand command;
        command.enableDepthTest = true;

        command.meshObjects.emplace_back(curveCubePtr);

        command.shader = shader;

        command.textureObjects.emplace_back(colorMapTexture);

        command.transform.position = Vec3f(0, 1, 0);
        command.transform.rotation = Vec3f(0);
        command.transform.scale = Vec3f(1);

        scene.commands.emplace(scene.commands.end(), command);

        command = RenderCommand();
        command.enableDepthTest = true;

        command.meshObjects.emplace_back(spherePtr);

        command.shader = shader;

        command.textureObjects.emplace_back(colorMapTexture);

        command.transform.position = lightPos;
        command.transform.rotation = Vec3f(0);
        command.transform.scale = Vec3f(0.1f);

        scene.commands.emplace(scene.commands.end(), command);

        command = RenderCommand();
        command.enableDepthTest = true;

        command.meshObjects.emplace_back(planePtr);

        command.shader = shader;

        command.textureObjects.emplace_back(colorMapTexture);

        command.transform.position = Vec3f(0);
        command.transform.rotation = Vec3f(0);
        command.transform.scale = Vec3f(10.0f);

        scene.commands.emplace(scene.commands.end(), command);

        command = RenderCommand();
        command.enableDepthTest = false;
        command.meshObjects.emplace_back(cubePtr);

        command.shader = skyboxShader;

        command.textureObjects.emplace_back(skyboxTexture);

        scene.commands.emplace(scene.commands.begin(), command);

        scene.camera = &camera;

        camera.transform.position = Vec3f(0, 3, 3);
        camera.transform.rotation = Vec3f(1, 0, 0);

        skybox = &*(scene.commands.begin());
        sphere = &*(scene.commands.begin() + 2);
        cube = &*(scene.commands.begin() + 1);
    }

    void destroyScene() override {
        for (auto *p : objects)
            delete p;
    }

private:
    std::vector<RenderObject *> objects;

    float cameraRotationSpeed = 45.0f;
    float cameraMovementSpeed = 5.0f;

    bool incrementLight = false;

    float lightMovementSpeed = 1.0f;

    Mouse mouseLastFrame;

    ColorRGBA clearColor = ColorRGBA(30, 30, 30, 255);

    PerspectiveCamera camera;

    RenderCommand *skybox;
    RenderCommand *sphere;
    RenderCommand *cube;
};

#endif //MANA_SAMPLE0_HPP
