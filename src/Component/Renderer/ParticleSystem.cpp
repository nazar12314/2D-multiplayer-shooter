#include "ParticleSystem.h"

#include "MyTime.h"
#include "Texture.h"
#include "Transform.h"

Particle::Particle(const glm::vec2& pos, float rot, float lifetime, const glm::vec2& speed, const Curve<float>& scale, const Curve<Color>& color) :
	pos(pos), rot(rot), lifetime(lifetime), speed(speed), scale(scale), color(color) {}

ParticleSystem::ParticleSystem(GameObject* obj, const Sprite* sprite, const Color& color, int order): BaseRenderer(obj, color, order), _lastPos(transform()->getPos())
{
	_texture = new Texture(sprite);
}

void ParticleSystem::update()
{
	updateEmission();
	updateProperties();
	updateAlive();
}
void ParticleSystem::updateEmission()
{
	if (!_emitting)
	{
		_lastPos = transform()->getPos();
		return;
	}

	auto currPos = transform()->getPos();

	_emissionTimer += _emissionRate.eval() * Time::deltaTime();
	if (_rateOverDistance.to != 0)
	{
		_emissionTimer += distance(currPos, _lastPos) * _rateOverDistance.eval();
		_lastPos = currPos;
	}

	while (_emissionTimer >= 1)
	{
		_emissionTimer -= 1;

		auto localPos = glm::vec2(Math::randomFloat(-shape.x, shape.x), Math::randomFloat(-shape.y, shape.y));
		auto spawnPos = transform()->localToWorldPos(localPos);
		auto particle = Particle(spawnPos, _rot.eval(), _lifetime.eval(), Math::randomDir() * _speed.eval(), _scale.eval(), _color.eval());
		_particles.push_back(particle);
	}
}
void ParticleSystem::updateProperties()
{
	auto dt = Time::deltaTime();
	for (int i = 0; i < _particles.size(); i++)
	{
		auto& p = _particles[i];
		p.pos += p.speed * dt;
	}
}
void ParticleSystem::updateAlive()
{
	auto dt = Time::deltaTime();
	for (int i = 0; i < _particles.size(); i++)
	{
		auto& p = _particles[i];
		p.aliveTime += dt;
		if (p.aliveTime >= p.lifetime)
		{
			_particles.erase(_particles.begin() + i);
			i--;
		}
	}
}

void ParticleSystem::render(const Camera* camera) const
{
	for (const auto& p : _particles)
	{
		auto color = p.color.evalAt(p.aliveTime);
		_texture->setColor(color);

		auto size = glm::vec2_one * p.scale.evalAt(p.aliveTime);
		Renderer::renderTexWorld(_texture->texture(), p.pos, size, p.rot, camera);
	}
}

RangedValue<float>& ParticleSystem::emissionRate() { return _emissionRate; }
RangedValue<float>& ParticleSystem::rateOverDistance() { return _rateOverDistance; }
RangedValue<float>& ParticleSystem::lifetime() { return _lifetime; }
RangedValue<float>& ParticleSystem::speed() { return _speed; }
RangedValue<float>& ParticleSystem::rot() { return _rot; }

RangedCurve<float>& ParticleSystem::scale() { return _scale; }
RangedCurve<Color>& ParticleSystem::color() { return _color; }

void ParticleSystem::setShape(const glm::vec2& shape) { this->shape = shape; }

void ParticleSystem::setEmitting(bool emitting) { _emitting = emitting; }
