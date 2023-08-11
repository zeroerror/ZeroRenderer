#include "EditorContext.h"

EditorContext::EditorContext() {
}

EditorContext::~EditorContext() {
    if(currentSceneMeta != nullptr) delete currentSceneMeta;
    if(sceneView != nullptr) delete sceneView;

    if(rootCanvas != nullptr) delete rootCanvas;
    if(hierarchyCanvasNode != nullptr) delete hierarchyCanvasNode;
    if(titleBarCanvasNode != nullptr) delete titleBarCanvasNode;
    if(sceneViewCanvasNode != nullptr) delete sceneViewCanvasNode;
    if(projectLeftCanvasNode != nullptr) delete projectLeftCanvasNode;
}