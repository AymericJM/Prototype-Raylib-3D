/*
** EPITECH PROJECT, 2026
** RayJump-64
** File description:
** UpdatePlayerAnimationState
*/

#include "components/Components.hpp"

void UpdatePlayerAnimationState(Physics& phys, ModelRenderer& renderer)
{
    int nextAnim = 2; // 2 = Idle for the robot
    bool shouldLoop = true;

    if (!phys.isGrounded) {
        nextAnim = 3;      // Jump 
        shouldLoop = false;
    } else if (phys.velocity.x != 0 || phys.velocity.z != 0) {
        nextAnim = IsKeyDown(KEY_LEFT_SHIFT) ? 6 : 10;
        shouldLoop = true;
    }

    // reset the frame if the index of the anim change to refresh the anim
    if (renderer.currentAnimIndex != nextAnim) {
        renderer.currentAnimIndex = nextAnim;
        renderer.currentAnimFrame = 0;
        renderer.loop = shouldLoop;
    }
}
