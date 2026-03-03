/*
** RayJump 64
** File description:
** CameraSystem
*/

#include <entt/entt.hpp>
#include "components/Components.hpp"

void CameraSystem(Camera& camera, Vector3 targetPos, Vector2& cameraAngle, float& cameraDistance)
{
    Vector2 mouseDelta = GetMouseDelta();
    cameraAngle.x -= mouseDelta.x * 0.003f;
    cameraAngle.y -= mouseDelta.y * 0.003f;

    if (cameraAngle.y > 1.2f) cameraAngle.y = 1.2f;
    if (cameraAngle.y < 0.3f) cameraAngle.y = 0.3f;

    cameraDistance -= GetMouseWheelMove() * 0.5f;
    if (cameraDistance < 3.0f) cameraDistance = 3.0f;
    if (cameraDistance > 10.0f) cameraDistance = 10.0f;

    float eyeHeight = 1.5f;
    camera.target = (Vector3){ targetPos.x, targetPos.y + eyeHeight, targetPos.z };
    camera.position.x = targetPos.x + cameraDistance * cosf(cameraAngle.y) * sinf(cameraAngle.x);
    camera.position.y = (targetPos.y + eyeHeight) + cameraDistance * sinf(cameraAngle.y);
    camera.position.z = targetPos.z + cameraDistance * cosf(cameraAngle.y) * cosf(cameraAngle.x);

    UpdateCamera(&camera, CAMERA_CUSTOM);
}
