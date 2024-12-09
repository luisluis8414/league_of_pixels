#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <unordered_map>
#include <typeindex>

enum class EventType {
	DRAW,
	WINDOW_RESIZE,
	ATTACK
};

class Event {
	friend class EventDispatcher;
public:
	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual std::string ToString() const { return GetName(); }

protected:
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
	DrawEvent(sf::RenderWindow& window) : m_window(window) {};
	sf::RenderWindow& GetWindow() const { return m_window; }

	EventType GetEventType() const override { return EventType::DRAW; }
	const char* GetName() const override { return "Draw Event"; }
	
private:
	sf::RenderWindow& m_window;
};


class EventDispatcher {
public:
	template <typename T>
	using EventFn = std::function<void(T&)>;

	template <typename T>
	void subscribe(const EventFn<T>& listener) {
		auto& listeners = m_listeners[std::type_index(typeid(T))];
		listeners.push_back([listener](Event& e) {
			listener(static_cast<T&>(e));
			});
	}

	void emit(Event& event) {
		auto it = m_listeners.find(std::type_index(typeid(event)));
		if (it != m_listeners.end()) {
			for (auto& listener : it->second) {
				if (event.m_handled) break;
				listener(event);
			}
		}
	}

private:
	std::unordered_map<std::type_index, std::vector<std::function<void(Event&)>>> m_listeners;
};
