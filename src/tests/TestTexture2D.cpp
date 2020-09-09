#include "TestTexture2D.h"
#include "../Renderer.h"
#include "imgui/imgui.h"

namespace test {
    TestTexture2D::TestTexture2D() {
    }

    TestTexture2D::~TestTexture2D() noexcept {}

    void TestTexture2D::OnUpdate(float deltaTime) {}

    void TestTexture2D::OnRender() {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void TestTexture2D::OnImGuiRender() {
        ImGui::ColorEdit4("Clear Color", m_ClearColor);
    }
}