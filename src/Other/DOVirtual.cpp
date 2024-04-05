#include "DOVirtual.h"

void DOVirtual::update(float deltaTime)
{
	_tweens.apply_changes();
	for (auto call : _tweens)
		call->update(deltaTime);
}

Tween* DOVirtual::delayedCall(const std::function<void()>& function, float delay)
{
	auto delayedCall = new DelayedCall(function, delay);
	_tweens.push_back(delayedCall);
	return delayedCall;
}
Tween* DOVirtual::floatTo(float startValue, float endValue, float time, const std::function<void(float)>& setter)
{
	return valueTo(startValue, endValue, time, setter);
}
Tween* DOVirtual::vec2To(const glm::vec2& startValue, const glm::vec2& endValue, float time, const std::function<void(glm::vec2)>& setter)
{
	return valueTo(startValue, endValue, time, setter);
}
Tween* DOVirtual::colorTo(const Color& startValue, const Color& endValue, float time, const std::function<void(Color)>& setter)
{
	return valueTo(startValue, endValue, time, setter);
}

Tween::Tween(float time): _time(time) {}

void Tween::update(float deltaTime)
{
	_elapsed += deltaTime;
	if (_elapsed >= _time)
	{
		_elapsed = _time;
		finish();
	}
}

void Tween::finish()
{
	DOVirtual::_tweens.erase_delayed(this);
	delete this;
}
void Tween::kill()
{
	DOVirtual::_tweens.erase_delayed(this);
	delete this;
}

DelayedCall::DelayedCall(const std::function<void()>& function, float delay): Tween(delay), _function(function) {}
void DelayedCall::finish()
{
	_function();
	Tween::finish();
}
