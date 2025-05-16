#include "tetris_game.h"
#include <chrono>
#include <thread>

TetrisGame::TetrisGame() :
    playfield(10, 20),
    currentTetromino(),
    nextTetromino(),
    renderer(80, 30, 2),
    score(0),
    gameOver(false),
    isPaused(false),
    dropCounter(0),
    dropInterval(1000) {
}

void TetrisGame::initialize() {
    renderer.initialize();
    resetGame();
}

void TetrisGame::run() {
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (true) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        int deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
        lastTime = currentTime;

        // ��������
        GameAction action = inputHandler.getAction();
        processInput(action);

        if (!gameOver && !isPaused) {
            // ������Ϸ״̬
            dropCounter += deltaTime;
            if (dropCounter >= dropInterval) {
                update();
                dropCounter = 0;
            }
        }

        // ��Ⱦ��Ϸ
        render();

        // ������Ϸ�ٶ�
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void TetrisGame::processInput(GameAction action) {
    if (gameOver) {
        if (action == GameAction::Restart) {
            resetGame();
        }
        return;
    }

    if (action == GameAction::Pause) {
        isPaused = !isPaused;
        return;
    }

    if (isPaused) return;

    Tetromino tempTetromino = currentTetromino;

    switch (action) {
    case GameAction::MoveLeft:
        tempTetromino.move(-1, 0);
        if (playfield.isPositionValid(tempTetromino)) {
            currentTetromino = tempTetromino;
        }
        break;
    case GameAction::MoveRight:
        tempTetromino.move(1, 0);
        if (playfield.isPositionValid(tempTetromino)) {
            currentTetromino = tempTetromino;
        }
        break;
    case GameAction::MoveDown:
        tempTetromino.move(0, 1);
        if (playfield.isPositionValid(tempTetromino)) {
            currentTetromino = tempTetromino;
            score += 1; // ���Ƶ÷�
        }
        break;
    case GameAction::Rotate:
        tempTetromino.rotate();
        if (playfield.isPositionValid(tempTetromino)) {
            currentTetromino = tempTetromino;
        }
        break;
    case GameAction::Drop:
        while (playfield.isPositionValid(tempTetromino)) {
            currentTetromino = tempTetromino;
            tempTetromino.move(0, 1);
            score += 2; // Ӳ����÷ָ���
        }
        update();
        break;
    }
}

void TetrisGame::update() {
    // ��������
    Tetromino tempTetromino = currentTetromino;
    tempTetromino.move(0, 1);

    if (playfield.isPositionValid(tempTetromino)) {
        currentTetromino = tempTetromino;
    }
    else {
        // ���鲻�����ƣ��̶�������
        playfield.mergeTetromino(currentTetromino);

        // �����������
        int linesCleared = playfield.clearLines();
        if (linesCleared > 0) {
            score += linesCleared * 100; // ����е÷�
            // ���ŵ÷����ӣ������ٶȼӿ�
            dropInterval = std::max(200, 1000 - score / 10);
        }

        // �����Ϸ�Ƿ����
        if (playfield.isGameOver()) {
            gameOver = true;
            return;
        }

        // �����µķ���
        currentTetromino = nextTetromino;
        nextTetromino = Tetromino();
    }
}

void TetrisGame::render() {
    renderer.render(playfield, currentTetromino, nextTetromino, score, gameOver);
}

void TetrisGame::resetGame() {
    playfield = Playfield(10, 20);
    currentTetromino = Tetromino();
    nextTetromino = Tetromino();
    score = 0;
    gameOver = false;
    isPaused = false;
    dropCounter = 0;
    dropInterval = 1000;
}