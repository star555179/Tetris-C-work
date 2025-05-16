#include "tetris_input.h"

TetrisInput::TetrisInput() {
    for (int i = 0; i < 256; ++i) {
        keyStates[i] = false;
        prevKeyStates[i] = false;
    }
}

void TetrisInput::update() {
    for (int i = 0; i < 256; ++i) {
        prevKeyStates[i] = keyStates[i];
        keyStates[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
    }
}

bool TetrisInput::isKeyPressed(int keyCode) {
    return keyStates[keyCode] && !prevKeyStates[keyCode];
}

bool TetrisInput::isKeyDown(int keyCode) {
    return keyStates[keyCode];
}

bool TetrisInput::isKeyReleased(int keyCode) {
    return !keyStates[keyCode] && prevKeyStates[keyCode];
}

GameAction InputHandler::getAction() {
    input.update();

    if (input.isKeyPressed(VK_LEFT)) return GameAction::MoveLeft;
    if (input.isKeyPressed(VK_RIGHT)) return GameAction::MoveRight;
    if (input.isKeyDown(VK_DOWN)) return GameAction::MoveDown;
    if (input.isKeyPressed(VK_UP)) return GameAction::Rotate;
    if (input.isKeyPressed(VK_SPACE)) return GameAction::Drop;
    if (input.isKeyPressed('P')) return GameAction::Pause;
    if (input.isKeyPressed('R')) return GameAction::Restart;

    return GameAction::None;
}