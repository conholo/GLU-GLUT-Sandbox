#pragma once

#include "Core.h"

class TextureTestLayer : public Core::Layer
{
public:

	void OnAttach() override;
	void OnDetach() override;
	void OnDisplay(float deltaTime) override;
	void OnAnimate(float deltaTime) override;
	void OnEvent(Core::Event& event) override;

private:
	void CreateTexture();
	int CreateTexturedGeometryDrawList(Core::PrimitiveType type);

private:
	uint32_t m_TextureID;
	int CubeDrawList = -1;
};