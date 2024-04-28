#pragma once

#include "Component.h"

template <typename T> class Singleton : public Component
{
	inline static T* m_instance = nullptr;

	void awake() override
	{
		if (m_instance != nullptr)
		{
			destroy(gameObject());
			return;
		}

		m_instance = static_cast<T*>(this);
	}

protected:
	explicit Singleton(GameObject* obj) : Component(obj) { }

public:
	static T* instance()
	{
		if (m_instance == nullptr)
			m_instance = GameObject::create()->addComponent<T>();
		return m_instance;
	}

	friend class GameObject;
};
