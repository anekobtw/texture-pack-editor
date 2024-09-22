#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <windows.h>
#include <commdlg.h>
#include <iostream>
using namespace geode::prelude;

class CustomPopup : public FLAlertLayer {
private:
    InputNode* inputFileName;

public:
    bool init() {
        if (!FLAlertLayer::init(150)) return false;

        auto screenSize = CCDirector::sharedDirector()->getWinSize();

        auto inputSpriteName = InputNode::create(300, "Sprite name");
        inputSpriteName->setPosition(ccp(screenSize.width / 2, screenSize.height / 3 * 2));

        inputFileName = InputNode::create(300, "File name");
        inputFileName->setPosition(ccp(screenSize.width / 2, screenSize.height / 3 * 1));

        auto openFileDialogButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("gj_folderBtn_001.png"),
            this, menu_selector(CustomPopup::openFileDialog)
        );
        openFileDialogButton->setPosition(ccp(screenSize.width / 2 + 175, screenSize.height / 3 * 1));
        auto menu = CCMenu::create();
        menu->setPosition(ccp(0, 0));
        menu->addChild(openFileDialogButton);

        this->m_mainLayer->addChild(CCLayerColor::create(ccc4(0, 0, 0, 100)));
        this->m_mainLayer->addChild(inputSpriteName);
        this->m_mainLayer->addChild(inputFileName);
        this->m_mainLayer->addChild(menu);

        handleTouchPriority(this);
        return true;
    }

    void registerWithTouchDispatcher() override {
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -503, true);
    }

    void openFileDialog(CCObject*) {
        std::string selectedFile = FileDialog();
        if (!selectedFile.empty()) {
            inputFileName->setString(selectedFile);
            CCLOG("Selected file: %s", selectedFile.c_str());
        }
    }

    std::string FileDialog() {
        OPENFILENAME ofn;       // common dialog box structure
        char szFile[260];      // buffer for file name

        // Initialize OPENFILENAME
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "All Files\0*.*\0Text Documents\0*.TXT\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        
        // Display the File Open dialog box
        if (GetOpenFileName(&ofn)) {
            return ofn.lpstrFile;
        } else {
            return "";
        }
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
