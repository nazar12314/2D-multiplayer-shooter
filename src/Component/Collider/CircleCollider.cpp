//
// Created by Nazar Kononenko on 16.03.2024.
//

#include "CircleCollider.h"
#include "PolygonCollider.h"
#include "glm/geometric.hpp"
#include "Object.h"

bool CircleCollider::collidesWith(Collider *other) {
    if (auto* box = dynamic_cast<PolygonCollider*>(other))
    {
        return box->collidesWith(this);
    }
    if (auto* circle = dynamic_cast<CircleCollider*>(other))
    {
        float distance = glm::distance(obj->pos(), other->obj->pos());
        return distance < radius + circle->radius;
    }

    return false;
}
