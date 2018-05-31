#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	return GameScene::create();
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	//add touch listener
	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("level-background-0.jpg");
	bg->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	this->addChild(bg);

	stoneLayer = Layer::create();
	stone = Sprite::create("stone.png");
	stone->setPosition(Vec2(560, 480));
	stoneLayer->addChild(stone);
	this->addChild(stoneLayer);
	mouseLayer = Layer::create();
	mouseLayer->setPosition(Vec2(0, origin.y + visibleSize.height / 2));
	mouse = Sprite::createWithSpriteFrameName("gem-mouse-1.png");
	mouse->setPosition(visibleSize.width / 2, 0);
	Animate* mouseAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("mouseAnimation"));
	mouse->runAction(RepeatForever::create(mouseAnimate));
	mouseLayer->addChild(mouse,1);
	this->addChild(mouseLayer,1);
	
	auto label = Label::createWithTTF("Shoot", "fonts/arial.ttf", 50);
	auto menuLabel = MenuItemLabel::create(label, CC_CALLBACK_1(GameScene::shootMenuCallback, this));
	menuLabel -> setPosition(origin.x + visibleSize.width - 120, origin.y+visibleSize.height/2+200);
	auto menu =Menu::create(menuLabel, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
	return true;
}

bool GameScene::onTouchBegan(Touch *touch, Event *unused_event) {

	auto location = touch->getLocation();
	auto cheese = Sprite::create("cheese.png");
	cheese->setPosition(location.x, location.y);
	auto fadeOut = FadeOut::create(2);
	auto delay = DelayTime::create(2);
	auto remove = CallFunc::create([&]() {
		this->removeChild(cheese);
	});
	auto seq = Sequence::create(delay, fadeOut, remove, nullptr);
	cheese->runAction(seq);
	this->addChild(cheese, 0);
	auto loc = mouse->convertToNodeSpace(location);
	auto moveBy = MoveBy::create(2, Vec2(loc.x - mouse->getContentSize().width/2, loc.y - mouse->getContentSize().height/2));
	mouse->runAction(moveBy);
	
	return true;
}

void GameScene::shootMenuCallback(Ref * pSender)
{
	auto stone1 = Sprite::create("stone.png");
	
	auto pos = mouse->convertToWorldSpace(Vec2(0,0));
	stone1->setAnchorPoint(Vec2(0,0));
	stone1->setPosition(stone->getPosition());
	this->addChild(stone1,3);
	auto stone1MoveTo = MoveTo::create(1, pos);
	auto fadeOut = FadeOut::create(0.5);
	auto remove = CallFunc::create([&]() {
		this->removeChild(stone1);
	});
	auto seq = Sequence::create(stone1MoveTo, fadeOut, remove, nullptr);
	stone1->runAction(seq);
	auto diamond = Sprite::createWithSpriteFrameName("diamond-0.png");
	diamond->setPosition(pos);
	diamond->setAnchorPoint(Vec2(0, 0));
	Animate* diamondAnimate = Animate::create(AnimationCache::getInstance()->getAnimation("diamondAinmation"));
	diamond->runAction(RepeatForever::create(diamondAnimate));
	this->addChild(diamond, 2);
	float x = CCRANDOM_0_1() * this->getContentSize().width;
	float y = CCRANDOM_0_1() * this->getContentSize().height;
	auto mouseMoveBy = MoveBy::create(2, mouse->convertToNodeSpace(Vec2(x, y)));
	mouse->runAction(mouseMoveBy);
}


