#include <raylib.h>
#include "chunks.h"
#include "utils.h"

Chunk* chunks = NULL;
int numChunksX = 3;
int numChunksY = 3;
Camera2D camera = { 0 };

int main() {
    InitWindow(W_Width, W_Height, "Chunk Generator with Borders and Camera");

    camera.target = (Vector2){ 0.0f, 0.0f };
    camera.offset = (Vector2){ 0.0f, 0.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    InitChunks(); 

    while (!WindowShouldClose()) {
        UpdateGameCamera();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);

        DrawChunks(); 

        EndMode2D();

        EndDrawing();
    }

    DestructChunks();

    CloseWindow();
    return 0;
}
