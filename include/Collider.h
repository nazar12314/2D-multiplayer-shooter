//
// Created by Nazar Kononenko on 09.03.2024.
//

#ifndef INC_2D_MULTIPLAYER_SHOOTER_COLLIDER_H
#define INC_2D_MULTIPLAYER_SHOOTER_COLLIDER_H

#include "CircleCollider.h"
#include "Object.h"
#include "glm/vec2.hpp"
#include <glm/glm.hpp>

#include <cmath>

class Collider {
public:
    virtual ~Collider() = 0;

    virtual bool collidesWith(const Collider* other) = 0;
    void onCollision(const Collider* other);
};


#endif //INC_2D_MULTIPLAYER_SHOOTER_COLLIDER_H
