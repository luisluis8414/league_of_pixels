#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <typeindex>
#include <unordered_map>

class Entity;
class Projectile;

enum class RenderLayer { BACKGROUND = -1, MAP, BUILDINGS, ENTITIES, PLAYER, TEXT, UI };

enum class EventType {
  INIT,
  DRAW,
  SECOND,
  CLEAN_UP,
  TICK,
  GAME_OVER,
  DESTROY_ENTITY,
  DESTROY_PROJECTILE,
  KEYPRESSED,
  MOUSE_CLICKED,
  REGISER_PROJECTILE,
  ACTION_EVENT,
  CURSOR_ON_EDGE_EVENT,
  ABILITY_DMG_EVENT,
};

class Event {
  friend class EventDispatcher;

 public:
  virtual const std::string getName() const = 0;

  virtual EventType getEventType() const {
    return m_eventType;
  };

 protected:
  Event(EventType type) : m_eventType(type) {
  }
  EventType m_eventType;
};

class InitEvent : public Event {
 public:
  InitEvent() : Event(EventType::INIT) {};

  ~InitEvent() = default;

  const std::string getName() const override {
    return "Init Event";
  }
};

class DrawEvent : public Event {
 public:
  DrawEvent(sf::RenderWindow& window) : Event(EventType::DRAW), m_window(window) {};

  ~DrawEvent() = default;

  sf::RenderWindow& getWindow() const {
    return m_window;
  }
  const std::string getName() const override {
    return "Draw Event";
  }

 private:
  sf::RenderWindow& m_window;
};

class SecondsEvent : public Event {
 public:
  SecondsEvent() : Event(EventType::SECOND) {};

  ~SecondsEvent() = default;

  const std::string getName() const override {
    return "Second Event";
  }
};

class CleanUpEvent : public Event {
 public:
  CleanUpEvent() : Event(EventType::CLEAN_UP) {};

  ~CleanUpEvent() = default;

  const std::string getName() const override {
    return "CleanUp Event";
  }
};

class TickEvent : public Event {
 public:
  TickEvent(const float& deltaTime) : Event(EventType::TICK), m_detlaTime(deltaTime) {};

  ~TickEvent() = default;

  const float getDeltaTime() const {
    return m_detlaTime;
  }
  const std::string getName() const override {
    return "Update Event";
  }

 private:
  const float m_detlaTime;
};

class KeyPressedEvent : public Event {
 public:
  KeyPressedEvent(const sf::Keyboard::Key key_event, sf::Vector2f mousePosition)
      : Event(EventType::KEYPRESSED), m_keyEvent(key_event), m_mousePosition(mousePosition) {};

  ~KeyPressedEvent() = default;

  const std::string getName() const override {
    return "Key Pressed Event";
  }

  const sf::Keyboard::Key& getKeyboardEvent() const {
    return m_keyEvent;
  }
  const sf::Vector2f getMousePosition() const {
    return m_mousePosition;
  }

 private:
  const sf::Keyboard::Key m_keyEvent;
  const sf::Vector2f m_mousePosition;
};

class MouseRightClickEvent : public Event {
 public:
  MouseRightClickEvent(const sf::Vector2f& position) : Event(EventType::MOUSE_CLICKED), m_position(position) {};

  ~MouseRightClickEvent() = default;

  const sf::Vector2f& getPosition() const {
    return m_position;
  }
  const std::string getName() const override {
    return "Left Mouse Click Event";
  }

 private:
  const sf::Vector2f& m_position;
};

class GameOverEvent : public Event {
 public:
  GameOverEvent() : Event(EventType::GAME_OVER) {};

  ~GameOverEvent() = default;

  const std::string getName() const override {
    return "Key Pressed Event";
  }
};

class DestroyEntityEvent : public Event {
 public:
  DestroyEntityEvent(Entity* entity) : Event(EventType::DESTROY_ENTITY), m_entity(entity) {
  }

  ~DestroyEntityEvent() = default;

  const std::string getName() const override {
    return "Destroy Entity Event";
  }
  Entity* getEntity() const {
    return m_entity;
  }

 private:
  Entity* m_entity;
};

class DestroyProjectileEvent : public Event {
 public:
  DestroyProjectileEvent(Projectile* projectile) : Event(EventType::DESTROY_PROJECTILE), m_projectile(projectile) {
  }

  ~DestroyProjectileEvent() = default;

  const std::string getName() const override {
    return "Destroy Projectile Event";
  }
  Projectile* getProjectile() const {
    return m_projectile;
  }

 private:
  Projectile* m_projectile;
};

class RegisterProjectileEvent : public Event {
 public:
  RegisterProjectileEvent(std::shared_ptr<Projectile> projectile)
      : Event(EventType::REGISER_PROJECTILE), m_projectile(projectile) {};

  ~RegisterProjectileEvent() = default;

  const std::string getName() const override {
    return "Register Projectile Event";
  }

  const std::shared_ptr<Projectile> getProjectile() {
    return m_projectile;
  }

 private:
  std::shared_ptr<Projectile> m_projectile;
};

enum class ActionEventType { TargetAction, MoveAction };

class ActionEvent : public Event {
 public:
  ActionEvent(const ActionEventType actionType, const Entity& targeter, Entity* target, const sf::Vector2f& position)
      : Event(EventType::ACTION_EVENT),
        m_actor(targeter),
        m_target(target),
        m_destination(position),
        m_actionType(actionType) {
  }

  ~ActionEvent() = default;

  const std::string getName() const override {
    return "Action Event";
  }

  const Entity& getTargeter() const {
    return m_actor;
  }
  Entity* getTarget() const {
    return m_target;
  }
  const sf::Vector2f& getPosition() const {
    return m_destination;
  }
  ActionEventType getActionType() const {
    return m_actionType;
  }

 private:
  const Entity& m_actor;
  Entity* m_target;
  const sf::Vector2f& m_destination;
  ActionEventType m_actionType;
};

class AbilityDmgEvent : public Event {
 public:
  AbilityDmgEvent(sf::FloatRect hitbox, float spellDmg)
      : Event(EventType::ABILITY_DMG_EVENT), m_Spelldmg(spellDmg), m_hitbox(hitbox) {
  }

  ~AbilityDmgEvent() = default;

  const std::string getName() const override {
    return "Ability Dmg Event";
  }

  const sf::FloatRect getHitbox() const {
    return m_hitbox;
  }
  const float getSpellDmg() const {
    return m_Spelldmg;
  }

 private:
  sf::FloatRect m_hitbox;
  float m_Spelldmg;
};

enum MouseEdge {
  None = 0,
  Left = 1 << 0,   // 0001
  Right = 1 << 1,  // 0010
  Top = 1 << 2,    // 0100
  Bottom = 1 << 3  // 1000
};

class MouseOnEdgeEvent : public Event {
 public:
  MouseOnEdgeEvent(int edges) : Event(EventType::CURSOR_ON_EDGE_EVENT), m_edges(edges) {
  }

  ~MouseOnEdgeEvent() = default;

  const std::string getName() const override {
    return "Cursor on Edge Event";
  }
  int getEdges() const {
    return m_edges;
  }

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
    subscribers.push_back(
        {entity, static_cast<int>(layer), [subscriber](Event& event) { subscriber(static_cast<T&>(event)); }});

    std::sort(subscribers.begin(), subscribers.end(), [](const Subscription& a, const Subscription& b) {
      return a.layer < b.layer;
    });
  }

  void unsubscribe(void* ref) {
    m_pendingUnsubscribes.push_back(ref);
  }

  void emit(Event& event) {
    auto it = m_subscriptions.find(std::type_index(typeid(event)));
    if (it != m_subscriptions.end()) {
      for (Subscription& subscription : it->second) {
        subscription.callback(event);
      }
    }

    processPendingUnsubscribes();
  }

 private:
  struct Subscription {
    void* subscriber;
    int layer;
    std::function<void(Event&)> callback;
  };

  std::unordered_map<std::type_index, std::vector<Subscription>> m_subscriptions;
  std::vector<void*> m_pendingUnsubscribes;

  void processPendingUnsubscribes() {
    for (void* ref : m_pendingUnsubscribes) {
      for (auto it = m_subscriptions.begin(); it != m_subscriptions.end();) {
        std::vector<Subscription>& subscriptions = it->second;
        subscriptions.erase(std::remove_if(subscriptions.begin(),
                                           subscriptions.end(),
                                           [ref](const Subscription& sub) { return sub.subscriber == ref; }),
                            subscriptions.end());

        if (subscriptions.empty()) {
          it = m_subscriptions.erase(it);
        } else {
          ++it;
        }
      }
    }
    m_pendingUnsubscribes.clear();
  }
};
