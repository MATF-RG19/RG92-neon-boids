//
// Created by rentorious on 09/09/2020.
//
#include "Test.h"

namespace test {
    class TestTexture2D : public Test {
    public:
        TestTexture2D();

        ~TestTexture2D();

        void OnUpdate(float deltaTime) override;

        void OnRender() override;

        void OnImGuiRender() override;

    private:
    };


}
