#include "BoxCollider.h"

bool BoxCollider::collidesWith(const Collider* other) {
    const auto* sTransform = dynamic_cast<const Object*>(this);
    const auto* oTransform = dynamic_cast<const Object*>(other);

    float sRot = glm::radians(sTransform->getRot());

    glm::vec2 sPos = sTransform->getPos();
    glm::vec2 oPos = oTransform->getPos();

    if (const auto * box = dynamic_cast<const BoxCollider*>(other)) {
        auto [sAxisX, sAxisY] = getAxis(this);
        auto [oAxisX, oAxisY] = getAxis(box);

        if (!overlapOnAxis(sPos, oPos, sAxisX, sAxisY, box) ||
            !overlapOnAxis(sPos, oPos, oAxisX, oAxisY, box)) {
            return false;
        }

        return true;
    } else if (const auto * circle = dynamic_cast<const CircleCollider*>(other)) {
        double unrotatedX = std::cos(sRot)
                * (oPos.x - sPos.x) - std::sin(sRot)
                * (oPos.y - sPos.y) + sPos.x;

        double unrotatedY = std::sin(sRot)
                * (oPos.x - sPos.x) + std::cos(sRot)
                * (oPos.y - sPos.y) + sPos.y;

        double closestX, closestY;

        if (unrotatedX < sPos.y)
            closestX = sPos.y;
        else if (unrotatedX > sPos.y + size.x)
            closestY = sPos.y + size.x;
        else
            closestY = unrotatedY;

        double distance = findDistance(unrotatedX, unrotatedY, closestX, closestY);

        if (distance < circle->getRadius()) return true;
    }

    return false;
}

double BoxCollider::findDistance(double startX, double startY, double endX, double endY) {
    double a = std::abs(startX - endX);
    double b = std::abs(startY - endY);

    return std::sqrt((a * a) + (b * b));
}

std::pair<glm::vec2, glm::vec2> BoxCollider::getAxis(const BoxCollider *box) {
    glm::vec2 OX = {1, 0};
    glm::vec2 OY = {0, 1};

    const auto* boxTransform = dynamic_cast<const Object*>(box);
    float rot = boxTransform->getRot();

    auto rotateX = [](glm::vec2 pos, float rot) { return pos.x * std::cos(rot) - pos.y * std::sin(rot);};
    auto rotateY = [](glm::vec2 pos, float rot) { return pos.x * std::sin(rot) + pos.y * std::cos(rot);};

    OX.x = rotateX(OX, rot);
    OX.y = rotateY(OX, rot);

    OY.x = rotateX(OY, rot);
    OY.y = rotateY(OY, rot);

    return {OX, OY};
}

float BoxCollider::projectOntoAxis(const glm::vec2& pos, const glm::vec2& axisX, const glm::vec2& axisY, const glm::vec2& halfSize) {
    glm::vec2 corner = pos - axisX * halfSize.x - axisY * halfSize.y;

    return corner.x * axisX.x + corner.y * axisX.y;
}

bool BoxCollider::overlapOnAxis(const glm::vec2& sPos, const glm::vec2& oPos, const glm::vec2& axisX, const glm::vec2& axisY, const BoxCollider* box) {
    glm::vec2 sHalfSize = size * 0.5f;
    glm::vec2 oHalfSize = box->size * 0.5f;

    float sProjection = projectOntoAxis(sPos, axisX, axisY, sHalfSize);
    float oProjection = projectOntoAxis(oPos, axisX, axisY, oHalfSize);

    float overlap = std::abs(sProjection - oProjection);
    float combinedHalfSize = sHalfSize.x + oHalfSize.x;

    return overlap < combinedHalfSize;
}
