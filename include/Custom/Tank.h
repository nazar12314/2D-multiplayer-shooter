#pragma once

#include "Color.h"
#include "Component.h"

struct Player;
class Rigidbody;
class TankPlayerController;
class TextRenderer;
class TankRemoteController;
class SpriteRenderer;

class Tank : public Component
{
	float _firerate = 1;
	std::string _name;
	Color _mainColor;
	bool _controlledByPlayer;

	Player* _player = nullptr;
	SpriteRenderer* _gun = nullptr;
	Transform* _gunPivot = nullptr;
	Collider* _col = nullptr;
	Rigidbody* _rb = nullptr;
	TextRenderer* _nameText = nullptr;

	TankPlayerController* _playerController = nullptr;
	TankRemoteController* _remoteController = nullptr;

	float _shootTimer = 0;

	Tank(GameObject* obj, const std::string& name, const Color& mainColor, bool controlledByPlayer);

	void start() override;
	void createGun();
	void createParts() const;
	void createParticles() const;
	void createNameTag();

	void update() override;
	void lateUpdate() override;
	void shoot(bool silent = false);

public:
	bool didShoot = false;

	Player* player() const;
	Transform* gunPivot() const;
	Rigidbody* rb() const;
	TankPlayerController* playerController() const;
	TankRemoteController* remoteController() const;

	void setPlayer(Player* player);

	void kill(const Tank* killer);
	void respawn(glm::vec2 pos);

	friend class GameObject;
	friend class TankRemoteController;
	friend class TankPlayerController;
};
