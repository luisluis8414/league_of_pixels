#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <unordered_map>
#include <typeindex>
class Entity;

enum class EventType {
	DRAW,
	TICK_UPDATE,
	ANIMATION_UPDATE,
	GAME_OVER,
	DESTROY_ENTITY,
	KEYPRESSED,
	WINDOW_RESIZE
};

class Event {
	friend class EventDispatcher;
public:
	virtual const char* GetName() const = 0;

	virtual EventType GetEventType() const { return m_eventType; };
	virtual std::string ToString() const { return GetName(); }

protected:
	Event(EventType type) : m_eventType(type) {}
	EventType m_eventType;
	bool m_handled = false;
};	

//class PlayerMovedEvent : public Event {
//public:
//	PlayerMovedEvent(int x, int y) : m_x(x), m_y(y) {};
//	int GetX() const { return m_x; }
//	int GetY() const { return m_y; }
//
//	std::string ToString() const override {
//		std::stringstream ss;
//		ss << "PlayerMovedEvent: x: " << m_x << ", y: " << m_y;
//		return ss.str();
//	}
//private:
//	int m_x, m_y;
//};

class DrawEvent : public Event {
public:
	DrawEvent(sf::RenderWindow& window) : Event(EventType::DRAW), m_window(window) {};
	
	sf::RenderWindow& GetWindow() const { return m_window; }
	const char* GetName() const override { return "Draw Event"; }
	
private:
	sf::RenderWindow& m_window;
};

class TickEvent : public Event {
public:
	TickEvent(const float& deltaTime) : Event(EventType::TICK_UPDATE), m_detlaTime(deltaTime) {};

	const float GetDeltaTime() const { return m_detlaTime; }
	const char* GetName() const override { return "Update Event"; }

private:
	const float m_detlaTime;
};

class KeyPressedEvent : public Event {
public:
	KeyPressedEvent(sf::Event::KeyEvent key_event) : Event(EventType::KEYPRESSED), m_keyEvent(key_event) {};

	const sf::Event::KeyEvent& GetKeyboardEvent() const { return m_keyEvent; }
	const char* GetName() const override { return "Key Pressed Event"; }

private:
	sf::Event::KeyEvent m_keyEvent;
};

class GameOverEvent : public Event {
public:
	GameOverEvent() : Event(EventType::GAME_OVER) {};

	const char* GetName() const override { return "Key Pressed Event"; }
};

class DestroyEntityEvent : public Event {
public:
	DestroyEntityEvent(Entity* entity) : Event(EventType::DESTROY_ENTITY), m_entity(entity) {};

	const char* GetName() const override { return "Destroy Entity Event"; }
	Entity* GetEntity() { return m_entity; }
private:
	Entity* m_entity;
};



class EventDispatcher {
public:
	template <typename T>
	using EventFn = std::function<void(T&)>;

	template <typename T>
	void subscribe(void* entity, const EventFn<T>& listener) {
		auto& listeners = m_subscriptions[std::type_index(typeid(T))];
		listeners.push_back({
			entity,
			[listener](Event& event) {
				listener(static_cast<T&>(event)); 
			}
		});
	}

	void unsubscribe(void* entity) {
		for (auto it = m_subscriptions.begin(); it != m_subscriptions.end(); ++it) {
			auto& listeners = it->second; 

			listeners.erase(
				std::remove_if(
					listeners.begin(),
					listeners.end(),
					[entity](const Subscription& sub) {
						return sub.subscriber == entity;
					}
				),
				listeners.end()
			);

			
			if (listeners.empty()) {
				it = m_subscriptions.erase(it); 
			}
		}
	}

	void emit(Event& event) {
		auto it = m_subscriptions.find(std::type_index(typeid(event)));
		if (it != m_subscriptions.end()) {
			for (auto& subscription : it->second) {
				if (event.m_handled) break;
				subscription.callback(event);
			}
		}
	}

private:
	struct Subscription {
		void* subscriber; 
		std::function<void(Event&)> callback;
	};

	std::unordered_map<std::type_index, std::vector<Subscription>> m_subscriptions;
};
