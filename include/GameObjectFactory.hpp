#pragma once

#include <Geode/Geode.hpp>
#include <map>
#include <optional>
#include <functional>
#include "GameObjectController.hpp"

class GameObjectFactory {
 protected:
 	using container_t = GameObjectController*; // might change the name
 	using generator_t = std::function<container_t(GameObject*)>;

 	std::map<unsigned int, generator_t> m_generators;
 	std::map<unsigned int, container_t> m_containers; // uuid

 	std::map<unsigned int, std::string> m_textures;
  public:

 	std::optional<container_t> fetch(unsigned int uuid);
 	std::optional<container_t> fetch(GameObject* obj);

 	std::optional<generator_t> fetchGenerator(unsigned int object_id);
 	std::string const& fetchTexture(unsigned int object_id);

 	void clear();

	static GameObjectFactory* get();
	// Add custom object generator
	void add(unsigned int object_id, generator_t gen);
	// Add custom object generator with custom texture
	void add(unsigned int object_id, std::string txt, generator_t gen);
	// Assign texture for object
	void addTexture(unsigned int object_id, std::string txt);

	// At runtime, add custom object
	void addContainer(unsigned int uuid, container_t container);
};