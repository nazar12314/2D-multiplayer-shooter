#include "ScoreDisplayer.h"

#include <algorithm>

#include "Camera.h"
#include "PlayerManager.h"
#include "SDLHandler.h"
#include "TextRenderer.h"
#include "Transform.h"

ScoreDisplayer::ScoreDisplayer(GameObject* gameObject): Singleton(gameObject) {}

void ScoreDisplayer::update()
{
	updateDisplayPositions();
}
void ScoreDisplayer::updateDisplayPositions()
{
	std::ranges::sort(scores, [](const auto& a, const auto& b) { return a.score > b.score; });

	for (size_t i = 0; i < scores.size(); i++)
	{
		auto player = scores[i].player;
		auto display = scores[i].display;

		display->setText(player->name + ": " + std::to_string(scores[i].score));

		auto camera = Camera::getMain();
		auto cameraPos = camera->transform()->pos();
		auto cameraSize = camera->size();
		auto ratio = SDLHandler::windowAspectRatio;
		auto pos = cameraPos + glm::vec2(1.0f / 2 * ratio - 0.2f, 1.0f / 2 - 0.05f - i * 0.05f) * cameraSize;

		display->transform()->setScale({cameraSize / 2, cameraSize / 2});
		display->gameObject()->getComponent<Transform>()->setPos(pos);
	}
}

void ScoreDisplayer::addScore(Player* player, int score)
{
	auto it = std::ranges::find_if(scores, [player](const auto& pair) { return pair.player == player; });
	it->score += score;

	updateDisplayPositions();
}

void ScoreDisplayer::addPlayerToScoreboard(Player* player, int score)
{
	auto display = GameObject::create("ScoreText")->addComponent<TextRenderer>("", glm::vec2(1, 0.1f), player->color.saturate(0.5f).lighten(0.1f));
	scores.push_back({player, score, display});

	updateDisplayPositions();
}
