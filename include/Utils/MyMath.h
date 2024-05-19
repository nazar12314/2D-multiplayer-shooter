#pragma once
#include <random>
#include <vector>

#include "glm/fwd.hpp"
#include "glm/vec2.hpp"
#include "glm/mat3x3.hpp"

enum class EaseType
{
	Linear = 0,
	InQuad = 1,
	OutQuad = 2,
	InOutQuad = 3,
	InCubic = 4,
	OutCubic = 5,
	InOutCubic = 6,
	InQuart = 7,
	OutQuart = 8,
	InOutQuart = 9,
};

class Math
{
	inline static std::mt19937 gen {std::random_device()()};

	static float shortestAngle(float currentAngle, float targetAngle);

public:
	static float rotateTowardsAngle(float currentAngle, float targetAngle, float maxStep);
	static float rotateTowardsDir(float currentAngle, glm::vec2 dir, float maxStep);

	static float distanceSquared(glm::vec2 a, glm::vec2 b);
	static float cross(glm::vec2 a, glm::vec2 b);

	static float round(float value, int digits = 0);

	template <typename T> requires requires(T& t1, T& t2) { t1 + t2; t1 * t2; }
	static T lerp(const T& a, const T& b, float t, EaseType ease = EaseType::Linear);

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
	static glm::ivec2 randomVec2(int min, int max);
	static glm::vec2 randomVec2(glm::vec2 min, glm::vec2 max);
	static glm::vec2 randomDir();

	static bool nearlyEqual(float a, float b, float epsilon = 0.0001f);
	static bool nearlyEqual(glm::vec2 a, glm::vec2 b, float epsilon = 0.0001f);

	static bool nearlyZero(float a, float epsilon = 0.0001f);
	static bool nearlyZero(glm::vec2 a, float epsilon = 0.0001f);

	static float evaluateEase(EaseType ease, float t);

	static void setRandomSeed(int seed) { gen.seed(seed); }
};

template <typename T> requires requires(T& t1, T& t2) { t1 + t2; t1 * t2; }
T Math::lerp(const T& a, const T& b, float t, EaseType ease) { return a + (b - a) * evaluateEase(ease, glm::clamp(t, 0.0f, 1.0f)); }

namespace glm
{
	inline static constexpr vec2 vec2_zero = {0, 0};
	inline static constexpr vec2 vec2_one = {1, 1};

	inline static constexpr vec2 vec2_up = {0, 1};
	inline static constexpr vec2 vec2_down = {0, -1};
	inline static constexpr vec2 vec2_left = {-1, 0};
	inline static constexpr vec2 vec2_right = {1, 0};

	inline static constexpr mat3 mat3_identity = mat3(1);
}

namespace uuid
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution dis(0, 15);
	static std::uniform_int_distribution dis2(8, 11);

	std::string generate_uuid_v4();
}
