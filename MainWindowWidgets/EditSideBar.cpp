#include "EditSideBar.h"

EditSideBar::EditSideBar(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}


// 初始化 成员函数
void EditSideBar::initMemberVariable()
{
    // 设置 控件 宽度范围
    this->setMaximumWidth(SIDEBAR_MAX_WIDTH);
    this->setMinimumWidth(SIDEBAR_MIN_WIDTH);
}
// 初始化 布局
void EditSideBar::initLayout()
{
    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    this->setLayout(mainLayout);

    // 栈容器
    m_stkContainer = new QStackedWidget(this); // 新建 栈容器
    mainLayout->addWidget(m_stkContainer); // 添加到布局

    // 主题页面
    m_themePage = new SideBarThemePage(m_stkContainer);
    // m_themePage->setStyleSheet("background-color: #d8a9e4;");
    m_stkContainer->addWidget(m_themePage);

    // 字体页面
    m_fontPage = new SideBarFontPage(m_stkContainer);
    // m_fontPage->setStyleSheet("background-color: #aef445;");
    m_stkContainer->addWidget(m_fontPage);

    // 布局页面
    m_layoutPage = new SideBarLayoutPage(m_stkContainer);
    // m_layoutPage->setStyleSheet("background-color: #8495a5;");
    m_stkContainer->addWidget(m_layoutPage);

    m_historicalPage = new SideBarHistoricalPage(m_stkContainer);
    // m_historicalPage->setStyleSheet("background-color: #a374c8;");
    m_stkContainer->addWidget(m_historicalPage);

    m_settingPage = new SideBarSettingPage(m_stkContainer);
    // m_settingPage->setStyleSheet("background-color: #8428f4;");
    m_stkContainer->addWidget(m_settingPage);

    // 设置 主题页 为当前页
    m_stkContainer->setCurrentIndex(int(SideBarPage::ThemePage));
}
// 初始化 绑定事件
void EditSideBar::initConnect()
{
    // 绑定 页面 关闭 处理槽函数
    connect(m_themePage, &SideBarThemePage::closePage, this, [=](){
        emit editSideBarPages_close(EditToolBarButton::ThemeButton);
    });
    connect(m_fontPage, &SideBarFontPage::closePage, this, [=](){
        emit editSideBarPages_close(EditToolBarButton::FontButton);
    });
    connect(m_layoutPage, &SideBarLayoutPage::closePage, this, [=](){
        emit editSideBarPages_close(EditToolBarButton::LayoutButton);
    });
    connect(m_historicalPage, &SideBarHistoricalPage::closePage, this, [=](){
        emit editSideBarPages_close(EditToolBarButton::HistoricalButton);
    });
    connect(m_settingPage, &SideBarSettingPage::closePage, this, [=](){
        emit editSideBarPages_close(EditToolBarButton::SettingsButton);
    });


    // 绑定 字体改变 槽函数
    connect(m_fontPage, &SideBarFontPage::font_changed,this,[=](TextEditArea editArea, QFont font, QColor fontColor){
        emit fontPage_font_changed(editArea, font, fontColor);
    });

    // 绑定 标题对齐方式 改变 槽函数
    connect(m_layoutPage, &SideBarLayoutPage::titleAlign_changed,this,[=](Qt::Alignment flag){
        emit layoutPage_titleAlign_changed(flag);
    });

    // 绑定 间距边距滑动条 改变 槽函数
    connect(m_layoutPage, &SideBarLayoutPage::layoutValuesChanged,this,[=](int firstLineIndent, int wordSpacing,
                                                                            int lineSpacing, int leftRightMargin,
                                                                            int bottomMargin){
        emit contentLayout_changed(firstLineIndent, wordSpacing, lineSpacing, leftRightMargin, bottomMargin);
    });

    // 绑定 历史版本页面 初始化信息信号
    connect(m_historicalPage, &SideBarHistoricalPage::initHistoricalVersionInfo,this,[=](QString latestVersionID,int backupFileCount){
        emit historicalVersionInfo_init(latestVersionID, backupFileCount);
    });
    // 绑定 历史版本 恢复
    connect(m_historicalPage, &SideBarHistoricalPage::restoreVersion,this,[=](QString filePath){
        emit historicalVersion_restore(filePath);
    });

    // 绑定 章节标题可见
    connect(m_settingPage, &SideBarSettingPage::chapterTitleVisible_changed, this, [=](bool checked){
        emit chapterTitleVisible_changed(checked);
    });
}

// 工具按钮 点击处理 槽函数，实现页面切换
void EditSideBar::do_editToolBarButton_clicked(EditToolBarButton toolButton, bool checked)
{
    // 进行 切换页面
    switch (toolButton) {
    case EditToolBarButton::ThemeButton:
        m_stkContainer->setCurrentIndex(int(SideBarPage::ThemePage)); // 切换 页面
        this->setVisible(checked); // 显示 或 隐藏 侧边栏
        break;
    case EditToolBarButton::FontButton:
        m_stkContainer->setCurrentIndex(int(SideBarPage::FontPage)); // 切换 页面
        this->setVisible(checked); // 显示 或 隐藏 侧边栏
        break;
    case EditToolBarButton::LayoutButton:
        m_stkContainer->setCurrentIndex(int(SideBarPage::LayoutPage)); // 切换 页面
        this->setVisible(checked); // 显示 或 隐藏 侧边栏
        break;
    case EditToolBarButton::HistoricalButton:
        m_stkContainer->setCurrentIndex(int(SideBarPage::HistoricalPage)); // 切换 页面
        this->setVisible(checked); // 显示 或 隐藏 侧边栏
        break;
    case EditToolBarButton::SettingsButton:
        m_stkContainer->setCurrentIndex(int(SideBarPage::SettingPage)); // 切换 页面
        this->setVisible(checked); // 显示 或 隐藏 侧边栏
        break;
    case EditToolBarButton::ShelfButton:
    case EditToolBarButton::CatalogButton:
    case EditToolBarButton::UndoButton:
    case EditToolBarButton::RedoButton:
    case EditToolBarButton::TypeSettingButton:
    case EditToolBarButton::FindReplaceButtton:
        break;
    default:
        break;
    }

}

// 双击 打开文章 处理槽函数
void EditSideBar::do_openChapter(QString openChapterID, QString openChapterTitle)
{
    // 历史版本页面 处理 打开章节 槽函数
    m_historicalPage->do_openChapter(openChapterID);
}

// 更新 历史版本页面 处理槽函数
void EditSideBar::do_historicalVersionPage_update()
{
    m_historicalPage->updateButtonArea();
}

// 初始化 布局 数值
void EditSideBar::do_layoutSliderValues_init(int firstLineIndent, int wordSpacing, int lineSpacing, int leftRightMargin, int bottomMargin)
{
    m_layoutPage->setSlidersValues(firstLineIndent,wordSpacing,lineSpacing,leftRightMargin,bottomMargin);
}

// 关闭文本编辑编辑页
void EditSideBar::do_editWorkspace_noFile()
{
    // 清空 历史版本
    m_historicalPage->clearHistoricalPage();
}

