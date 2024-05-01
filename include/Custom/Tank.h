#pragma once

#include "Color.h"
#include "Component.h"

class TankPlayerController;
class TextRenderer;
class TankRemoteController;
class SpriteRenderer;

class Tank : public Component
{
	float _firerate = 4;
	std::string _name;
	Color _mainColor;
	bool _controlledByPlayer;

	SpriteRenderer* _gun = nullptr;
	Transform* _gunPivot = nullptr;
	Collider* _col = nullptr;
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

	Transform* gunPivot() const;

	TankPlayerController* playerController() const;
	TankRemoteController* remoteController() const;

	friend class GameObject;
	friend class TankRemoteController;
	friend class TankPlayerController;
};
