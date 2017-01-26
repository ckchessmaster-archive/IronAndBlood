#include "SpriteBatch.h"

#include <algorithm>

namespace Pixel {

	SpriteBatch::SpriteBatch() : _vbo(0), _vao(0) {
	
	}//end Constructor

	SpriteBatch::~SpriteBatch() {}

	void SpriteBatch::init() {
		createVertexArray();
	}//end init

	void SpriteBatch::begin(GlyphSortType sortType /* GlyphSortType::TEXTURE */) {
		_sortType = sortType;
		_renderBatches.clear();

		//Make sure to free the memory for the glyphs before we clear them
		for (int i = 0; i < _glyphs.size(); i++) {
			delete _glyphs[i];
		}//end for

		_glyphs.clear();
	}//end begin

	void SpriteBatch::end() {
		sortGlyphs();
		createRenderBatches();
	}//end end

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color) {
		Glyph* newGlyph = new Glyph;
		newGlyph->texture = texture;
		newGlyph->depth = depth;

		newGlyph->topLeft.color = color;
		newGlyph->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		newGlyph->bottomLeft.color = color;
		newGlyph->bottomLeft.setPosition(destRect.x, destRect.y);
		newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);

		newGlyph->bottomRight.color = color;
		newGlyph->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		newGlyph->topRight.color = color;
		newGlyph->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		_glyphs.push_back(newGlyph);
	}//end draw

	void SpriteBatch::renderBatch() {
		glBindVertexArray(_vao);
		
		for (int i = 0; i < _renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}//end for

		glBindVertexArray(0);
	}//end renderBatch

	void SpriteBatch::createRenderBatches() {
		std::vector<Vertex> verticies;
		verticies.resize(_glyphs.size() * 6);

		if (_glyphs.empty()) {
			return;
		}//end if

		int offset = 0;
		int currentVertex = 0;
		_renderBatches.emplace_back(offset, 6, _glyphs[0]->texture);
		verticies[currentVertex++] = _glyphs[0]->topLeft;
		verticies[currentVertex++] = _glyphs[0]->bottomLeft;
		verticies[currentVertex++] = _glyphs[0]->bottomRight;
		verticies[currentVertex++] = _glyphs[0]->bottomRight;
		verticies[currentVertex++] = _glyphs[0]->topRight;
		verticies[currentVertex++] = _glyphs[0]->topLeft;
		offset += 6;

		for (int currentGlyph = 1; currentGlyph < _glyphs.size(); currentGlyph++) {
			if (_glyphs[currentGlyph]->texture != _glyphs[currentGlyph - 1]->texture) {
				_renderBatches.emplace_back(offset, 6, _glyphs[currentGlyph]->texture);
			} else {
				_renderBatches.back().numVertices += 6;
			}//end if/else
			
			verticies[currentVertex++] = _glyphs[currentGlyph]->topLeft;
			verticies[currentVertex++] = _glyphs[currentGlyph]->bottomLeft;
			verticies[currentVertex++] = _glyphs[currentGlyph]->bottomRight;
			verticies[currentVertex++] = _glyphs[currentGlyph]->bottomRight;
			verticies[currentVertex++] = _glyphs[currentGlyph]->topRight;
			verticies[currentVertex++] = _glyphs[currentGlyph]->topLeft;
			offset += 6;
		}//end for

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, verticies.size() * sizeof(Vertex), verticies.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}//end createRenderBatches

	void SpriteBatch::createVertexArray() {
		
		if (_vao == 0) {
			glGenVertexArrays(1, &_vao);
		}//end if
		glBindVertexArray(_vao);


		if (_vbo == 0) {
			glGenBuffers(1, &_vbo);
		}//end if
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		//Tell opengl that we want to use the first attribute array.
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//This is the position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//This is the color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//This is the UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);

	}//end createVertexArray

	void SpriteBatch::sortGlyphs() {
		
		switch (_sortType) {
			case GlyphSortType::BACK_TO_FRONT:
				std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
				break;
			//end case BACK_TO_FRONT

			case GlyphSortType::FRONT_TO_BACK:
				std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
				break;
			//end case FRONT_TO_BACK

			case GlyphSortType::TEXTURE:
				std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
				break;
			//end case TEXTURE
		}//end switch
		
	}//end sortGlyphs

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
		return (a->depth < b->depth);
	}//end compareFrontToBack

	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
		return (a->depth > b->depth);
	}//end compareBackToFront

	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
		return (a->texture < b->texture);
	}//end compareTexture
}//end namespace