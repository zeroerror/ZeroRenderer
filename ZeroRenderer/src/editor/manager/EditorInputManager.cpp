#include "EditorInputManager.h"
#include <src/vendor/imgui/imgui.h>
#include "MouseButton.h"
#include "ZeroMathUtil.h"

KeyStatus_ EditorInputManager::_allKeyStatus[1024] = {};
vec2 EditorInputManager::_lastMousePos;
vec2 EditorInputManager::_mousePosDelta;

bool EditorInputManager::EditorInputManager::_CheckMouseDown(const MouseButtons_& button) {
	ImGuiIO& io = ImGui::GetIO();
	return io.MouseDown[button];
}

bool EditorInputManager::_CheckKeyDown(const ImGuiKey& key) {
	ImGuiIO& io = ImGui::GetIO();
	return io.KeysDown[key];
}

vec2 EditorInputManager::GetMousePosDelta() { return _mousePosDelta; }
vec2 EditorInputManager::GetMousePos() {
	ImVec2 p = ImGui::GetMousePos();
	return vec2(p.x, p.y);
}

bool EditorInputManager::GetMouseButtonDown(const MouseButtons_& button) { return _allKeyStatus[button] == KeyStatus_Down; }
bool EditorInputManager::GetMouseButtonPressing(const MouseButtons_& button) { return _allKeyStatus[button] == KeyStatus_Pressing; }
bool EditorInputManager::GetMouseButtonUp(const MouseButtons_& button) { return _allKeyStatus[button] == KeyStatus_Up; }
bool EditorInputManager::GetKeyDown(const ImGuiKey& key) { return _allKeyStatus[key] == KeyStatus_Down; }
bool EditorInputManager::GetKeyPressing(const ImGuiKey& key) { return _allKeyStatus[key] == KeyStatus_Pressing; }
bool EditorInputManager::GetKeyUp(const ImGuiKey& key) { return _allKeyStatus[key] == KeyStatus_Up; }

void EditorInputManager::_TickMouseKeyStatus(const MouseButtons_& key) {
	if (_CheckMouseDown(key))_allKeyStatus[key] = (KeyStatus_)ZeroMathUtil::_Clamp((_allKeyStatus[key] + 1), KeyStatus_Down, KeyStatus_Pressing);
	else _allKeyStatus[key] = (KeyStatus_)ZeroMathUtil::_Clamp((_allKeyStatus[key] - 1), KeyStatus_None, KeyStatus_Up);
}

void EditorInputManager::_TickKeyStatus(const ImGuiKey& key) {
	if (_CheckKeyDown(key))_allKeyStatus[key] = (KeyStatus_)ZeroMathUtil::_Clamp((_allKeyStatus[key] + 1), KeyStatus_Down, KeyStatus_Pressing);
	else _allKeyStatus[key] = (KeyStatus_)ZeroMathUtil::_Clamp((_allKeyStatus[key] - 1), KeyStatus_None, KeyStatus_Up);
}

void EditorInputManager::_TickEditorInput() {
	vec2 curMousePos = GetMousePos();
	_mousePosDelta = curMousePos - _lastMousePos;
	_lastMousePos = curMousePos;

	_TickMouseKeyStatus(MouseButtons_Left);
	_TickMouseKeyStatus(MouseButtons_Right);
	_TickMouseKeyStatus(MouseButtons_Middle);
	_TickKeyStatus(ImGuiKey_Backspace);
	_TickKeyStatus(ImGuiKey_W);
	_TickKeyStatus(ImGuiKey_S);
	_TickKeyStatus(ImGuiKey_A);
	_TickKeyStatus(ImGuiKey_D);
}