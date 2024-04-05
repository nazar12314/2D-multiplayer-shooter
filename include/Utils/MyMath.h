#pragma once
#include <vector>

#include "glm/fwd.hpp"
#include "glm/vec2.hpp"
#include "glm/mat3x3.hpp"

class Math
{
	static float shortestAngle(float currentAngle, float targetAngle);

public:
	static float rotateTowardsAngle(float currentAngle, float targetAngle, float maxStep);
	static float rotateTowardsDir(float currentAngle, glm::vec2 dir, float maxStep);

	static float distanceSquared(glm::vec2 a, glm::vec2 b);
	static float cross(glm::vec2 a, glm::vec2 b);

	static float round(float value, int digits = 0);
	static float lerp(float a, float b, float t);

	static std::tuple<float, glm::vec2> findMinSeparation(const std::vector<glm::vec2>& verticesA, const std::vector<glm::vec2>& verticesB);
	static std::tuple<float, glm::vec2> findMinSeparation(glm::vec2 centerA, float radiusA, const std::vector<glm::vec2>& verticesB);
	static std::tuple<float, glm::vec2> findMinSeparation(glm::vec2 centerA, float radiusA, glm::vec2 centerB, float radiusB);

	static bool intersects(const std::vector<glm::vec2>& verticesA, const std::vector<glm::vec2>& verticesB);
	static bool intersects(glm::vec2 centerA, float radiusA, const std::vector<glm::vec2>& verticesB);
	static bool intersects(glm::vec2 centerA, float radiusA, glm::vec2 centerB, float radiusB);

	static std::vector<glm::vec2> findContactPoints(const std::vector<glm::vec2>& verticesA, const std::vector<glm::vec2>& verticesB);
	static std::vector<glm::vec2> findContactPoints(glm::vec2 centerA, float radiusA, const std::vector<glm::vec2>& verticesB);
	static std::vector<glm::vec2> findContactPoints(glm::vec2 centerA, float radiusA, glm::vec2 centerB, float radiusB);

	static int closestVertexToPoint(glm::vec2 point, const std::vector<glm::vec2>& vertices);

	static float randomFloat(float min, float max);
	static int randomInt(int min, int max);
	static float randomValue();
	static glm::vec2 randomVec2(float min, float max);
	static glm::vec2 randomVec2(glm::vec2 min, glm::vec2 max);
	static glm::vec2 randomDir();

	static bool nearlyEqual(float a, float b, float epsilon = 0.0001f);
	static bool nearlyEqual(glm::vec2 a, glm::vec2 b, float epsilon = 0.0001f);

	static bool nearlyZero(float a, float epsilon = 0.0001f);
	static bool nearlyZero(glm::vec2 a, float epsilon = 0.0001f);
};

namespace glm
{
	inline static constexpr vec2 vec2_up = {0, 1};
	inline static constexpr vec2 vec2_down = {0, -1};
	inline static constexpr vec2 vec2_left = {-1, 0};
	inline static constexpr vec2 vec2_right = {1, 0};

	inline static constexpr mat3 mat3_identity = mat3(1);
}
