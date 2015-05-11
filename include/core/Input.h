#pragma once
//
// Created by max on 2015-05-04.
//

#include <vector>
#include <Eigen/Geometry>

namespace Input {
    void Init();
    void Update();

    bool IsKeyDown(unsigned char key);
    Eigen::Vector2f GetInputAxis();
    Eigen::Vector2f GetMouseMove();
    bool IsInFocus();

    static void KeyboardFunc(unsigned char key, int x, int y);
    static void KeyboardUpFunc(unsigned char key, int x, int y);
    static void MotionFunc(int x, int y);
    static void TimerFunc(int val);
    static void MouseFunc(int button, int state, int x, int y);
    static std::vector<bool> isUp;

    static Eigen::Vector2f inputAxis;
    static Eigen::Vector2f mouseMove;
    static bool inFocus;
}

