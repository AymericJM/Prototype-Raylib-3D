/*
** Prototype-Raylib-3D
** File description:
** ControlPlayerSystem
*/

#include "entt/entt.hpp"
#include "components/Components.hpp"

void ControlPlayerSystem(entt::registry& registry, float cameraAngleX)
{
    auto view = registry.view<PlayerTag, Physics, TransformComponent>();

    for (auto entity : view) {
        auto& phys = view.get<Physics>(entity);
        auto& trans = view.get<TransformComponent>(entity);

        Vector3 moveDir = { 0.0f, 0.0f, 0.0f };
        bool isMoving = false;
        bool isRunning = IsKeyDown(KEY_LEFT_SHIFT);

        // detect (zqsd) in function of the camera's angle
        if (IsKeyDown(KEY_W)) {
            moveDir.x += -sinf(cameraAngleX);
            moveDir.z += -cosf(cameraAngleX);
            isMoving = true;
        }
        if (IsKeyDown(KEY_S)) {
            moveDir.x +=  sinf(cameraAngleX);
            moveDir.z +=  cosf(cameraAngleX);
            isMoving = true;
        }
        if (IsKeyDown(KEY_A)) {
            moveDir.x +=  sinf(cameraAngleX - PI/2.0f);
            moveDir.z +=  cosf(cameraAngleX - PI/2.0f);
            isMoving = true;
        }
        if (IsKeyDown(KEY_D)) {
            moveDir.x +=  sinf(cameraAngleX + PI/2.0f);
            moveDir.z +=  cosf(cameraAngleX + PI/2.0f);
            isMoving = true;
        }

        // velocity horizontal
        if (isMoving) {
            float magnitude = sqrtf(moveDir.x * moveDir.x + moveDir.z * moveDir.z);
            float currentSpeed = isRunning ? 0.2f : 0.1f;

            phys.velocity.x = (moveDir.x / magnitude) * currentSpeed;
            phys.velocity.z = (moveDir.z / magnitude) * currentSpeed;

            // this is to rotate the player to the movement direction
            trans.rotation = atan2f(moveDir.x, moveDir.z) * RAD2DEG;
        } else {
            // stop the movement is no key pressed
            phys.velocity.x = 0.0f;
            phys.velocity.z = 0.0f;
        }

        // handle jump
        if (phys.isGrounded && IsKeyPressed(KEY_SPACE)) {
            phys.velocity.y = 0.3f; // Jump Force
            phys.isGrounded = false;
        }
    }
}
