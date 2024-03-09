//
// Created by Nazar Kononenko on 09.03.2024.
//

#ifndef INC_2D_MULTIPLAYER_SHOOTER_CIRCLECOLLIDER_H
#define INC_2D_MULTIPLAYER_SHOOTER_CIRCLECOLLIDER_H

#include "Collider.h"
#include "glm/vec2.hpp"

class CircleCollider : Collider {
private:
    float radius;

public:
    bool collidesWith(const Collider* collider) override;
    float getRadius() const;
};


#endif //INC_2D_MULTIPLAYER_SHOOTER_CIRCLECOLLIDER_H
