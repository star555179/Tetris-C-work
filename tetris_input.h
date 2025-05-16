#pragma once
#include <windows.h>

// 输入处理类
class TetrisInput {
private:
    bool keyStates[256];
    bool prevKeyStates[256];

public:
    TetrisInput();
    void update();
    bool isKeyPressed(int keyCode);
    bool isKeyDown(int keyCode);
    bool isKeyReleased(int keyCode);
};

// 游戏操作枚举
enum class GameAction {
    None, MoveLeft, MoveRight, MoveDown, Rotate, Drop, Pause, Restart
};

// 输入处理器类
class InputHandler {
private:
    TetrisInput input;

public:
    GameAction getAction();
};    #pragma once
