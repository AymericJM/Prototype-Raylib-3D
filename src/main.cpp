/*
** raylib_test
** File description:
** main
*/

#include "raylib.h"
#include "rcamera.h"

#include <stddef.h>
#include <math.h>

#define MAX_COLUMNS 20

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera first person");

    // Define the camera to look into our 3d world (position, target, up vector)
    Camera camera = { 0 };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 80.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    int cameraMode = CAMERA_CUSTOM;

    Vector2 cameraAngle = { 0.0f, 0.0f }; // x = horizontal, y = vertical
    float cameraDistance = 5.0f;          // Distance au robot


    // load gltf model
    Model model = LoadModel("assets/robot.glb");
    int animsCount = 0;
    ModelAnimation *modelAnimations = LoadModelAnimations("assets/robot.glb", &animsCount);

    unsigned int animIndex = 2; // 4 for IDLE animation
    unsigned int animCurrentFrame = 0;
    ModelAnimation anim;

    Vector3 playerPos = {0.0f, 0.0f, 0.0f};
    float playerSpeed = 0.1f;
    float playerRotation = 0.0f;


    BoundingBox walls[4];
    walls[0] = (BoundingBox){ {-16.5f, 0.0f, -16.0f}, {-15.5f, 5.0f, 16.0f} }; // Mur bleu
    walls[1] = (BoundingBox){ {15.5f, 0.0f, -16.0f}, {16.5f, 5.0f, 16.0f} };  // Mur vert
    walls[2] = (BoundingBox){ {-16.0f, 0.0f, 15.5f}, {16.0f, 5.0f, 16.5f} };  // Mur jaune


    Vector3 platPos = {2.0f, 1.8f, 2.0f};
    Vector3 platSize = {4.0f, 0.6f, 4.0f};

    walls[3] = (BoundingBox){
        { platPos.x - platSize.x/2, platPos.y - platSize.y/2, platPos.z - platSize.z/2 },
        { platPos.x + platSize.x/2, platPos.y + platSize.y/2, platPos.z + platSize.z/2 }
    };       // Platform (un peu plus épaisse)


    // JUMP MECANIC
    float playerVelocityY = 0.0f;
    bool isGrounded = true;
    const float gravity = -0.015f;
    const float jumpForce = 0.3f;


    DisableCursor();                    // Limit cursor to relative movement inside the window

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // UPDATE
        // get mouse movements
        Vector2 mouseDelta = GetMouseDelta();

        cameraAngle.x -= mouseDelta.x * 0.003f;
        cameraAngle.y -= mouseDelta.y * 0.003f;

        if (cameraAngle.y > 1.2f) cameraAngle.y = 1.2f;
        if (cameraAngle.y < 0.3f) cameraAngle.y = 0.3f;

        cameraDistance -= GetMouseWheelMove() * 0.5f;
        if (cameraDistance < 3.0f) cameraDistance = 3.0f;
        if (cameraDistance > 10.0f) cameraDistance = 10.0f;

        // handling player inputs && movements
        Vector3 movement = {0.0f, 0.0f, 0.0f};
        bool isMoving = false;
        bool isRunning = IsKeyDown(KEY_LEFT_SHIFT);

        if (IsKeyDown(KEY_W)) { movement.x += -sinf(cameraAngle.x); movement.z += -cosf(cameraAngle.x); isMoving = true; }
        if (IsKeyDown(KEY_S)) { movement.x +=  sinf(cameraAngle.x); movement.z +=  cosf(cameraAngle.x); isMoving = true; }
        if (IsKeyDown(KEY_A)) { movement.x +=  sinf(cameraAngle.x - PI/2.0f); movement.z +=  cosf(cameraAngle.x - PI/2.0f); isMoving = true; }
        if (IsKeyDown(KEY_D)) { movement.x +=  sinf(cameraAngle.x + PI/2.0f); movement.z +=  cosf(cameraAngle.x + PI/2.0f); isMoving = true; }

        // JUMP HANDLING
        if (isGrounded && IsKeyPressed(KEY_SPACE)) {
            playerVelocityY = jumpForce;
            isGrounded = false;
        }

        // Apply gravity and Y movement
        playerVelocityY += gravity;
        playerPos.y += playerVelocityY;

        // GROUND & PLATFORM COLLISIONS (Vertical)
        isGrounded = false;
        
        // Floor check
        if (playerPos.y <= 0.0f) {
            playerPos.y = 0.0f;
            playerVelocityY = 0.0f;
            isGrounded = true;
        }

        // Box collision check (Vertical)
        BoundingBox playerBox = {
            { playerPos.x - 0.4f, playerPos.y, playerPos.z - 0.4f },
            { playerPos.x + 0.4f, playerPos.y + 2.0f, playerPos.z + 0.4f }
        };

        for (int i = 0; i < 4; i++) {
            if (CheckCollisionBoxes(playerBox, walls[i])) {
                // Si on est en train de tomber et que nos pieds touchent le haut de la box
                if (playerVelocityY < 0.0f && playerPos.y >= walls[i].max.y - 0.5f) {
                    playerPos.y = walls[i].max.y;
                    playerVelocityY = 0.0f;
                    isGrounded = true;
                }
            }
        }

        // HORIZONTAL MOVEMENT WITH COLLISIONS
        if (isMoving) {
            float magnitude = sqrtf(movement.x * movement.x + movement.z * movement.z);
            float currentSpeed = isRunning ? 0.2f : 0.1f;
            movement.x = (movement.x / magnitude) * currentSpeed;
            movement.z = (movement.z / magnitude) * currentSpeed;

            // TEST X
            Vector3 nextPosX = playerPos;
            nextPosX.x += movement.x;
            BoundingBox boxX = {
                { nextPosX.x - 0.4f, playerPos.y, playerPos.z - 0.4f },
                { nextPosX.x + 0.4f, playerPos.y + 2.0f, playerPos.z + 0.4f }
            };
            bool colX = false;
            for(int i = 0; i < 4; i++) 
                if (CheckCollisionBoxes(boxX, walls[i]) && playerPos.y < walls[i].max.y - 0.2f) colX = true;
            if (!colX) playerPos.x = nextPosX.x;

            // TEST Z
            Vector3 nextPosZ = playerPos;
            nextPosZ.z += movement.z;
            BoundingBox boxZ = {
                { playerPos.x - 0.4f, playerPos.y, nextPosZ.z - 0.4f },
                { playerPos.x + 0.4f, playerPos.y + 2.0f, nextPosZ.z + 0.4f }
            };
            bool colZ = false;
            for(int i = 0; i < 4; i++) 
                if (CheckCollisionBoxes(boxZ, walls[i]) && playerPos.y < walls[i].max.y - 0.2f) colZ = true;
            if (!colZ) playerPos.z = nextPosZ.z;

            playerRotation = atan2f(movement.x, movement.z) * RAD2DEG;
        }

        // ANIMATION SELECTION
        if (!isGrounded) animIndex = 3; 
        else if (isMoving) animIndex = isRunning ? 6 : 10;
        else animIndex = 2;


        // CAMERA
        camera.target = playerPos;
        float eyeHeight = 1.5f;
        
        camera.position.x = playerPos.x + cameraDistance * cosf(cameraAngle.y) * sinf(cameraAngle.x);
        camera.position.y = (playerPos.y + eyeHeight) + cameraDistance * sinf(cameraAngle.y);
        camera.position.z = playerPos.z + cameraDistance * cosf(cameraAngle.y) * cosf(cameraAngle.x);
        
        camera.target.y += eyeHeight;

        UpdateCamera(&camera, cameraMode);

        // Update model Animation
        if (modelAnimations != NULL) {
            anim = modelAnimations[animIndex];
            animCurrentFrame = (animCurrentFrame + 1) % anim.frameCount;
            UpdateModelAnimation(model, anim, animCurrentFrame);
        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);

                DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 32.0f, 32.0f }, LIGHTGRAY); // Draw ground
                DrawCube((Vector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
                DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
                DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall

                DrawCube(platPos, platSize.x, platSize.y, platSize.z, RED);

                DrawModelEx(model, playerPos, (Vector3){ 0, 1, 0 }, playerRotation, (Vector3){ 0.5f, 0.5f, 0.5f }, WHITE);

            EndMode3D();

            DrawText(TextFormat("Animation: %s", anim.name), 10, GetScreenHeight() - 20, 10, DARKGRAY);
            DrawText(TextFormat("Grounded: %s", isGrounded ? "YES" : "NO"), 10, GetScreenHeight() - 40, 10, DARKGRAY);

        EndDrawing();
    }

    // De-Initialization
    UnloadModelAnimations(modelAnimations, animsCount);
    UnloadModel(model);
    CloseWindow();

    return 0;
}