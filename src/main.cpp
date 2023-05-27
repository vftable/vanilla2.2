#include <sstream>
#include <Geode/Geode.hpp>

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/GameManager.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/CCSprite.hpp>

#include <GameObjectFactory.hpp>

using namespace geode::prelude;
using namespace cocos2d;

void squeezeAnim(PlayerObject* player) {
	if (!(player->m_isShip || player->m_isBall || player->m_isDart || player->m_isSpider || player->m_isRobot || (player->m_vehicleSize != 1.0))) {
		float vsize_mod = player->m_vehicleSize == 1.0f ? 1.0f : player->m_vehicleSize;

		CCArray* actions_array = CCArray::create();
		actions_array->addObject(CCScaleTo::create(0.1f, 0.6f * 1 * vsize_mod, 1.3f * vsize_mod));
		actions_array->addObject(CCScaleTo::create(0.1f, 1.0f * 1 * vsize_mod, 1.0f * vsize_mod));

		player->runAction(CCSequence::create(actions_array));
	}
}

/**
2.2 spider pad & orb
**/

class $modify(GJBaseGameLayer) {
	void bumpPlayer(PlayerObject* player, GameObject* object) {
		object->m_orbMultiActivate = false;
		squeezeAnim(player);

		switch (object->m_objectID) {
			case 140: // purple pad
				player->spiderTestJump(player->m_isUpsideDown);
				break;
			default:
				GJBaseGameLayer::bumpPlayer(player, object);
				break;
		}
	}
};

class $modify(PlayerObject) {
	void ringJump(GameObject* object) {
		if (this->m_hasJustHeld) {
			object->m_orbMultiActivate = false;
			squeezeAnim(this);

			switch (object->m_objectID) {
				case 141: // purple orb
					this->spiderTestJump(this->m_isUpsideDown);
					break;
				default:
					PlayerObject::ringJump(object);
					break;
			}
		}
	}
};

/**
2.2 trigger hooks
**/

class SpiderPad : public GameObjectController {
 public:
	SpiderPad(GameObject* g) : GameObjectController(g) {}

	void setup() override {
		m_glowEnabled = false;
		m_object->m_parentMode = -2;

 		auto spr = CCSprite::create("spiderBump_001.png"_spr);
 		auto frame = spr->displayFrame();

 		m_object->setDisplayFrame(frame);
 		m_object->m_objectSize = cocos2d::CCSizeMake(frame->getRect().size.width, frame->getRect().size.height);

 		m_object->m_textureName = "spiderBump_001.png";

		m_object->m_touchTriggered = true;
		m_object->m_objectType = GameObjectType::PinkJumpPad;
	}
};

class SpiderOrb : public GameObjectController {
 public:
	SpiderOrb(GameObject* g) : GameObjectController(g) {}

	void setup() override {
		m_glowEnabled = false;
		m_object->m_parentMode = -2;

 		auto spr = CCSprite::create("spiderRing_001.png"_spr);
 		auto frame = spr->displayFrame();

 		m_object->setDisplayFrame(frame);
 		m_object->m_objectSize = cocos2d::CCSizeMake(frame->getRect().size.width, frame->getRect().size.height);

 		m_object->m_textureName = "spiderRing_001.png";

		m_object->m_touchTriggered = true;
		m_object->m_objectType = GameObjectType::PinkJumpRing;
	}
};

$execute {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	GameObjectFactory::get()->add(140, [](GameObject* object) {
		return new SpiderPad(object);
	});

	GameObjectFactory::get()->add(141, [](GameObject* object) {
		return new SpiderOrb(object);
	});

}