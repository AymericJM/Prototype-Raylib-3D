/*
** Prototype-Raylib-3D
** File description:
** PhysicsSystem
*/

#include <entt/entt.hpp>
#include "components/Components.hpp"

void PhysicsSystem(entt::registry& registry)
{
    auto view = registry.view<TransformComponent, Physics>();
    auto obstacles = registry.view<ObstacleTag>();

    for (auto entity : view) {
        auto& pos = view.get<TransformComponent>(entity).pos;
        auto& phys = view.get<Physics>(entity);

        // gravity here
        phys.velocity.y += -0.015f;
        pos.y += phys.velocity.y;

        // reset isGrounded each frame
        phys.isGrounded = false;

        // detect floor only by obstacles
        BoundingBox playerBox = {
            { pos.x - 0.4f, pos.y, pos.z },
            { pos.x + 0.4f, pos.y + 2.0f, pos.z + 0.4f }
        };

        for (auto obstacle : obstacles) {
            auto& obsBox = obstacles.get<ObstacleTag>(obstacle).box;

            if (CheckCollisionBoxes(playerBox, obsBox)) {
                // check if we are on the top of an obstacle
                if (phys.velocity.y < 0.0f && pos.y >= obsBox.max.y - 0.5f) {
                    pos.y = obsBox.max.y;
                    phys.velocity.y = 0.0f;
                    phys.isGrounded = true;
                }
            }
        }

        // DEAD ZONE set here
        if (pos.y < -50.0f)
            pos = (Vector3){ 0, 5, 0 }; // TODO: set correct respawn coord


        // Test X
        Vector3 nextPosX = pos;
        nextPosX.x += phys.velocity.x;
        BoundingBox boxX = { { nextPosX.x - 0.4f, pos.y, pos.z - 0.4f }, { nextPosX.x + 0.4f, pos.y + 2.0f, pos.z + 0.4f } };
        bool colX = false;
        for (auto obsEnt : obstacles) {
            auto& b = obstacles.get<ObstacleTag>(obsEnt).box;
            if (CheckCollisionBoxes(boxX, b) && pos.y < b.max.y - 0.2f) colX = true;
        }
        if (!colX) pos.x = nextPosX.x;

        // Test Z
        Vector3 nextPosZ = pos;
        nextPosZ.z += phys.velocity.z;
        BoundingBox boxZ = { { pos.x - 0.4f, pos.y, nextPosZ.z - 0.4f }, { pos.x + 0.4f, pos.y + 2.0f, nextPosZ.z + 0.4f } };
        bool colZ = false;
        for (auto obsEnt : obstacles) {
            auto& b = obstacles.get<ObstacleTag>(obsEnt).box;
            if (CheckCollisionBoxes(boxZ, b) && pos.y < b.max.y - 0.2f) colZ = true;
        }
        if (!colZ) pos.z = nextPosZ.z;

    }

}
