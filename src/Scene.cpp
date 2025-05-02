#include "Scene.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Skybox.h"
#include "DebugLight.h"
#include "Water.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

Scene::Scene(const int width, const int height)
{
    WIDTH = width;
    HEIGHT = height;
}

void Scene::Run(GLFWwindow *window)
{
    std::vector<float> screenVertices = {// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                                         // positions   // texCoords
                                         -1.0f, 1.0f, 0.0f, 1.0f,
                                         -1.0f, -1.0f, 0.0f, 0.0f,
                                         1.0f, -1.0f, 1.0f, 0.0f,

                                         -1.0f, 1.0f, 0.0f, 1.0f,
                                         1.0f, -1.0f, 1.0f, 0.0f,
                                         1.0f, 1.0f, 1.0f, 1.0f};

    loadTexturesAndShaders();

    Framebuffer geometryBuffer;
    geometryBuffer.attachColorBufferHDR(WIDTH, HEIGHT);
    geometryBuffer.attachRenderbuffer(WIDTH, HEIGHT);

    Framebuffer reflectionBuffer;
    reflectionBuffer.attachColorBufferHDR(WIDTH, HEIGHT);
    reflectionBuffer.attachRenderbuffer(WIDTH, HEIGHT);

    Framebuffer refractionBuffer;
    refractionBuffer.attachColorBufferHDR(WIDTH, HEIGHT);
    refractionBuffer.attachRenderbuffer(WIDTH, HEIGHT);

    unsigned int screenVao, screenVbo;
    glGenVertexArrays(1, &screenVao);
    glGenBuffers(1, &screenVbo);
    glBindVertexArray(screenVao);
    glBindBuffer(GL_ARRAY_BUFFER, screenVbo);
    glBufferData(GL_ARRAY_BUFFER, screenVertices.size() * sizeof(float), screenVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Camera camera(window);
    camera.setCameraPosition(glm::vec3(0.0, 1.0, 3.0));

    std::vector<std::string> forestTextureLocations = {
        RESOURCES_PATH "textures/skybox/forest/posx.jpg",
        RESOURCES_PATH "textures/skybox/forest/negx.jpg",
        RESOURCES_PATH "textures/skybox/forest/posy.jpg",
        RESOURCES_PATH "textures/skybox/forest/negy.jpg",
        RESOURCES_PATH "textures/skybox/forest/posz.jpg",
        RESOURCES_PATH "textures/skybox/forest/negz.jpg"};

    SkyBox skybox(forestTextureLocations);

    Shader pbrShader = ResourceManager::GetShader("basic");

    Renderer renderer;
    Light light;
    light.lightAngle = 150.0f;

    PbrMaterial material = {
        ResourceManager::GetTexture("albedo"),
        ResourceManager::GetTexture("normal"),
        ResourceManager::GetTexture("metallic"),
        ResourceManager::GetTexture("roughness"),
        ResourceManager::GetTexture("ao"),
    };

    PbrBall object(material);
    object.setPosition(glm::vec3(0.0, 1.0, -3.0));

    DebugLight debugLight;
    Shader lightShader = ResourceManager::GetShader("light");

    Water water(glm::vec3(0.0, 0.0, 0.0f));
    Shader waterShader = ResourceManager::GetShader("water");
    waterShader.Bind();
    waterShader.setUniformInt("reflectionTexture", 0);
    waterShader.setUniformInt("refractionTexture", 1);
    waterShader.setUniformInt("dudvMap", 2);
    waterShader.setUniformInt("normalMap", 3);
    waterShader.UnBind();
    float waveStrength = 0.005f;
    float moveFactor = 0.0f;
    float waveSpeed = 0.06f;

    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    bool lockCursor = true;
    bool released = true;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Texture dudv = ResourceManager::GetTexture("dudv");
    Texture waterNormalMap = ResourceManager::GetTexture("wnormal");
    Shader screenShader = ResourceManager::GetShader("screen");

    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
        {
            released = true;
        }
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && released)
        {
            lockCursor = !lockCursor;
            released = false;
        }

        if (lockCursor)
        {
            camera.setDisableRotation(false);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            camera.setDisableRotation(true);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        camera.update(window, deltaTime);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render scene
        //
        // -----------------------------

        // Reflection pass
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        reflectionBuffer.Bind();

        glm::vec3 currentCamPos = camera.GetCameraPosition();
        float distance = 2 * (currentCamPos.y - water.GetHeight());

        camera.setCameraPosition(glm::vec3(currentCamPos.x, currentCamPos.y - distance, currentCamPos.z));
        camera.invertPitch();
        camera.updateViewMatrix(window);

        glm::vec4 clipPlane = glm::vec4(0, 1, 0, -water.GetHeight());
        renderer.drawScene(camera, skybox, object, light, clipPlane);

        camera.setCameraPosition(currentCamPos);
        camera.invertPitch();
        camera.updateViewMatrix(window);

        // Refraction pass
        refractionBuffer.Bind();
        clipPlane = glm::vec4(0, -1, 0, water.GetHeight());
        renderer.drawScene(camera, skybox, object, light, clipPlane);

        // Geometry Pass
        glDisable(GL_CLIP_DISTANCE0);
        geometryBuffer.Bind();
        skybox.Draw(renderer.getProjection(), glm::mat4(glm::mat3(camera.GetViewMatrix())));
        clipPlane = glm::vec4(0.0f);
        renderer.drawScene(camera, skybox, object, light, clipPlane);

        moveFactor += waveSpeed * deltaTime;
        moveFactor = fmod(moveFactor, 1);

        WaterData waterData = {
            reflectionBuffer.getTargetTexture(),
            refractionBuffer.getTargetTexture(),
            dudv.GetRendererID(),
            waterNormalMap.GetRendererID(),
            waveStrength,
            moveFactor};

        water.Draw(waterShader, waterData, camera, light, renderer.getProjection());

        Framebuffer::UnbindAll();

        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader.Bind();
        glBindVertexArray(screenVao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, geometryBuffer.getTargetTexture());
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glEnable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, 0);

        ImGui::Begin("ImGui window");

        // Light Settings
        ImGui::SliderFloat3("Light Color", &light.lightColor.x, 0.0f, 1.0f);
        ImGui::SliderFloat("Light Position", &light.lightAngle, 0.0f, 360.0f);
        ImGui::SliderFloat("Light Height", &light.lightHeight, -10.0f, 10.0f);
        ImGui::SliderFloat("Light Radius", &light.lightRadius, 0.0f, 25.0f);
        ImGui::SliderFloat("Light Scale", &light.lightScale, 1.0f, 200.0f);
        ImGui::Text("Light Pos = ( %f, %f, %f )", light.lightPos.x, light.lightPos.y, light.lightPos.z);
        ImGui::Text("Light Color = ( %f, %f, %f )", light.lightColor.x, light.lightColor.y, light.lightColor.z);
        ImGui::SliderFloat("Wave strength", &waveStrength, 0.0, 0.05);
        ImGui::SliderFloat("Wave speed", &waveSpeed, 0.0, 0.1);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Scene::loadTexturesAndShaders()
{
    ResourceManager::LoadShader(RESOURCES_PATH "shaders/basic.vs", RESOURCES_PATH "shaders/pbr.fs", "basic");
    ResourceManager::LoadShader(RESOURCES_PATH "shaders/skybox.vs", RESOURCES_PATH "shaders/skybox.fs", "skybox");
    ResourceManager::LoadShader(RESOURCES_PATH "shaders/screen.vs", RESOURCES_PATH "shaders/screen.fs", "screen");
    ResourceManager::LoadShader(RESOURCES_PATH "shaders/basic.vs", RESOURCES_PATH "shaders/debug_light.fs", "light");
    ResourceManager::LoadShader(RESOURCES_PATH "shaders/water.vs", RESOURCES_PATH "shaders/water.fs", "water");

    ResourceManager::LoadTexture(RESOURCES_PATH "textures/Carbon/albedo.png", "albedo");
    ResourceManager::LoadTexture(RESOURCES_PATH "textures/Carbon/metallic.png", "metallic");
    ResourceManager::LoadTexture(RESOURCES_PATH "textures/Carbon/normal.png", "normal");
    ResourceManager::LoadTexture(RESOURCES_PATH "textures/Carbon/roughness.png", "roughness");
    ResourceManager::LoadTexture(RESOURCES_PATH "textures/Carbon/ao.png", "ao");
    ResourceManager::LoadTexture(RESOURCES_PATH "textures/dudv.png", "dudv");
    ResourceManager::LoadTexture(RESOURCES_PATH "textures/water-normal.png", "wnormal");

    Shader screenShader = ResourceManager::GetShader("screen");
}
