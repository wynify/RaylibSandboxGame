#include "chunks.h"
#include <stdlib.h>
#include <stdio.h>

extern Chunk* chunks;
extern int numChunksX;
extern int numChunksY;
extern Camera2D camera;

// Инициализация чанка
void InitChunk(Chunk* chunk, int chunkWidth, int chunkHeight, Vector2 position) {
    chunk->chunkWidth = chunkWidth;
    chunk->chunkHeight = chunkHeight;
    chunk->chunkPosition = position;
    chunk->cells = (Cell*)malloc(chunkWidth * chunkHeight * sizeof(Cell));

    for (int y = 0; y < chunkHeight; y++) {
        for (int x = 0; x < chunkWidth; x++) {
            chunk->cells[y * chunkWidth + x].position = (Vector2){ x * CellSize, y * CellSize };
            chunk->cells[y * chunkWidth + x].type = EMPTY;
            chunk->cells[y * chunkWidth + x].color = DARKPURPLE;
        }
    }
}

// Генерация чанка
void GenerateChunk(Chunk* chunk) {
    for (int y = 0; y < chunk->chunkHeight; y++) {
        for (int x = 0; x < chunk->chunkWidth; x++) {
            float noise = (float)(rand() % 100) / 100.0f;

            if (noise > 0.6f) {
                chunk->cells[y * chunk->chunkWidth + x].type = FILLED;
                chunk->cells[y * chunk->chunkWidth + x].color = GREEN;
            }
            else if (noise < 0.4f) {
                chunk->cells[y * chunk->chunkWidth + x].type = BLOCKED;
                chunk->cells[y * chunk->chunkWidth + x].color = RED;
            }
            else {
                chunk->cells[y * chunk->chunkWidth + x].type = EMPTY;
                chunk->cells[y * chunk->chunkWidth + x].color = DARKGRAY;
            }
        }
    }
}

// Рисование чанка
void DrawChunk(Chunk* chunk) {
    for (int y = 0; y < chunk->chunkHeight; y++) {
        for (int x = 0; x < chunk->chunkWidth; x++) {
            Cell cell = chunk->cells[y * chunk->chunkWidth + x];
            Vector2 drawPosition = { chunk->chunkPosition.x + cell.position.x, chunk->chunkPosition.y + cell.position.y };
            Vector2 drawSize = { CellSize, CellSize };
            DrawRectangleV(drawPosition, drawSize, cell.color);
            DrawRectangleLines(drawPosition.x, drawPosition.y, CellSize, CellSize, BLACK);
        }
    }

    Vector2 chunkPosition = chunk->chunkPosition;
    float chunkWidthInPixels = chunk->chunkWidth * CellSize;
    float chunkHeightInPixels = chunk->chunkHeight * CellSize;
    DrawRectangleLines(chunkPosition.x, chunkPosition.y, chunkWidthInPixels, chunkHeightInPixels, PURPLE);
}

// Инициализация чанков
void InitChunks() {
    chunks = (Chunk*)malloc(numChunksX * numChunksY * sizeof(Chunk));
    for (int y = 0; y < numChunksY; y++) {
        for (int x = 0; x < numChunksX; x++) {
            Vector2 position = { (float)(x * CHUNK_SIZE * CellSize), (float)(y * CHUNK_SIZE * CellSize) };
            InitChunk(&chunks[y * numChunksX + x], CHUNK_SIZE, CHUNK_SIZE, position);
            GenerateChunk(&chunks[y * numChunksX + x]);
        }
    }
}

// Отображение чанков
void DrawChunks() {
    // Рисуем только те чанки, которые попадают в область видимости
    for (int y = 0; y < VISIBLE_CHUNKS_Y; y++) {
        for (int x = 0; x < VISIBLE_CHUNKS_X; x++) {
            int chunkIndex = (int)(camera.offset.y / (CellSize * CHUNK_SIZE) + y) * numChunksX + (int)(camera.offset.x / (CellSize * CHUNK_SIZE) + x);
            if (chunkIndex >= 0 && chunkIndex < numChunksX * numChunksY) {
                DrawChunk(&chunks[chunkIndex]);
            }
        }
    }
}

// Освобождение памяти
void DestructChunks() {
    if (chunks != NULL) {
        for (int i = 0; i < numChunksX * numChunksY; i++) {
            free(chunks[i].cells);
        }
        free(chunks);
    }
}

// Обновление камеры
void UpdateGameCamera() {
    // Перемещение камеры с помощью средней кнопки мыши
    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        Vector2 mouseDelta = GetMouseDelta();
        camera.offset.x += mouseDelta.x;
        camera.offset.y += mouseDelta.y;
    }

    // Зум с помощью колёсика мыши
    float wheelMove = GetMouseWheelMove();  // Получаем изменение колёсика мыши
    camera.zoom += wheelMove * 0.1f;  // Изменяем зум камеры, коэффициент 0.1f для умеренного зума

    // Ограничим зум, чтобы не было слишком большого или слишком маленького масштаба
    if (camera.zoom < 0.1f) {
        camera.zoom = 0.1f;  // Минимальный зум
    }
    else if (camera.zoom > 3.0f) {
        camera.zoom = 3.0f;  // Максимальный зум
    }
}
