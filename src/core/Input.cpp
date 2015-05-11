//
// Created by max on 2015-05-04.
//

#include <Input.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

using Eigen::Vector2f;

static const unsigned KEY_LEFT = 'a';
static const unsigned KEY_RIGHT = 'd';
static const unsigned KEY_UP = 'w';
static const unsigned KEY_DOWN = 's';
static const unsigned KEY_ESC = 0x1B;

static int x, y;

void Input::Init() {
    isUp.resize(255, false);
    glutMotionFunc(MotionFunc);
    glutPassiveMotionFunc(MotionFunc);
    glutKeyboardFunc(KeyboardFunc);
    glutKeyboardUpFunc(KeyboardUpFunc);
    glutMouseFunc(MouseFunc);
    glutTimerFunc(100, TimerFunc, 0);
}

bool Input::IsKeyDown(unsigned char key) {
    return isUp[key];
}

void Input::Update() {
    float toX = 0.0f, toY = 0.0f;
    if (IsKeyDown(KEY_LEFT)) {
        toX = -1.0f;
    }
    else if (IsKeyDown(KEY_RIGHT)) {
        toX = 1.0f;
    }
    if (IsKeyDown(KEY_UP)) {
        toY = 1.0f;
    }
    else if (IsKeyDown(KEY_DOWN)) {
        toY = -1.0f;
    }

    // TODO: Add some smoothing if wanted
    inputAxis << toX, toY;
}

Vector2f Input::GetInputAxis() {
    return inputAxis;
}

Vector2f Input::GetMouseMove() {
    return mouseMove;
}

bool Input::IsInFocus() {
    return inFocus;
}

void Input::KeyboardFunc(unsigned char key, int x, int y) {
    if (key == KEY_ESC) {
        glutSetCursor(GLUT_CURSOR_INHERIT);
        inFocus = false;
    }
    isUp[key] = true;
}

void Input::KeyboardUpFunc(unsigned char key, int x, int y) {
    isUp[key] = false;
}

void Input::MotionFunc(int newX, int newY) {
    x = newX;
    y = newY;
}

void Input::TimerFunc(int val) {
    if (inFocus) {
        int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
        int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

        mouseMove << x - centerX, y - centerY;

        // Set pointer to center position
        glutWarpPointer(centerX, centerY);
        x = centerX; y = centerY;
    }
    else {
        mouseMove << 0.0f, 0.0f;
    }
    glutTimerFunc(100, TimerFunc, 0);
}

void Input::MouseFunc(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        glutSetCursor(GLUT_CURSOR_NONE);
        inFocus = true;
    }
}
