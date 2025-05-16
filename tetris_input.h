#pragma once
#include <windows.h>

// ���봦����
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

// ��Ϸ����ö��
enum class GameAction {
    None, MoveLeft, MoveRight, MoveDown, Rotate, Drop, Pause, Restart
};

// ���봦������
class InputHandler {
private:
    TetrisInput input;

public:
    GameAction getAction();
};    #pragma once
