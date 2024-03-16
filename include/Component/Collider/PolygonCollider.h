#pragma once

#include "Object.h"
#include "Collider.h"
#include "glm/vec2.hpp"
#include <vector>
#include "glm/gtx/rotate_vector.hpp"
#include <iostream>

class PolygonCollider : public Collider
{
public:
	glm::vec2 size {0, 0};
    std::vector<glm::vec2> edges;

	PolygonCollider(Object* obj, glm::vec2 size) : Collider(obj), size(size) {
        initEdges();
        updateEdges(obj->getRot());

//        obj->onRotChange += [this](float rot) {
//            updateEdges(rot);
//        };


//        for (auto edge : edges) {
//            std::cout << edge.x << " " << edge.y << std::endl;
//        }
//
//        std::cout << "\n";
    }

	bool collidesWith(Collider* other) override;
	static double findDistance(double startX, double startY, double endX, double endY);

private:
    void initEdges() {
        glm::vec2 sPos = obj->getPos();

        edges = {
                {sPos.x - size.x / 2, sPos.y - size.y / 2}, // bottom left
                {sPos.x - size.x / 2, sPos.y + size.y / 2}, // top left
                {sPos.x + size.x / 2, sPos.y + size.y / 2}, // top right
                {sPos.x + size.x / 2, sPos.y - size.y / 2}  // bottom right
        };
    }

    void updateEdges(float rotation = 0.0f) {
        glm::vec2 sPos = obj->getPos();
        float angle = glm::radians(rotation);

        for (auto& edge : edges) {
            auto edg = glm::rotate(edge - sPos, angle) + sPos;

            std::cout << edg.x << " " << edg.y << std::endl;
        }
    }
};
