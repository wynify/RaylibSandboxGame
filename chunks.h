#ifndef CHUNK_H
#define CHUNK_H

#include <raylib.h>

#define CellSize 25
#define CHUNK_SIZE 100
#define VISIBLE_CHUNKS_X 50 // Количество чанков по оси X, которые могут быть видимыми
#define VISIBLE_CHUNKS_Y 50 // Количество чанков по оси Y, которые могут быть видимыми

typedef enum { EMPTY, FILLED, BLOCKED } CellType;

typedef struct {
    Vector2 position;
    CellType type;
    Color color;
} Cell;

typedef struct {
    Cell* cells;
    int chunkWidth;
    int chunkHeight;
    Vector2 chunkPosition;
} Chunk;

// Глобальная переменная для камеры
extern Camera2D camera;

// Функции для работы с чанками
void InitChunk(Chunk* chunk, int chunkWidth, int chunkHeight, Vector2 position);
void GenerateChunk(Chunk* chunk);
void DrawChunk(Chunk* chunk);
void InitChunks(void);
void DrawChunks(void);
void DestructChunks(void);
void UpdateCamera(void);  // Функция обновления камеры

#endif // CHUNK_H
