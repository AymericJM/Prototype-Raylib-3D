/*
** RayJump 64
** File description:
** Components
*/

#ifndef COMPONENTS_HPP_
#define COMPONENTS_HPP_

#include <raylib.h>

// ############################################################################
// ################################## CORE ####################################
// ############################################################################

/**
 * @brief Represents the spatial state of an entity in the world.
 *
 * @param pos Vector3
 * @param rotation float
 * @param scale float
 */
struct TransformComponent
{
    Vector3 pos;
    float rotation;
    float scale;
};


// ############################################################################
// ################################ PHYSIC ####################################
// ############################################################################

struct Physics
{
    Vector3 velocity;
    bool isGrounded;
};


// ############################################################################
// ################################ RENDER  ###################################
// ############################################################################

/**
 * @brief Represents the 3d model of an entity in the world.
 *
 * @param model Model
 * @param modelAnim ModelAnimation*
 * @param animsCount int
 * @param currentAnimIndex int
 * @param currentAnimFrame int
 */
struct ModelRenderer
{
    Model model;
    ModelAnimation* modelAnim;
    int animsCount;
    int currentAnimIndex;
    int currentAnimFrame;

    bool loop;
};


// ############################################################################
// ################################# TAGS #####################################
// ############################################################################


struct PlayerTag {};

struct ObstacleTag { BoundingBox box; };


#endif /* !COMPONENTS_HPP_ */
