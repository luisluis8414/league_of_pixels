#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <unordered_map>
#include <typeindex>

class Entity;

enum class RenderLayer {
	BACKGROUND = -1,
	MAP,
	ENTITIES,
	PLAYER,
	TEXT,
	UI
};

enum class EventType {
	DRAW,
	TEXT_DRAW,
	MAP_DRAW,
	TICK_UPDATE,
	ANIMATION_UPDATE,
	GAME_OVER,
	DESTROY_ENTITY,
	KEYPRESSED,
	MOUSE_CLICKED,
	WINDOW_RESIZE,
	COLLISION_EVENT,
	TARGET_EVENT,
	MOVE_EVENT,
	SCROLL_EVENT,
	CURSOR_ON_EDGE_EVENT
};


class Event {
	friend class EventDispatcher;
public:
	virtual const char* getName() const = 0;

	virtual EventType getEventType() const { return m_eventType; };
	virtual std::string toString() const { return getName(); }

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
	
	sf::RenderWindow& getWindow() const { return m_window; }
	const char* getName() const override { return "Draw Event"; }
	
private:
	sf::RenderWindow& m_window;
};

class TickEvent : public Event {
public:
	TickEvent(const float& deltaTime) : Event(EventType::TICK_UPDATE), m_detlaTime(deltaTime) {};

	const float getDeltaTime() const { return m_detlaTime; }
	const char* getName() const override { return "Update Event"; }

private:
	const float m_detlaTime;
};

class KeyPressedEvent : public Event {
public:
	KeyPressedEvent(const sf::Event::KeyEvent key_event) : Event(EventType::KEYPRESSED), m_keyEvent(key_event) {};

	const sf::Event::KeyEvent& getKeyboardEvent() const { return m_keyEvent; }
	const char* getName() const override { return "Key Pressed Event"; }

private:
	const sf::Event::KeyEvent m_keyEvent;
};

class MouseRightClickEvent : public Event {
public:
	MouseRightClickEvent(const sf::Vector2f& position): Event(EventType::MOUSE_CLICKED), m_position(position) {};

	const sf::Vector2f& getPosition() const { return m_position; }
	const char* getName() const override { return "Left Mouse Click Event"; }
private: 
	const sf::Vector2f& m_position;
};

class GameOverEvent : public Event {
public:
	GameOverEvent() : Event(EventType::GAME_OVER) {};

	const char* getName() const override { return "Key Pressed Event"; }
};

class DestroyEntityEvent : public Event {
public:
	DestroyEntityEvent(Entity* entity)
		: Event(EventType::DESTROY_ENTITY), m_entity(entity) {
	}

	const char* getName() const override { return "Destroy Entity Event"; }
	Entity* getEntity() const { return m_entity; }

private:
	Entity* m_entity;
};


class CollisionEvent : public Event {
public:
	CollisionEvent(const Entity& entityA,const Entity& entityB) : Event(EventType::COLLISION_EVENT), m_entityA(entityA), m_entityB(entityB) {};
	const char* getName() const override { return "Collision Event"; }

	const Entity& getEntityA() const { return m_entityA; }
	const Entity& getEntityB() const { return m_entityB; }

private:
	const Entity& m_entityA;
	const Entity& m_entityB;
};

enum class ActionEventType {
	TargetAction,
	MoveAction
};

class ActionEvent : public Event {
public:
	ActionEvent(const ActionEventType actionType, const Entity& targeter, Entity* target, const sf::Vector2f& position)
		: Event(EventType::TARGET_EVENT),
		m_actor(targeter),
		m_target(target),
		m_destination(position),
		m_actionType(actionType) {
	}

	const char* getName() const override { return "Action Event"; }

	const Entity& getTargeter() const { return m_actor; }
	Entity* getTarget() const { return m_target; }
	const sf::Vector2f& getPosition() const { return m_destination; }
	ActionEventType getActionType() const { return m_actionType; }

private:
	const Entity& m_actor;             
	Entity* m_target;                  
	const sf::Vector2f& m_destination;
	ActionEventType m_actionType;      
};



class MoveEvent : public Event {
public:
	MoveEvent(sf::Sprite& sprite, const sf::FloatRect& hitbox, const sf::Vector2f& step, sf::Vector2f& destination)
		: Event(EventType::MOVE_EVENT), m_sprite(sprite), m_hitbox(hitbox), m_step(step), m_destination(destination){
	};

	const char* getName() const override { return "Move Event"; }

	sf::Sprite& getSprite() const { return m_sprite; }
	const sf::Vector2f& getStep() const { return m_step; }
	const sf::FloatRect& getHitbox() const { return m_hitbox; }
	sf::Vector2f& getDestination() { return m_destination; }

private:
	sf::Sprite& m_sprite;
	const sf::Vector2f& m_step;
	const sf::FloatRect& m_hitbox;
	sf::Vector2f& m_destination;
};

class ScrollEvent : public Event {
public:
	ScrollEvent(int x, int y, float delta): Event(EventType::SCROLL_EVENT), m_delta(delta), m_x(x), m_y(y) {};

	const char* getName() const override { return "Scroll Event"; }

	int getX() const { return m_x; }
	int getY() const { return m_y; }
	float getDelta() const { return m_delta; }
private:
	int m_x;		// X position of the mouse pointer, relative to the left of the owner window.
	int m_y;		// Y position of the mouse pointer, relative to the top of the owner window.
	float m_delta;	// Number of ticks the wheel has moved (positive is up, negative is down)
};

enum MouseEdge {
	None = 0,
	Left = 1 << 0,  // 0001
	Right = 1 << 1,  // 0010
	Top = 1 << 2,  // 0100
	Bottom = 1 << 3   // 1000
};

class MouseOnEdgeEvent : public Event {
public:
	MouseOnEdgeEvent(int edges) : Event(EventType::CURSOR_ON_EDGE_EVENT), m_edges(edges) {}

	const char* getName() const override { return "Cursor on Edge Event"; }
	int getEdges() const { return m_edges; }

private:
	int m_edges;
};


class EventDispatcher {
public:
	template <typename T>
	using EventFn = std::function<void(T&)>;

	template <typename T>
	void subscribe(void* entity, const EventFn<T>& subscriber, RenderLayer layer = RenderLayer::MAP) {
		std::vector<Subscription>& subscribers = m_subscriptions[std::type_index(typeid(T))];
		subscribers.push_back({
			entity,
			static_cast<int>(layer), 
			[subscriber](Event& event) {
				subscriber(static_cast<T&>(event));
			}
			});

		std::sort(subscribers.begin(), subscribers.end(),
			[](const Subscription& a, const Subscription& b) {
				return a.layer < b.layer;
			});
	}

	void unsubscribe(void* ref) {
		for (auto it = m_subscriptions.begin(); it != m_subscriptions.end();) {
			std::vector<Subscription>& subscriptions = it->second;
			subscriptions.erase(
				std::remove_if(
					subscriptions.begin(),
					subscriptions.end(),
					[ref](const Subscription& sub) {
						return sub.subscriber == ref;
					}
				),
				subscriptions.end()
			);

			if (subscriptions.empty()) {
				it = m_subscriptions.erase(it);
			}
			else {
				++it;
			}
		}
	}

	void emit(Event& event) {
		auto it = m_subscriptions.find(std::type_index(typeid(event)));
		if (it != m_subscriptions.end()) {
			for (Subscription& subscription : it->second) {
				if (event.m_handled) break;
				subscription.callback(event);
			}
		}
	}

private:
	struct Subscription {
		void* subscriber;
		int layer;
		std::function<void(Event&)> callback;
	};

	std::unordered_map<std::type_index, std::vector<Subscription>> m_subscriptions;
};


