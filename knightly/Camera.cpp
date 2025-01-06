#include "Camera.h"
#include "Config.h"
#include <iostream>

Camera::Camera(EventDispatcher& dispatcher, sf::RenderWindow& window) : m_window(window) {
	dispatcher.subscribe<ScrollEvent>(this, [this](ScrollEvent& event) {
		handleScroll(event.getX(), event.getY(), event.getDelta());
		});

    dispatcher.subscribe<MouseOnEdgeEvent>(this, [this](MouseOnEdgeEvent& event) {
        handleCursorOnEdge(event.getEdges());
       });
}

void Camera::handleScroll(int x, int y, float delta) {
    if (delta == 0) return;

    sf::View currentView = m_window.getView();
    float zoomFactor = (delta > 0) ? 0.9f : 1.1f;

    sf::Vector2f originalSize = m_window.getDefaultView().getSize();
    sf::Vector2f currentSize = currentView.getSize();

    sf::Vector2f newSize = currentSize * zoomFactor;

    newSize.x = std::clamp(newSize.x, originalSize.x * Config::Camera::MIN_ZOOM, originalSize.x * Config::Camera::MAX_ZOOM);
    newSize.y = std::clamp(newSize.y, originalSize.y * Config::Camera::MIN_ZOOM, originalSize.y * Config::Camera::MAX_ZOOM);

    if (delta > 0 && newSize.x != currentSize.x) {
        sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Vector2i(x, y));
        sf::Vector2f currentCenter = currentView.getCenter();
        sf::Vector2f newCenter = mousePos + (currentCenter - mousePos) * zoomFactor;

        currentView.setCenter(newCenter);
    }

    currentView.setSize(newSize);

    m_window.setView(currentView);
}

void Camera::handleCursorOnEdge(int edgeMask) {
    sf::View currentView= m_window.getView();

    float scrollSpeed = Config::Camera::SCOLLSPEED;

    if (edgeMask & MouseEdge::Left) {
        currentView.move(-scrollSpeed, 0);
    }

    if (edgeMask & MouseEdge::Right) {
        currentView.move(scrollSpeed, 0);
    }

    if (edgeMask & MouseEdge::Top) {
        currentView.move(0, -scrollSpeed);
    }

    if (edgeMask & MouseEdge::Bottom) {
        currentView.move(0, scrollSpeed);
    }

    m_window.setView(currentView);
}

	
