#include "TankEffects.h"

#include "Assets.h"
#include "DOTween.h"
#include "MyMath.h"
#include "Rigidbody.h"
#include "SpriteRenderer.h"
#include "Transform.h"
#include "TankController.h"

void TankEffects::start()
{
	_tankController = getComponent<TankController>();
	_lastPos = transform()->getPos();
}

void TankEffects::update()
{
	if (!_tankController->isMoving()) return;

	auto currPos = transform()->getPos();
	auto movedBy = distance(_lastPos, currPos);
	_lastPos = currPos;
	_movedByTracker += movedBy;

	while (_movedByTracker >= _particlesPerDistance)
	{
		_movedByTracker -= _particlesPerDistance;
		spawnSmokeParticle();
	}
}

void TankEffects::spawnSmokeParticle() const
{
	auto spawnPos = transform()->localToGlobalPos({Math::randomFloat(-0.25f, 0.25f), -0.5f});
	auto smoke = GameObject::create("Smoke", spawnPos)->transform();
	smoke->addComponent<SpriteRenderer>(Assets::load<Sprite>("assets/sprites/square.png"), glm::vec2(0.1f, 0.1f), Color(0.8f, 0.8f, 0.8f), -1);

	float dur = Math::randomFloat(0.5f, 1.0f);
	smoke->doScale({0, 0}, dur);
	smoke->doMove(smoke->getPos() + glm::vec2(0, 0.5f), dur)->onComplete([smoke] { destroy(smoke->gameObject()); });
}
