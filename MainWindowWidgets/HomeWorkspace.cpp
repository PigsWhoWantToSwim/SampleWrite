#include "HomeWorkspace.h"

HomeWorkspace::HomeWorkspace(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}




void HomeWorkspace::initMemberVariable()
{

}

void HomeWorkspace::initLayout()
{
    // 主布局，用于将 栈容器居中
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    this->setLayout(mainLayout);

    // 栈容器
    m_stkContainer = new QStackedWidget(this);
    mainLayout->addWidget(m_stkContainer);

    m_homePage = new HomeWorkspaceHomePage(m_stkContainer);
    m_stkContainer->addWidget(m_homePage);

    m_findPage = new BlankPage(m_stkContainer);
    m_findPage->setTipsText("暂无内容");
    // m_findPage->setStyleSheet("background-color: #7a2c8a;");
    m_stkContainer->addWidget(m_findPage);

    m_recyclePage = new HomeWorkspaceRecyclePage(m_stkContainer);
    // m_recyclePage->setStyleSheet("background-color: #9f7c4a;");
    m_stkContainer->addWidget(m_recyclePage);

    m_settingsPage = new HomeWorkspaceSettingsPage(m_stkContainer);
    // m_settingsPage->setStyleSheet("background-color: #4b9c44;");
    m_stkContainer->addWidget(m_settingsPage);

    m_aboutPage = new HomeWorkspaceAboutPage(m_stkContainer);
    // m_aboutPage->setStyleSheet("background-color: #6b49af;");
    m_stkContainer->addWidget(m_aboutPage);

    m_stkContainer->setCurrentIndex(0); // 设置为默认页面
}

void HomeWorkspace::initConnect()
{
    // 新建 和 编辑 小说
    connect(m_homePage, &HomeWorkspaceHomePage::newNovel, this, [=](){
        emit newNovel();
    });
    connect(m_homePage, &HomeWorkspaceHomePage::editNovel,
            this, [=](QString novelID, QString novelTitle, QString author,
                      QString novelType, QString novelCoverPath){
        emit editNovel(novelID, novelTitle, author, novelType, novelCoverPath);
    });
    // 点击小说
    connect(m_homePage, &HomeWorkspaceHomePage::novelWidget_clicked, this, [=](QString novelID, QString novelTitle){
        emit novelWidget_clicked(novelID, novelTitle);
    });

    // 删除 和 恢复 小说
    connect(m_homePage, &HomeWorkspaceHomePage::deleteNovel, m_recyclePage, &HomeWorkspaceRecyclePage::do_deleteNovel);
    connect(m_recyclePage, &HomeWorkspaceRecyclePage::recycledWork_restored, m_homePage, &HomeWorkspaceHomePage::do_updateHomePage);
}

// 标题栏 设置关于按钮点击 处理槽函数
void HomeWorkspace::do_homeTitleBar_clicked(TitleBarClickedButton clickedButton)
{
    Q_UNUSED(clickedButton);

    m_homePage->updateContainerWidth();
    m_settingsPage->updateContainerWidth();
}
// 页面改变 处理槽函数
void HomeWorkspace::do_HomeWorkspacePage_changed(HomeMainWindowPage page)
{
    switch (page) {
    case HomeMainWindowPage::HomePage:
        m_stkContainer->setCurrentIndex(int(HomeMainWindowPage::HomePage));
        break;
    case HomeMainWindowPage::FindPage:
        m_stkContainer->setCurrentIndex(int(HomeMainWindowPage::FindPage));
        break;
    case HomeMainWindowPage::RecyclePage:
        m_stkContainer->setCurrentIndex(int(HomeMainWindowPage::RecyclePage));
        break;
    case HomeMainWindowPage::SettingsPage:
        m_stkContainer->setCurrentIndex(int(HomeMainWindowPage::SettingsPage));
        break;
    case HomeMainWindowPage::AboutPage:
        m_stkContainer->setCurrentIndex(int(HomeMainWindowPage::AboutPage));
        break;
    default:
        break;
    }
}

// 新小说 被添加 处理槽函数
// void HomeWorkspace::do_updateNovels()
// {
//     m_homePage->updatePage();
// }

void HomeWorkspace::do_novelAdded(QString novelID)
{
    m_homePage->do_novelAdded(novelID);
}

void HomeWorkspace::do_novelEdited(QString novelID)
{
    m_homePage->do_novelEdited(novelID);
}

void HomeWorkspace::do_homePage_update()
{
    m_homePage->do_updateHomePage();
}
// 目录栏 删除项 处理槽函数
void HomeWorkspace::do_directioryItem_deleted()
{
    m_recyclePage->do_directoryItem_deleted();
}

// void HomeWorkspace::resizeEvent(QResizeEvent *event)
// {
//     QWidget::resizeEvent(event);

//     m_settingsPage->updateThemeContainerLayout();
// }
