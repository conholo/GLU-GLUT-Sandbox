#pragma once

#include "Core.h"
#include <stdint.h>

class Project3Layer : public Core::Layer
{
public:
	Project3Layer();
	~Project3Layer();

	void OnAttach() override;
	void OnDetach() override;
	void OnDisplay(float deltaTime) override;
	void OnAnimate(float deltaTime) override;
	void OnEvent(Core::Event& e) override;

private:
	void CreateGlobeList();
	void DisplayDistortedGlobe();
	void DisplayNormalGlobe();
	void DisplayGlobe();
	void CreateGUI();

	static void OnDisplayModeChanged(int mode);
	static void OpenMainMenu(int mode);

	static Project3Layer* s_Instance;

private:
	bool m_Distort = false;
	bool m_Textured = false;

private:
	float m_CameraRotation = 0.0f;
	Core::Geometry* m_Geometry = nullptr;
	Core::Geometry* m_GeometryB = nullptr;
	float m_ScaleAmount = 1.0f;
	float m_Counter = 0.0f;
	float m_DistortionPercentA = 0.0f;
	float m_DistortionPercentB = 0.0f;
	float m_DistortionAmount = 0.0f;
	Core::Texture* m_Texture = nullptr;
	int32_t m_GeometryList;
};