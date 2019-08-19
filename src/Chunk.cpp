#include "Chunk.h"
#include <SFML/Graphics/Text.hpp>
sfv::Chunk::Chunk()
	: Chunk(static_cast<int>(-1), 0)
{
}

sfv::Chunk::Chunk(std::size_t index_, std::size_t length_, const sf::Font* font_)
	: index(index_),
	length(length_),
	fillColor(sf::Color::White),
	outlineColor(sf::Color::Black),
	style(sf::Text::Style::Regular),
	characterSize(18),
	outlineThickness(0.f),
	font(font_),
	verticeLength(0),
	outlineLength(0)
{
}

sfv::Chunk::Chunk(const Chunk& chunk) noexcept
	: index(chunk.index),
	length(chunk.length),
	verticeLength(chunk.verticeLength),
	fillColor(chunk.fillColor),
	outlineColor(chunk.outlineColor),
	lineColor(chunk.lineColor),
	style(chunk.style),
	characterSize(chunk.characterSize),
	font(chunk.font),
	outlineThickness(chunk.outlineThickness)
{

}

sfv::Chunk::Chunk(Chunk&& chunk) noexcept
	: index(std::move(chunk.index)),
	length(std::move(chunk.length)),
	verticeLength(std::move(chunk.verticeLength)),
	fillColor(std::move(chunk.fillColor)),
	outlineColor(std::move(chunk.outlineColor)),
	lineColor(std::move(chunk.lineColor)),
	style(std::move(chunk.style)),
	characterSize((chunk.characterSize)),
	font(std::move(chunk.font)),
	outlineThickness(std::move(chunk.outlineThickness))
{

}

sfv::Chunk& sfv::Chunk::operator=(const Chunk& chunk) noexcept
{
	index = chunk.index;
	length = chunk.length;
	verticeLength = chunk.verticeLength;
	fillColor = chunk.fillColor;
	outlineColor = chunk.outlineColor;
	lineColor = chunk.lineColor;
	style = chunk.style;
	characterSize = chunk.characterSize;
	font = chunk.font;
	outlineThickness = chunk.outlineThickness;

	return *this;
}

sfv::Chunk& sfv::Chunk::operator=(Chunk&& chunk) noexcept
{
	index = std::move(chunk.index);
	length = std::move(chunk.length);
	verticeLength = std::move(chunk.verticeLength);
	fillColor = std::move(chunk.fillColor);
	outlineColor = std::move(chunk.outlineColor);
	lineColor = std::move(chunk.lineColor);
	style = std::move(chunk.style);
	characterSize = std::move(chunk.characterSize);
	font = std::move(chunk.font);
	outlineThickness = std::move(chunk.outlineThickness);

	return *this;
}

float sfv::Chunk::getHeight() const
{
	return font->getLineSpacing(characterSize);
}

bool sfv::Chunk::operator==(const Chunk& chunk) const
{
	return fillColor == chunk.fillColor && outlineColor == chunk.outlineColor && outlineThickness == chunk.outlineThickness
		   &&  style == chunk.style     && characterSize == chunk.characterSize && font == chunk.font;
}

bool sfv::Chunk::operator!=(const Chunk& chunk) const
{
	return !(this->operator==(chunk));
}

sfv::ChunkBuilder::ChunkBuilder(std::size_t length)
	:length(length),
	font(nullptr) {
}

sfv::ChunkBuilder& sfv::ChunkBuilder::fill(sf::Color color)
{
	fillColor = color;
	return *this;
}

sfv::ChunkBuilder& sfv::ChunkBuilder::outline(sf::Color color, float thickness)
{
	outlineColor = color;
	outlineThickness.emplace(thickness);
	return *this;
}

sfv::ChunkBuilder& sfv::ChunkBuilder::stylize(sf::Uint32 style_)
{
	style.emplace(style_);
	return *this;
}

sfv::ChunkBuilder& sfv::ChunkBuilder::fontType(const sf::Font& font_)
{
	font = &font_;
	return *this;
}

sfv::ChunkBuilder& sfv::ChunkBuilder::charSize(sf::Uint32 size)
{
	characterSize.emplace(size);
	return *this;
}
