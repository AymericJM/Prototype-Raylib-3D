/*
** RayJump 64
** File description:
** AnimationSystem
*/

#include <entt/entt.hpp>
#include "components/Components.hpp"

void AnimationSystem(entt::registry& registry)
{
    auto view = registry.view<ModelRenderer>();

    for (auto entity : view) {
        auto& renderer = view.get<ModelRenderer>(entity);

        if (renderer.animsCount > 0) {
            ModelAnimation anim = renderer.modelAnim[renderer.currentAnimIndex];

            if (renderer.loop || renderer.currentAnimFrame < anim.frameCount - 1) {
                renderer.currentAnimFrame++;

                if (renderer.loop && renderer.currentAnimFrame >= anim.frameCount)
                    renderer.currentAnimFrame = 0;
            }

            UpdateModelAnimation(renderer.model, anim, renderer.currentAnimFrame);
        }
    }
}
