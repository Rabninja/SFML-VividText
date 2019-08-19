#pragma once

#ifndef SFV_SMART_TEXT_H
#define SFV_SMART_TEXT_H

#include <SFML\Graphics\Text.hpp>
#include <vector>
#include "Chunk.h"

namespace sfv {
	class VividText : public sf::Drawable, public sf::Transformable
	{
	private:
		//Deque for text objects and one whole string
		//Deque for text Data objects to hold information and one whole vertex array
		mutable bool m_needsUpdate;
		const sf::Font* m_font;
		sf::String m_string;
		mutable sf::FloatRect m_bounds;
		mutable std::vector<sfv::Chunk> m_chunks;
		mutable std::vector<sf::Vertex> m_vertices;
		mutable std::vector<sf::Vertex> m_outlineVertices;
	public:
		VividText(const sf::String& text, const sf::Font& font);
		VividText();
		~VividText();

		void setStyle(sf::Uint32 style);

		void setStyle(sf::Uint32 style, std::size_t start);

		void setStyle(sf::Uint32 style, std::size_t start, std::size_t length);

		void setFillColor(sf::Color color);

		void setFillColor(sf::Color color, std::size_t start);

		void setFillColor(sf::Color color, std::size_t start, std::size_t length);

		void setOutlineColor(sf::Color color);

		void setOutlineColor(sf::Color color, std::size_t start);

		void setOutlineColor(sf::Color color, std::size_t start, std::size_t length);

		void setOutlineThickness(float thickness);

		void setOutlineThickness(float thickness, std::size_t start);

		void setOutlineThickness(float thickness, std::size_t start, std::size_t length);

		void setFont(const sf::Font& font);

		void setFont(const sf::Font& font, std::size_t start);

		void setFont(const sf::Font& font, std::size_t start, std::size_t length);

		void setCharacterSize(sf::Uint32 charSize);

		void setCharacterSize(sf::Uint32 charSize, std::size_t start);

		void setCharacterSize(sf::Uint32 charSize, std::size_t start, std::size_t length);

		void setProperties(const ChunkBuilder& data);

		void setProperties(const ChunkBuilder& data, std::size_t start);

		void setProperties(const ChunkBuilder& data, std::size_t start, std::size_t length);



		void setString(const sf::String& text);



		const sf::String& getString() const;

		sf::FloatRect getLocalBounds() const;

		sf::FloatRect getGlobalBounds() const;

		const Chunk& getChunk(std::size_t index) const;

		const std::size_t getChunkIndex(std::size_t subIndex) const;

		sf::Vector2f findLocalCharacterPos(std::size_t subIndex) const;

		sf::Vector2f findGlobalCharacterPos(std::size_t subIndex) const;

		void insert(const sf::String& text, std::size_t index, bool left);

		void insert(const sf::String& text, std::size_t index);

		void erase(std::size_t start, std::size_t length = 1);

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void ensureGeometryUpdate() const;

		void updateChunks(std::size_t start);

		void insertChunk(std::size_t subIndex, const Chunk& chunk);

		void replaceChunk(std::size_t subIndex, const ChunkBuilder& chunk);

		void eraseChunk(std::size_t subIndex, std::size_t length);
	};
}

#endif