#pragma once

#include "Singleton.h"

class TextRenderer;
struct Player;

struct ScoreInfo
{
	Player* player;
	int score;
	TextRenderer* display;
};

class ScoreDisplayer : public Singleton<ScoreDisplayer>
{
	std::vector<ScoreInfo> scores;

	explicit ScoreDisplayer(GameObject* gameObject);

	void update() override;
	void updateDisplayPositions();

public:
	void addScore(Player* player, int score = 1);
	void addPlayerToScoreboard(Player* player, int score = 0);

	friend class GameObject;
};
