/*
** Prototype-Raylib-3D
** File description:
** RenderSystem
*/

#include "entt/entt.hpp"
#include "components/Components.hpp"

void RenderSystem(entt::registry& registry)
{
    auto view = registry.view<TransformComponent, ModelRenderer>();

    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& render = view.get<ModelRenderer>(entity);

        DrawModelEx(render.model,
            transform.pos,
            (Vector3){ 0, 1, 0},
            transform.rotation,
            (Vector3){transform.scale, transform.scale, transform.scale},
            WHITE);
    }
}
