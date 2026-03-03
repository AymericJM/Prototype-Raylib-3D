/*
** RayJump-64
** File description:
** DebugRenderSystem
*/

#include "entt/entt.hpp"
#include "components/Components.hpp"

void DebugRenderSystem(entt::registry& registry) {
    auto view = registry.view<ObstacleTag>();

    for (auto entity : view) {
        auto& obs = view.get<ObstacleTag>(entity);

        // calcul the center and the size for DrawCubeWires method
        Vector3 size = {
            obs.box.max.x - obs.box.min.x,
            obs.box.max.y - obs.box.min.y,
            obs.box.max.z - obs.box.min.z
        };
        Vector3 center = {
            obs.box.min.x + size.x / 2,
            obs.box.min.y + size.y / 2,
            obs.box.min.z + size.z / 2
        };

        DrawCubeWires(center, size.x, size.y, size.z, RED);
    }
}
