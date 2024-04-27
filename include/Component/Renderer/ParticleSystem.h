#pragma once

#include "BaseRenderer.h"
#include "MyMath.h"

// Curve
template <typename T> struct Curve
{
	T from;
	T to;
	EaseType ease = EaseType::Linear;

	Curve(T value) : from(value), to(value) {}
	Curve(T from, T to, EaseType ease = EaseType::Linear) : from(from), to(to), ease(ease) {}

	T evalAt(float t) const { return Math::lerp(from, to, t, ease); }
	T eval() const { return Math::lerp(from, to, Math::randomValue(), ease); }

	void set(T value);
	void set(T from, T to, EaseType ease = EaseType::Linear);

	void setFrom(const T& from);
	void setTo(const T& to);
	void setEase(EaseType ease);
};

template <typename T> struct RangedValue : Curve<T>
{
	RangedValue(T value) : Curve<T>(value) {}
	RangedValue(T from, T to) : Curve<T>(from, to, EaseType::Linear) {}
};

template <typename T> struct RangedCurve
{
	Curve<T> min;
	Curve<T> max;

	RangedCurve(T value);
	RangedCurve(const Curve<T>& min, const Curve<T>& max);

	Curve<T> eval() const;

	void set(const T& value);
	void set(const T& min, const T& max);
	void set(const T& minFrom, const T& maxFrom, const T& minTo, const T& maxTo);
};


// Particle
struct Particle
{
	glm::vec2 pos;
	float rot;
	float lifetime;
	glm::vec2 speed;

	Curve<float> scale;
	Curve<Color> color;

	float aliveTime = 0;

	Particle(const glm::vec2& pos, float rot, float lifetime, const glm::vec2& speed, const Curve<float>& scale, const Curve<Color>& color);
};


// ParticleSystem
class ParticleSystem : public BaseRenderer
{
	// -- Properties --
	RangedValue<float> _emissionRate = 10;
	RangedValue<float> _rateOverDistance = 0;
	RangedValue<float> _lifetime = {1, 1};
	RangedValue<float> _speed = {1};
	RangedValue<float> _rot = {0};

	RangedCurve<float> _scale = {1};
	RangedCurve<Color> _color = {Color::WHITE};

	glm::vec2 shape = glm::vec2(1);
	// -- Properties --

	bool _emitting = true;
	std::vector<Particle> _particles;

	glm::vec2 _lastPos;
	float _emissionTimer = 0;

	ParticleSystem(GameObject* obj, const Sprite* sprite, const Color& color = Color::WHITE, int order = 0);

	void update() override;
	void updateAlive();
	void updateProperties();
	void updateEmission();

	void render(const Camera* camera) const override;

public:
	RangedValue<float>& emissionRate();
	RangedValue<float>& rateOverDistance();
	RangedValue<float>& lifetime();
	RangedValue<float>& speed();
	RangedValue<float>& rot();

	RangedCurve<float>& scale();
	RangedCurve<Color>& color();

	void setShape(const glm::vec2& shape);

	void setEmitting(bool emitting);

	friend class GameObject;
};

template <typename T> void Curve<T>::set(T value) { from = to = value; }
template <typename T> void Curve<T>::set(T from, T to, EaseType ease)
{
	this->from = from;
	this->to = to;
	this->ease = ease;
}
template <typename T> void Curve<T>::setFrom(const T& from) { this->from = from; }
template <typename T> void Curve<T>::setTo(const T& to) { this->to = to; }
template <typename T> void Curve<T>::setEase(EaseType ease) { this->ease = ease; }

template <typename T> RangedCurve<T>::RangedCurve(T value): min(value), max(value) {}
template <typename T> RangedCurve<T>::RangedCurve(const Curve<T>& min, const Curve<T>& max): min(min), max(max) {}
template <typename T> Curve<T> RangedCurve<T>::eval() const
{
	auto t = Math::randomValue();
	auto from = Math::lerp(min.from, max.from, t);
	auto to = Math::lerp(min.to, max.to, t);
	return Curve(from, to, min.ease); // max.ease is ignored for now
}

template <typename T> void RangedCurve<T>::set(const T& value)
{
	min.set(value);
	max.set(value);
}
template <typename T> void RangedCurve<T>::set(const T& min, const T& max)
{
	this->min.set(min);
	this->max.set(max);
}
template <typename T> void RangedCurve<T>::set(const T& minFrom, const T& maxFrom, const T& minTo, const T& maxTo)
{
	min.set(minFrom, minTo);
	max.set(maxFrom, maxTo);
}
