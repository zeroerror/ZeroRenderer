#include "EditorApp.h"

int main() {
	EditorApp editorApp = EditorApp();
	while (editorApp.Tick() != 0) {}
}