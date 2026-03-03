/*
** raylib_test
** File description:
** main
*/

#include "raylib.h"
#include "rcamera.h"
#include "components/Components.hpp"

#include <stddef.h>
#include <math.h>
#include <entt/entt.hpp>

#define MAX_COLUMNS 20

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

void ControlPlayerSystem(entt::registry& registry, float cameraAngleX);
void PhysicsSystem(entt::registry& registry);
void AnimationSystem(entt::registry& registry);
void RenderSystem(entt::registry& registry);
void CameraSystem(Camera& camera, Vector3 targetPos, Vector2& cameraAngle, float& cameraDistance);


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Prototype Raylib 3D");
    entt::registry registry;

    // Init player
    auto player = registry.create();
    registry.emplace<PlayerTag>(player);
    registry.emplace<TransformComponent>(player, (Vector3){ 5, 0, 0 }, 0.0f, 0.5f); //FIXME: change the start position
    registry.emplace<Physics>(player, (Vector3){ 0, 0, 0}, true);

    Model robotModel = LoadModel("assets/robot.glb");
    int animsCount = 0;
    ModelAnimation *modelAnimations = LoadModelAnimations("assets/robot.glb", &animsCount);
    registry.emplace<ModelRenderer>(player, robotModel, modelAnimations, animsCount, 2, 0);

    // Init Obstacles
    auto createObstacle = [&](Vector3 pos, Vector3 size) {
        auto entity = registry.create();
        BoundingBox box = {
            { pos.x - size.x/2, pos.y - size.y/2, pos.z - size.z/2 },
            { pos.x + size.x/2, pos.y + size.y/2, pos.z + size.z/2 }
        };
        registry.emplace<ObstacleTag>(entity, box);
        return entity;
    };

    // Walls
    createObstacle({-16.0f, 2.5f, 0.0f}, {1.0f, 5.0f, 32.0f}); // blue wall
    createObstacle({16.0f, 2.5f, 0.0f}, {1.0f, 5.0f, 32.0f});  // green wall
    createObstacle({0.0f, 2.5f, 16.0f}, {32.0f, 5.0f, 1.0f}); // yellow wall

    // floor TODO: temporary, need to update the map
    createObstacle({ 0.0f, -0.5f, 0.0f }, { 100.0f, 1.0f, 100.0f });

    // Red platform
    Vector3 platPos = {2.0f, 1.8f, 2.0f};
    Vector3 platSize = {4.0f, 0.6f, 4.0f};
    createObstacle(platPos, platSize);

    createObstacle({2.0f, 4.0f, 6.0f}, {4.0f, 0.6f, 4.0f}); // yellow wall


    // --- CAMERA ---
    Camera camera = { 0 };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 80.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Vector2 cameraAngle = { 0.0f, 0.0f }; // x = horizontal, y = vertical
    float cameraDistance = 5.0f;          // Distance au robot

    int cameraMode = CAMERA_CUSTOM;

    DisableCursor();
    SetTargetFPS(60);

    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // --- UPDATE SYSTEMS ---

        // player control
        ControlPlayerSystem(registry, cameraAngle.x);

        // physics -> velocity, gravity, collisions
        PhysicsSystem(registry);

        // the camera follow the player
        auto& playerTransform = registry.get<TransformComponent>(player);
        CameraSystem(camera, playerTransform.pos, cameraAngle, cameraDistance);

        // logic animation TODO: set into a StateSystem
        auto& playerPhys = registry.get<Physics>(player);
        auto& playerRender = registry.get<ModelRenderer>(player);

        if (!playerPhys.isGrounded)
            playerRender.currentAnimIndex = 3; // Jump
        else if (playerPhys.velocity.x != 0 || playerPhys.velocity.z != 0)
            playerRender.currentAnimIndex = IsKeyDown(KEY_LEFT_SHIFT) ? 6 : 10; // 6 for Run anim, 10 for walk TODO: set defines for animations
        else
            playerRender.currentAnimIndex = 2; // idle

        AnimationSystem(registry);

        // --- DRAW ---
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);

                DrawPlane((Vector3){ 0.0f, 0.0f, 0.0f }, (Vector2){ 100.0f, 100.0f }, LIGHTGRAY); // Draw ground
                DrawCube((Vector3){ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
                DrawCube((Vector3){ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
                DrawCube((Vector3){ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall
                DrawCube(platPos, platSize.x, platSize.y, platSize.z, RED);

                DrawCube({2.0f, 4.0f, 6.0f}, 4.0f, 0.6f, 4.0f, PINK);

                RenderSystem(registry);

            EndMode3D();

            DrawFPS(10, 10);
            DrawText(TextFormat("Animation: %s", playerRender.modelAnim->name), 10, GetScreenHeight() - 20, 10, DARKGRAY);
            DrawText(TextFormat("Grounded: %s", playerPhys.isGrounded ? "YES" : "NO"), 10, GetScreenHeight() - 40, 10, DARKGRAY);
            DrawText("ECS Engine - EnTT + Raylib", 10, SCREEN_HEIGHT - 60, 10, DARKGRAY);

        EndDrawing();
    }

    // De-Initialization
    UnloadModelAnimations(modelAnimations, animsCount);
    UnloadModel(robotModel);
    CloseWindow();

    return 0;
}