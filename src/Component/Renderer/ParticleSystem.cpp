#include "ParticleSystem.h"

#include "MyTime.h"
#include "Texture.h"
#include "Transform.h"

Particle::Particle(const glm::vec2& pos, float rot, float lifetime, const glm::vec2& speed, const Curve<float>& scale, const Curve<Color>& color) :
	pos(pos), rot(rot), lifetime(lifetime), speed(speed), scale(scale), color(color) {}

ParticleSystem::ParticleSystem(GameObject* obj, const Sprite* sprite, const Color& color, int order): BaseRenderer(obj, color, order),
                                                                                                      _lastDistanceEmitPos(transform()->pos())
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
		_lastDistanceEmitPos = transform()->pos();
		return;
	}

	if (_rateOverDistance.to != 0)
	{
		auto currPos = transform()->pos();
		auto dir = normalize(currPos - _lastDistanceEmitPos);
		auto dis = distance(currPos, _lastDistanceEmitPos);
		auto rateDis = 1 / _rateOverDistance.eval();
		while (dis >= rateDis)
		{
			dis -= rateDis;

			_lastDistanceEmitPos += dir * rateDis;
			emitParticle(_lastDistanceEmitPos - currPos);
			rateDis = 1 / _rateOverDistance.eval();
		}
	}

	_emissionTimer += _emissionRate.eval() * Time::deltaTime();
	while (_emissionTimer >= 1)
	{
		_emissionTimer -= 1;

		emitParticle();
	}
}
void ParticleSystem::onEnable()
{
	_emissionTimer = 0;
	_lastDistanceEmitPos = transform()->pos();
	_particles.clear();
}

void ParticleSystem::emitParticle(const glm::vec2& offset)
{
	auto localPos = glm::vec2(Math::randomFloat(-shape.x, shape.x), Math::randomFloat(-shape.y, shape.y));
	auto spawnPos = transform()->localToWorldPos(localPos) + offset;
	auto particle = Particle(spawnPos, _rot.eval(), _lifetime.eval(), Math::randomDir() * _speed.eval(), _scale.eval(), _color.eval());
	_particles.push_back(particle);
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
	for (int i = _particles.size() - 1; i >= 0; i--)
	{
		auto& p = _particles[i];
		p.aliveTime += dt;
		if (p.aliveTime >= p.lifetime)
			_particles.erase(_particles.begin() + i);
	}
}

void ParticleSystem::render(const Camera* camera) const
{
	for (const auto& p : _particles)
	{
		float t = p.aliveTime / p.lifetime;

		auto color = p.color.evalAt(t);
		_texture->setColor(color);

		auto size = glm::vec2_one * p.scale.evalAt(t);
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
