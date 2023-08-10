#include "EditorContext.h"

EditorContext::EditorContext() {
}

EditorContext::~EditorContext() {
    if(currentSceneMeta != nullptr) delete currentSceneMeta;
    if(sceneViewCamera != nullptr) delete sceneViewCamera;

    if(rootCanvas != nullptr) delete rootCanvas;
    if(hierarchyCanvasNode != nullptr) delete hierarchyCanvasNode;
    if(titleBarCanvasNode != nullptr) delete titleBarCanvasNode;
    if(sceneViewCanvasNode != nullptr) delete sceneViewCanvasNode;
    if(projectLeftCanvasNode != nullptr) delete projectLeftCanvasNode;
}