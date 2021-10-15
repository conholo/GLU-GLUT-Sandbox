#pragma once

#include <stdint.h>

namespace Core
{
	class Texture
	{
	public:
		Texture(const char* filePath);

		uint32_t GetTextureID() const { return m_TextureID; }

	private:
		uint32_t m_TextureID;
	};
}
