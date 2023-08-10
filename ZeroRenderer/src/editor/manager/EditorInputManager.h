#pragma once
#include "KeyStatus.h"
#include <src/vendor/glm/ext/vector_float2.hpp>
#include "MouseButton.h"
#include <imgui/imgui.h>

using namespace glm;
using namespace std;

class EditorInputManager {

public:
	static vec2 GetMousePosDelta();
	static vec2 GetMousePos();
	static bool GetMouseButtonDown(const MouseButtons_& button);
	static bool GetMouseButtonPressing(const MouseButtons_& button);
	static bool GetMouseButtonUp(const MouseButtons_& button);
	static bool GetKeyDown(const ImGuiKey& key);
	static bool GetKeyPressing(const ImGuiKey& key);
	static bool GetKeyUp(const ImGuiKey& key);

	static bool _CheckMouseDown(const MouseButtons_& button);
	static bool _CheckKeyDown(const ImGuiKey& key);
	static void _TickMouseKeyStatus(const MouseButtons_& key);
	static void _TickKeyStatus(const ImGuiKey& key);
	static void _TickEditorInput();

private:
	static KeyStatus_ _allKeyStatus[1024];
	static vec2 _lastMousePos;
	static vec2 _mousePosDelta;

};