#include "HomeMainWindow.h"

HomeMainWindow::HomeMainWindow(QWidget *parent)
    : BasedFramelessMainWindow{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void HomeMainWindow::initMemberVariable()
{

}
// 初始化 布局
void HomeMainWindow::initLayout()
{
    // 主布局，网格布局，第一行为 标题栏，第二行第一列为 导航栏，第二行第二列为 工作区
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    m_widContainer->setLayout(mainLayout);

    // 标题栏
    m_homeTitleBar = new HomeTitleBar(m_widContainer);
    m_homeTitleBar->setMinimumWidth(600);
    m_homeTitleBar->setFixedHeight(HOMETITLEBAR_HEIGHT);
    m_homeTitleBar->installEventFilter(this);
    mainLayout->addWidget(m_homeTitleBar, 0, 0, 1, 2);

    // 导航栏
    m_homeNavigtionBar = new HomeNavigationBar(m_widContainer);
    m_homeNavigtionBar->setFixedWidth(HOMENAVIGATIONBAR_WIDTH);
    m_homeNavigtionBar->installEventFilter(this);
    mainLayout->addWidget(m_homeNavigtionBar,1,0,1,1);

    // 工作区
    m_homeWorkspace = new HomeWorkspace(m_widContainer);
    m_homeWorkspace->installEventFilter(this);
    mainLayout->addWidget(m_homeWorkspace,1,1,1,1);
    // mainLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Expanding),1,1,1,1);


}

// 重写 初始化绑定 虚函数
void HomeMainWindow::initConnect()
{
    // 窗口按钮 点击 处理
    connect(this, &HomeMainWindow::windowState_changed, this, &HomeMainWindow::do_windowState_change);
    connect(m_homeTitleBar, &HomeTitleBar::titleBarButton_clicked, this, &HomeMainWindow::do_titleBarButton_clicked);
    connect(m_homeTitleBar, &HomeTitleBar::HomeTitleBar_SettingsAboutBtn_clicked, m_homeNavigtionBar, &HomeNavigationBar::do_HomeTitleBar_ThemeAboutBtn_clicked);
    connect(m_homeTitleBar, &HomeTitleBar::titleBarButton_clicked, m_homeWorkspace, &HomeWorkspace::do_homeTitleBar_clicked);
    connect(m_homeTitleBar, &HomeTitleBar::HomeTitleBar_NewNovelBtn_clicked, this, &HomeMainWindow::do_newNovel);

    // 工作区页面 切换 处理
    connect(m_homeTitleBar, &HomeTitleBar::HomeWorkspacePage_changed, m_homeWorkspace, &HomeWorkspace::do_HomeWorkspacePage_changed);
    connect(m_homeNavigtionBar, &HomeNavigationBar::HomeWorkspacePage_changed, m_homeTitleBar, &HomeTitleBar::do_HomeWorkspacePage_changed);
    connect(m_homeNavigtionBar, &HomeNavigationBar::HomeWorkspacePage_changed, m_homeWorkspace, &HomeWorkspace::do_HomeWorkspacePage_changed);

    // 工作区
    connect(m_homeWorkspace, &HomeWorkspace::newNovel, this, &HomeMainWindow::do_newNovel);
    connect(m_homeWorkspace, &HomeWorkspace::editNovel, this, &HomeMainWindow::do_editNovel);
    connect(m_homeWorkspace, &HomeWorkspace::novelWidget_clicked, this, &HomeMainWindow::do_novelWidget_clicked);



}

// 窗口状态改变，引起窗口按钮改变 处理槽函数
void HomeMainWindow::do_windowState_change(bool isMaximized)
{
    m_homeTitleBar->toggleMaxRestoreButtonVisible(isMaximized);
}

void HomeMainWindow::do_newNovel()
{
    ModifyNovelDialog *newNovelDig = new ModifyNovelDialog;
    // connect(newNovelDig, &ModifyNovelDialog::updateNovels, m_homeWorkspace, &HomeWorkspace::do_updateNovels);
    connect(newNovelDig, &ModifyNovelDialog::novelAdded, m_homeWorkspace, &HomeWorkspace::do_novelAdded);

    MaskFrame *maskFrm = new MaskFrame(this);
    maskFrm->setMaskedWidget(m_widContainer); //设置 被遮罩的控件
    maskFrm->setUpperWidget(newNovelDig); // 设置 在遮罩层上的控件

    // 设置 对话框 位置
    QPoint maskedWidgetLeftTop = m_widContainer->mapToGlobal(QPoint(0, 0));
    int x = maskedWidgetLeftTop.x() + (m_widContainer->width() - newNovelDig->width())/2;
    int y = maskedWidgetLeftTop.y() + (m_widContainer->height() - newNovelDig->height())/2;

    newNovelDig->move(QPoint(x, y));
    newNovelDig->exec();
}

void HomeMainWindow::do_editNovel(QString novelID, QString novelTitle, QString author, QString novelType, QString novelCoverPath)
{
    ModifyNovelDialog *editNovelDig = new ModifyNovelDialog;
    editNovelDig->setNovelInfo(novelID, novelTitle, author, novelType, novelCoverPath);
    connect(editNovelDig, &ModifyNovelDialog::novelEdited, m_homeWorkspace, &HomeWorkspace::do_novelEdited);

    MaskFrame *maskFrm = new MaskFrame(this);
    maskFrm->setMaskedWidget(m_widContainer); //设置 被遮罩的控件
    maskFrm->setUpperWidget(editNovelDig); // 设置 在遮罩层上的控件

    // 设置 对话框 位置
    QPoint maskedWidgetLeftTop = m_widContainer->mapToGlobal(QPoint(0, 0));
    int x = maskedWidgetLeftTop.x() + (m_widContainer->width() - editNovelDig->width())/2;
    int y = maskedWidgetLeftTop.y() + (m_widContainer->height() - editNovelDig->height())/2;

    editNovelDig->move(QPoint(x, y));
    editNovelDig->exec();
}

void HomeMainWindow::do_novelWidget_clicked(QString novelID, QString novelTitle)
{
    m_editMainWindow = new EditMainWindow(novelID, novelTitle);
    connect(m_editMainWindow, &EditMainWindow::returnHomeMainWindow, this, &HomeMainWindow::do_returnHomeMainWindow);
    connect(m_editMainWindow, &EditMainWindow::directioryItem_deleted, m_homeWorkspace, &HomeWorkspace::do_directioryItem_deleted);

    // 设置 位置 尺寸 状态
    QRect postion = this->geometry();
    m_editMainWindow->setGeometry(postion);

    this->hide();
    m_editMainWindow->show();
}

void HomeMainWindow::do_returnHomeMainWindow()
{
    // 设置 位置
    QRect postion = m_editMainWindow->geometry();

    delete m_editMainWindow;
    m_editMainWindow = nullptr;

    this->setGeometry(postion);
    this->show();

    m_homeWorkspace->do_homePage_update();
}




