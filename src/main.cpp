#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <windows.h>
#include <commdlg.h>
#include <iostream>
using namespace geode::prelude;


std::string FileDialog() {
    OPENFILENAME ofn;       
    char szFile[260] = { 0 };  

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "PNG Photos\0*.PNG\0All Files\0*.*\0";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        return ofn.lpstrFile;
    } else {
        CCLOG("File dialog failed: %d", CommDlgExtendedError());
        return "";
    }
}


class CustomPopup : public FLAlertLayer {
private:
    InputNode* inputFileName;

    void setupUI() {
        auto screenSize = CCDirector::sharedDirector()->getWinSize();

        auto inputSpriteName = InputNode::create(300, "Sprite name");
        inputSpriteName->setPosition(ccp(screenSize.width / 2, screenSize.height / 3 * 2));

        inputFileName = InputNode::create(300, "File name");
        inputFileName->setPosition(ccp(screenSize.width / 2, screenSize.height / 3));

        auto openFileDialogButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("gj_folderBtn_001.png"),
            this, menu_selector(CustomPopup::openFileDialog)
        );
        openFileDialogButton->setPosition(ccp(screenSize.width / 2 + 175, screenSize.height / 3));

        auto menu = CCMenu::create();
        menu->setPosition(ccp(0, 0));
        menu->addChild(openFileDialogButton);

        this->m_mainLayer->addChild(CCLayerColor::create(ccc4(0, 0, 0, 100)));
        this->m_mainLayer->addChild(inputSpriteName);
        this->m_mainLayer->addChild(inputFileName);
        this->m_mainLayer->addChild(menu);
    }

public:
    bool init() {
        if (!FLAlertLayer::init(150)) return false;

        setupUI();
        handleTouchPriority(this);
        return true;
    }

    void registerWithTouchDispatcher() override {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -503, true);
    }

    void openFileDialog(CCObject*) {
        inputFileName->setString(FileDialog());
    }

    CREATE_FUNC(CustomPopup);
};

class $modify(MyMenuLayer, MenuLayer) {
public:
    bool init() {
        if (!MenuLayer::init()) return false;

        auto myButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
            this,
            menu_selector(MyMenuLayer::load_scene)
        );

        auto rightSideMenu = this->getChildByID("right-side-menu");
        rightSideMenu->addChild(myButton);
        rightSideMenu->updateLayout();

        return true;
    }

    void load_scene(CCObject*) {
        auto customPopup = CustomPopup::create();
        CCDirector::sharedDirector()->getRunningScene()->addChild(customPopup, 1000);
    }
};
