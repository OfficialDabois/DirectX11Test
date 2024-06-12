#include "Scene.h"

void Scene::Add(IObject& object) {
	objects.push_back(&object);
}

void Scene::Remove(IObject& object) {
	auto it = std::find(objects.begin(), objects.end(), &object);
	if (it != objects.end())
		objects.erase(it);
}

void Scene::Render() {
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->Render();
	}
}