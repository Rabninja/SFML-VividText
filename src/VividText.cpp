#include "VividText.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <cmath>

////////////////////////////////////////////////////////////
// Source Author: Laurent Gomila
// Taken from SFML-2.4.1 Text.cpp
// Edits made to the source:
//    -Modified addLine & addGlyphQuad to work with std::vector<sf::Vertex>
//    -Modified ensureGeometryUpdate to work with varying styles, fonts, and sizes.
////////////////////////////////////////////////////////////
namespace
{
	// Add an underline or strikethrough line to the vertex array
	void addLine(float xOffset, std::vector<sf::Vertex>& vertices, float lineLength, float lineTop, const sf::Color& color, float offset, float thickness, float outlineThickness = 0)
	{
		const sf::Vector2f texCoords(1.f, 1.f);
		float top = std::roundf(lineTop + offset - (thickness / 2) + 0.5f);
		float bottom = top + std::floor(thickness + 0.5f);

		vertices.emplace_back(sf::Vector2f(-outlineThickness + xOffset, top - outlineThickness), color, texCoords);
		vertices.emplace_back(sf::Vector2f(lineLength + outlineThickness + xOffset, top - outlineThickness), color, texCoords);
		vertices.emplace_back(sf::Vector2f(-outlineThickness + xOffset, bottom + outlineThickness), color, texCoords);
		vertices.emplace_back(sf::Vector2f(-outlineThickness + xOffset, bottom + outlineThickness), color, texCoords);
		vertices.emplace_back(sf::Vector2f(lineLength + outlineThickness + xOffset, top - outlineThickness), color, texCoords);
		vertices.emplace_back(sf::Vector2f(lineLength + outlineThickness + xOffset, bottom + outlineThickness), color, texCoords);
	}

	// Add a glyph quad to the vertex array
	void addGlyphQuad(std::vector<sf::Vertex>& vertices, sf::Vector2f position, const sf::Color& color, const sf::Glyph& glyph, float italic, float outlineThickness = 0)
	{
		const float left = glyph.bounds.left;
		const float top = glyph.bounds.top;
		const float right = glyph.bounds.left + glyph.bounds.width;
		const float bottom = glyph.bounds.top + glyph.bounds.height;

		const float u1 = static_cast<float>(glyph.textureRect.left);
		const float v1 = static_cast<float>(glyph.textureRect.top);
		const float u2 = static_cast<float>(glyph.textureRect.left + glyph.textureRect.width);
		const float v2 = static_cast<float>(glyph.textureRect.top + glyph.textureRect.height);
		vertices.emplace_back(sf::Vector2f(position.x + left - italic * top - outlineThickness, position.y + top - outlineThickness), color, sf::Vector2f(u1, v1));
		vertices.emplace_back(sf::Vector2f(position.x + right - italic * top - outlineThickness, position.y + top - outlineThickness), color, sf::Vector2f(u2, v1));
		vertices.emplace_back(sf::Vector2f(position.x + left - italic * bottom - outlineThickness, position.y + bottom - outlineThickness), color, sf::Vector2f(u1, v2));
		vertices.emplace_back(sf::Vector2f(position.x + left - italic * bottom - outlineThickness, position.y + bottom - outlineThickness), color, sf::Vector2f(u1, v2));
		vertices.emplace_back(sf::Vector2f(position.x + right - italic * top - outlineThickness, position.y + top - outlineThickness), color, sf::Vector2f(u2, v1));
		vertices.emplace_back(sf::Vector2f(position.x + right - italic * bottom - outlineThickness, position.y + bottom - outlineThickness), color, sf::Vector2f(u2, v2));
	}

	const std::size_t NULL_INDEX = static_cast<std::size_t>(-1);
}
sfv::VividText::VividText(const sf::String& text, const sf::Font& font)
	: m_font(&font),
	m_vertices(sf::PrimitiveType::Triangles)
{
	setString(text);
}

sfv::VividText::VividText()
{

}

sfv::VividText::~VividText()
{
}

void sfv::VividText::setStyle(sf::Uint32 style)
{
	setStyle(style, 0U, m_string.getSize());
}

void sfv::VividText::setStyle(sf::Uint32 style, std::size_t start)
{
	setStyle(style, start, m_string.getSize() - start);
}

void sfv::VividText::setStyle(sf::Uint32 style, std::size_t start, std::size_t length)
{
	ChunkBuilder chunk(length);
	chunk.stylize(style);

	replaceChunk(start, chunk);
}

void sfv::VividText::setFillColor(sf::Color color)
{
	setFillColor(color, 0U, m_string.getSize());
}

void sfv::VividText::setFillColor(sf::Color color, std::size_t start)
{
	setFillColor(color, start, m_string.getSize() - start);
}

void sfv::VividText::setFillColor(sf::Color color, std::size_t start, std::size_t length)
{
	ChunkBuilder chunk(length);
	chunk.fill(color);

	replaceChunk(start, chunk);
}

void sfv::VividText::setOutlineColor(sf::Color color)
{
	setOutlineColor(color, 0U, m_string.getSize());
}

void sfv::VividText::setOutlineColor(sf::Color color, std::size_t start)
{
	setOutlineColor(color, start, m_string.getSize() - start);
}

void sfv::VividText::setOutlineColor(sf::Color color, std::size_t start, std::size_t length)
{
	ChunkBuilder chunk(length);
	chunk.outline(color);

	replaceChunk(start, chunk);
}

void sfv::VividText::setOutlineThickness(float thickness)
{
	setOutlineThickness(thickness, 0U, m_string.getSize());
}

void sfv::VividText::setOutlineThickness(float thickness, std::size_t start)
{
	setOutlineThickness(thickness, start, m_string.getSize() - start);
}

void sfv::VividText::setOutlineThickness(float thickness, std::size_t start, std::size_t length)
{
	ChunkBuilder chunk(length);
	chunk.outlineThickness = thickness;

	replaceChunk(start, chunk);
}

void sfv::VividText::setFont(const sf::Font& font)
{
	setFont(font, 0U, m_string.getSize());
}

void sfv::VividText::setFont(const sf::Font& font, std::size_t start)
{
	setFont(font, start, m_string.getSize() - start);
}

void sfv::VividText::setFont(const sf::Font& font, std::size_t start, std::size_t length)
{
	ChunkBuilder chunk(length);
	chunk.fontType(font);

	replaceChunk(start, chunk);
}

void sfv::VividText::setCharacterSize(sf::Uint32 charSize)
{
	setCharacterSize(charSize, 0U, m_string.getSize());
}

void sfv::VividText::setCharacterSize(sf::Uint32 charSize, std::size_t start)
{
	setCharacterSize(charSize, start, m_string.getSize() - start);
}

void sfv::VividText::setCharacterSize(sf::Uint32 charSize, std::size_t start, std::size_t length)
{
	ChunkBuilder chunk(length);
	chunk.charSize(charSize);

	replaceChunk(start, chunk);
}

void sfv::VividText::setProperties(const ChunkBuilder& data)
{
	setProperties(data, 0, m_string.getSize());
}

void sfv::VividText::setProperties(const ChunkBuilder& data, std::size_t start)
{
	setProperties(data, 0, m_string.getSize() - start);
}

void sfv::VividText::setProperties(const ChunkBuilder& data, std::size_t start, std::size_t length)
{
	replaceChunk(start, data);
}

void sfv::VividText::setString(const sf::String& text)
{
	m_string = "";
	insert(text, 0);
}

const sf::String& sfv::VividText::getString() const
{
	return m_string;
}

const  sfv::Chunk& sfv::VividText::getChunk(std::size_t index) const
{
	return m_chunks[index];
}

const std::size_t sfv::VividText::getChunkIndex(std::size_t subIndex) const
{
	const std::size_t chunkSize = m_chunks.size();
	std::size_t chunkIndex = 0;

	for (std::size_t chunk = 0; chunk != chunkSize; ++chunk) {
		if (subIndex < chunkIndex + m_chunks[chunk].length) {
			return chunk;
		}
		else {
			chunkIndex += m_chunks[chunk].length;
		}
	}
	return NULL_INDEX;
}

sf::Vector2f sfv::VividText::findLocalCharacterPos(std::size_t subIndex) const
{
	sf::Vector2f position;

	std::size_t offset = 0U;
	sf::Uint32 previous = 0U;
	for (const auto& chunk : m_chunks) {
		const sf::Font* font = chunk.font;
		const sf::Uint32 characterSize = chunk.characterSize;
		const bool bold = (chunk.style & sf::Text::Style::Bold) != 0;
		const float space = font->getGlyph(L' ', characterSize, bold).advance;
		const float vSpace = font->getLineSpacing(characterSize);

		for (std::size_t index = 0U; index != chunk.length; ++index) {
			const auto& current = m_string[offset + index];
			position.x += font->getKerning(previous, current, characterSize);

			switch (current)
			{
			case ' ':
				position.x = space;
				break;
			case '\t':
				position.x += space * 4.f;
				break;
			case '\n':
				position.y += vSpace;
				break;
			default:
				position.x += font->getGlyph(current, characterSize, bold).advance;
				break;
			}
			previous = current;
		}
		offset += chunk.length;
	}
	return position;
}


sf::FloatRect sfv::VividText::getLocalBounds() const
{
	ensureGeometryUpdate();
	return m_bounds;
}

sf::FloatRect sfv::VividText::getGlobalBounds() const
{
	ensureGeometryUpdate();
	return getTransform().transformRect(m_bounds);
}

sf::Vector2f sfv::VividText::findGlobalCharacterPos(std::size_t subIndex) const
{
	return getTransform().transformPoint(findLocalCharacterPos(subIndex));
}

void sfv::VividText::insert(const sf::String& text, std::size_t index, bool left)
{

}

void sfv::VividText::insert(const sf::String& text, std::size_t index)
{
	//first in last out container
	m_string.insert(index, text);

	insertChunk(index, Chunk(index, text.getSize(), m_font));
	m_needsUpdate = true;
}

void sfv::VividText::erase(std::size_t start, std::size_t length)
{
	m_string.erase(start, length);

	eraseChunk(start, length);
}

void sfv::VividText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_string.isEmpty()) {
		return;
	}
	ensureGeometryUpdate();
	states.transform *= getTransform();

	if (m_vertices.empty()) {
		return;
	}

	const sf::Vertex* vertices = m_vertices.data();
	const sf::Vertex* outline = m_outlineVertices.data();
	const sf::Font* font = m_chunks.front().font;
	std::size_t characterSize = m_chunks.front().characterSize;
	std::size_t previous = 0;
	std::size_t vertexLength = 0;
	std::size_t prevOutline = 0;
	std::size_t outlineLength = 0;
	for (auto& chunk : m_chunks) {
		if (font != chunk.font || characterSize != chunk.characterSize) {
			states.texture = &font->getTexture(characterSize);
			target.draw(outline + prevOutline, outlineLength, sf::PrimitiveType::Triangles, states);
			font = chunk.font;
			characterSize = chunk.characterSize;
			prevOutline += outlineLength;
			outlineLength = 0;
		}
		outlineLength += chunk.outlineLength;
	}
	states.texture = &font->getTexture(characterSize);
	target.draw(outline + prevOutline, outlineLength, sf::PrimitiveType::Triangles, states);
	font = m_chunks.front().font;
	characterSize = m_chunks.front().characterSize;

	for (auto& chunk : m_chunks) {
		if (font != chunk.font || characterSize != chunk.characterSize) {
			states.texture = &font->getTexture(characterSize);
			target.draw(vertices + previous, vertexLength, sf::PrimitiveType::Triangles, states);
			font = chunk.font;
			characterSize = chunk.characterSize;
			previous += vertexLength;
			vertexLength = 0;
		}
		vertexLength += chunk.verticeLength;
	}
	states.texture = &font->getTexture(characterSize);
	target.draw(vertices + previous, vertexLength, sf::PrimitiveType::Triangles, states);
}

void sfv::VividText::eraseChunk(std::size_t subIndex, std::size_t length)
{
	if (m_chunks.empty()) {
		return;
	}
	const std::size_t start = getChunkIndex(subIndex);
	const std::size_t end = getChunkIndex(subIndex + length);
	auto chunkIter = m_chunks.begin() + end;

	if (start != end) {
		m_chunks[start].length -= (m_chunks[start].index + m_chunks[start].length) - subIndex;
		m_chunks[end].length -= m_chunks[end].length - ((m_chunks[end].index + m_chunks[start].length) - (subIndex + length));
		m_chunks[end].index = subIndex;
		chunkIter = m_chunks.erase(m_chunks.begin() + start + (m_chunks[start].length != 0), chunkIter);
		if (chunkIter != m_chunks.end()) {
			++chunkIter;
		}
	}

	const auto endChunk = m_chunks.end();
	for (; chunkIter != endChunk; ++chunkIter) {
		chunkIter->index -= length;
	}
	updateChunks(start);
	m_needsUpdate = true;
}

void sfv::VividText::insertChunk(std::size_t subIndex, const Chunk& chunk)
{
	m_needsUpdate = true;

	if (m_chunks.empty()) {
		m_chunks.emplace_back(chunk);
		return;
	}
	const std::size_t start = getChunkIndex(subIndex);
	auto chunkIter = m_chunks.begin() + start;
	if (chunk == m_chunks[start]) {
		m_chunks[start].length += chunk.length;
		++chunkIter;
	}
	else {
		const std::size_t splicedSize = (chunkIter->index + chunkIter->length) - subIndex;
		Chunk splicedChunk = *chunkIter;
		chunkIter->length -= splicedSize;
		splicedChunk.index = chunkIter->index + chunk.length + chunkIter->length;
		splicedChunk.length = splicedSize;

		if (chunkIter->length == 0) {
			*chunkIter = chunk;
			++chunkIter;
			m_chunks.insert(chunkIter++, splicedChunk);
		}
		else {
			++chunkIter;
			m_chunks.insert(chunkIter++, chunk);
			m_chunks.insert(chunkIter++, splicedChunk);
		}
	}
	const auto endChunk = m_chunks.end();
	for (; chunkIter != endChunk; ++chunkIter) {
		chunkIter->index += chunk.length;
	}
}

void sfv::VividText::replaceChunk(std::size_t subIndex, const ChunkBuilder& chunkData)
{
	const std::size_t start = getChunkIndex(subIndex);
	const std::size_t end = getChunkIndex(subIndex + chunkData.length);

	if (start == NULL_INDEX) {
		return;
	}

	auto chunkIter = m_chunks.begin() + start;
	const auto startChunk = *chunkIter;
	const auto endChunk = end != NULL_INDEX ? m_chunks[end] : startChunk;
	const auto splicedSize = (chunkIter->index + chunkIter->length) - subIndex;

	std::size_t offset = 0;
	if (splicedSize != startChunk.length) {
		chunkIter->length -= splicedSize;
		chunkIter = m_chunks.insert(chunkIter + 1, startChunk);
		chunkIter->index = subIndex;
		++offset;
	}
	if (chunkData.length < splicedSize) {
		chunkIter->length = chunkData.length;
		chunkIter = m_chunks.insert(chunkIter + 1, startChunk);
		chunkIter->index = subIndex + chunkData.length;
		chunkIter->length = (startChunk.index + startChunk.length) - (subIndex + chunkData.length);
		++offset;
	}
	else {
		chunkIter->length = splicedSize;
	}
	if (end != -1 && start != end && subIndex + chunkData.length != endChunk.index) {
		const std::size_t endSplice = (endChunk.index + endChunk.length) - (subIndex + chunkData.length);
		chunkIter = m_chunks.insert(chunkIter + (end - start), endChunk); //std::prev(m_chunks.begin() + end + offset)
		chunkIter->length -= endSplice;

		auto endIter = std::next(chunkIter);
		endIter->index += chunkIter->length;
		endIter->length = endSplice;
		++offset;
	}

	const bool font = chunkData.font != nullptr;
	for (std::size_t index = start + (splicedSize != startChunk.length), stop = (end == -1 ? m_chunks.size() : end + offset); index < stop; ++index) {
		auto& chunk = m_chunks[index];
		if (font) {
			chunk.font = chunkData.font;
		}
		chunk.outlineThickness = chunkData.outlineThickness.value_or(chunk.outlineThickness);
		chunk.characterSize = chunkData.characterSize.value_or(chunk.characterSize);
		chunk.outlineColor = chunkData.outlineColor.value_or(chunk.outlineColor);
		chunk.fillColor = chunkData.fillColor.value_or(chunk.fillColor);
		chunk.style = chunkData.style.value_or(chunk.style);
	}
	updateChunks(start);
	m_needsUpdate = true;
}

void sfv::VividText::updateChunks(std::size_t start)
{
	auto last = m_chunks.end();
	auto first = std::next(m_chunks.begin(), start + 1);
	auto result = first;
	auto previous = std::prev(result);

	while (first != last) {
		if (*previous != *first) {
			if (first != result) {
				*result = std::move(*first);
			}
			previous = result;
			++result;
		}
		else {
			previous->length += first->length;
		}
		++first;
	}

	m_chunks.erase(result, m_chunks.end());
}


void sfv::VividText::ensureGeometryUpdate() const
{
	// Do nothing, if geometry has not changed
	if (!m_needsUpdate)
		return;
	// Mark geometry as updated
	m_needsUpdate = false;

	// Clear the previous geometry
	m_vertices.clear();
	m_outlineVertices.clear();
	m_bounds = sf::FloatRect();

	if (m_string.isEmpty()) {
		return;
	}
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float maxX = 0.f;
	float maxY = 0.f;

	const auto getMaximum = [&](std::size_t start, auto fn)
	{
		const std::size_t newline = m_string.find('\n', start);

		const std::size_t startChunk = getChunkIndex(start);
		const std::size_t endChunk = newline == std::string::npos ? m_chunks.size() : getChunkIndex(newline);

		auto begin = std::next(m_chunks.cbegin(), startChunk);
		auto end = std::next(m_chunks.cbegin(), endChunk);
		return std::max_element(begin, end, fn);
	};


	const std::size_t index = std::find(m_string.begin(), m_string.end(), '\n') - m_string.begin();
	float x = 0.f;
	float y = getMaximum(0, [&](const Chunk& left, const Chunk& right) {
		return left.characterSize < right.characterSize;
	})->characterSize;
	std::size_t offset = 0U;
	sf::Uint32 verticeOffset = 0U;
	std::size_t outlineOffset = 0U;
	sf::Uint32 prevChar = 0U;
	float previousX = 0.f;
	float curVSpace = getMaximum(0, [&](const Chunk& left, const Chunk& right) {
		return left.getHeight() < right.getHeight();
	})->getHeight();
	for (auto& chunk : m_chunks) {
		// No font or text: nothing to draw
		if (!chunk.font)
			continue;

		// Compute values related to the text style
		const bool bold = (chunk.style & sf::Text::Style::Bold) != 0;
		const bool underlined = (chunk.style & sf::Text::Style::Underlined) != 0;
		const bool strikeThrough = (chunk.style & sf::Text::Style::StrikeThrough) != 0;
		const float italic = (chunk.style & sf::Text::Style::Italic) ? 0.208f : 0.f; // 12 degrees
		const float underlineOffset = chunk.font->getUnderlinePosition(chunk.characterSize);
		const float underlineThickness = chunk.font->getUnderlineThickness(chunk.characterSize);

		// Compute the location of the strike through dynamically
		// We use the center point of the lowercase 'x' glyph as the reference
		// We reuse the underline thickness as the thickness of the strike through as well
		sf::FloatRect xBounds = chunk.font->getGlyph(L'x', chunk.characterSize, bold).bounds;
		float strikeThroughOffset = xBounds.top + xBounds.height / 2.f;

		// Precompute the variables needed by the algorithm
		float hspace = static_cast<float>(chunk.font->getGlyph(L' ', chunk.characterSize, bold).advance);
		// Create one quad for each character
		minX = std::min(minX, static_cast<float>(chunk.characterSize));
		minY = std::min(minY, static_cast<float>(chunk.characterSize));
		for (std::size_t i = 0U; i != chunk.length; ++i)
		{
			sf::Uint32 curChar = m_string[offset + i];

			// Apply the kerning offset
			x += chunk.font->getKerning(prevChar, curChar, chunk.characterSize);
			prevChar = curChar;

			if (curChar == L'\n')
			{
				minX = std::min(minX, x);
				minY = std::min(minY, y);

				if (underlined) {
					addLine(previousX, m_vertices, x - previousX, y, chunk.fillColor, underlineOffset, underlineThickness);

					if (chunk.outlineThickness != 0) {
						addLine(previousX, m_outlineVertices, x - previousX, y, chunk.outlineColor, underlineOffset, underlineThickness, chunk.outlineThickness);
					}
				}
				if (strikeThrough)
				{
					addLine(previousX, m_vertices, x - previousX, y, chunk.fillColor, strikeThroughOffset, underlineThickness);

					if (chunk.outlineThickness != 0) {
						addLine(previousX, m_outlineVertices, x - previousX, y, chunk.outlineColor, strikeThroughOffset, underlineThickness, chunk.outlineThickness);
					}
				}

				const float height = getMaximum(offset + i + 1, [](const Chunk& left, const Chunk& right) {
					return left.getHeight() < right.getHeight();
				})->getHeight();
				const float max = std::max(height, curVSpace);
				y += std::round(height * 0.65f + max * 0.25f + curVSpace * 0.1f);
				
				curVSpace = height;
				x = 0.f;
				previousX = 0.f;
				maxX = std::max(maxX, x);
				maxY = std::max(maxY, y);
				continue;
			}
			// Handle special characters
			else if ((curChar == ' ') || (curChar == '\t'))
			{
				// Update the current bounds (min coordinates)
				minX = std::min(minX, x);
				minY = std::min(minY, y);

				switch (curChar)
				{
				case ' ':  x += hspace;        break;
				case '\t': x += hspace * 4;    break;
				}

				// Update the current bounds (max coordinates)
				maxX = std::max(maxX, x);
				maxY = std::max(maxY, y);

				// Next glyph, no need to create a quad for whitespace
				continue;
			}


			const sf::Glyph& glyph = chunk.font->getGlyph(curChar, chunk.characterSize, bold);
			if (chunk.outlineThickness != 0)
			{
				const sf::Glyph& glyph = chunk.font->getGlyph(curChar, chunk.characterSize, bold, chunk.outlineThickness);
				const float left = glyph.bounds.left;
				const float top = glyph.bounds.top;
				const float right = glyph.bounds.left + glyph.bounds.width;
				const float bottom = glyph.bounds.top + glyph.bounds.height;
				addGlyphQuad(m_outlineVertices, sf::Vector2f(x, y), chunk.outlineColor, glyph, italic, chunk.outlineThickness);

				// Update the current bounds with the outlined glyph bounds
				minX = std::min(minX, x + left - italic * bottom - chunk.outlineThickness);
				maxX = std::max(maxX, x + right - italic * top - chunk.outlineThickness);
				minY = std::min(minY, y + top - chunk.outlineThickness);
				maxY = std::max(maxY, y + bottom - chunk.outlineThickness);
			}
			else {
				// Update the current bounds with the non outlined glyph bounds
				const float left = glyph.bounds.left;
				const float top = glyph.bounds.top;
				const float right = glyph.bounds.left + glyph.bounds.width;
				const float bottom = glyph.bounds.top + glyph.bounds.height;
				minX = std::min(minX, x + left - italic * bottom);
				maxX = std::max(maxX, x + right - italic * top);
				minY = std::min(minY, y + top);
				maxY = std::max(maxY, y + bottom);
			}


			addGlyphQuad(m_vertices, sf::Vector2f(x, y), chunk.fillColor, glyph, italic);
			// Advance to the next character
			x += glyph.advance;
		}
		offset += chunk.length;
		// If we're using the underlined style, add the last line
		if (underlined && (x > 0))
		{
			addLine(previousX, m_vertices, x - previousX, y, chunk.fillColor, underlineOffset, underlineThickness);

			if (chunk.outlineThickness != 0)
				addLine(previousX, m_outlineVertices, x - previousX, y, chunk.outlineColor, underlineOffset, underlineThickness, chunk.outlineThickness);
		}

		// If we're using the strike through style, add the last line across all characters
		if (strikeThrough && (x > 0))
		{
			addLine(previousX, m_vertices, x - previousX, y, chunk.fillColor, strikeThroughOffset, underlineThickness);

			if (chunk.outlineThickness != 0)
				addLine(previousX, m_outlineVertices, x - previousX, y, chunk.outlineColor, strikeThroughOffset, underlineThickness, chunk.outlineThickness);
		}
		chunk.outlineLength = m_outlineVertices.size() - outlineOffset;
		chunk.verticeLength = m_vertices.size() - verticeOffset;
		outlineOffset = m_outlineVertices.size();
		verticeOffset = m_vertices.size();
		previousX = x;
	}

	// Update the bounding rectangle
	m_bounds.left = minX;
	m_bounds.top = minY;
	m_bounds.width = maxX - minX;
	m_bounds.height = maxY - minY;
}

