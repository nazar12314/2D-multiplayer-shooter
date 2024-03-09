//
// Created by Nazar Kononenko on 09.03.2024.
//

#ifndef INC_2D_MULTIPLAYER_SHOOTER_BOXCOLLIDER_H
#define INC_2D_MULTIPLAYER_SHOOTER_BOXCOLLIDER_H

#include "Collider.h"


class BoxCollider :  Collider {
private:
    glm::vec2 size;

public:
    BoxCollider(double height, double width): size(height, width) {}

    bool collidesWith(const Collider* other) override;
    static double findDistance(double startX, double startY, double endX, double endY);
    static std::pair<glm::vec2, glm::vec2> getAxis(const BoxCollider* box);

    static float projectOntoAxis(const glm::vec2& pos, const glm::vec2& axisX, const glm::vec2& axisY, const glm::vec2& halfSize);
    bool overlapOnAxis(const glm::vec2& sPos, const glm::vec2& oPos, const glm::vec2& axisX, const glm::vec2& axisY, const BoxCollider* box);
};


#endif //INC_2D_MULTIPLAYER_SHOOTER_BOXCOLLIDER_H
