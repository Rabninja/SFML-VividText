#pragma once

#ifndef SFV_CHUNK_H
#define SFV_CHUNK_H

#include <SFML/Graphics/Font.hpp>
#include <optional>

namespace sfv {

	struct Chunk {
		std::size_t index;
		std::size_t length;
		std::size_t verticeLength;
		std::size_t outlineLength;
		sf::Color fillColor;
		sf::Color outlineColor;
		sf::Color lineColor;
		sf::Uint32 style;
		sf::Uint32 characterSize;
		const sf::Font* font;
		float outlineThickness;

		Chunk();

		Chunk(std::size_t index_, std::size_t length_, const sf::Font* font_ = nullptr);

		Chunk(const Chunk& chunk) noexcept;

		Chunk(Chunk&& chunk) noexcept;

		Chunk& operator=(const Chunk& chunk) noexcept;

		Chunk& operator=(Chunk&& chunk) noexcept;

		float getHeight() const;

		bool operator==(const Chunk& chunk) const;

		bool operator!=(const Chunk& chunk) const;
	};

	struct ChunkBuilder {
		std::size_t length;
		const sf::Font* font;
		std::optional<sf::Color> fillColor;
		std::optional<sf::Color> outlineColor;
		std::optional<sf::Color> lineColor;
		std::optional<sf::Uint32> style;
		std::optional<sf::Uint32> characterSize;
		std::optional<float> outlineThickness;

		ChunkBuilder(std::size_t length);
		ChunkBuilder() = default;
		ChunkBuilder(ChunkBuilder&&) = default;

		ChunkBuilder& fill(sf::Color color);

		ChunkBuilder& outline(sf::Color color, float thickness = 0.f);

		ChunkBuilder& stylize(sf::Uint32 style_);

		ChunkBuilder& fontType(const sf::Font& font_);

		ChunkBuilder& charSize(sf::Uint32 size);
	};
}
#endif
