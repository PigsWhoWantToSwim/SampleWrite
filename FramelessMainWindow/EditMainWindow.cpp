#include "EditMainWindow.h"

// EditMainWindow::EditMainWindow(QWidget *parent)
//     : BasedFramelessMainWindow{parent}
// {
//     m_novelID = "n240423171951ud";
//     m_novelTitle = "测试标题";

//     // 初始化
//     initMemberVariable();
//     initLayout();
//     initConnect();
// }

// EditMainWindow::EditMainWindow(QString novelID, QWidget *parent)
//     : BasedFramelessMainWindow{parent}
// {
//     m_novelID = novelID;

//     // 初始化
//     initMemberVariable();
//     initLayout();
//     initConnect();
// }

EditMainWindow::EditMainWindow(QString novelID, QString novelTitle, QWidget *parent)
    : BasedFramelessMainWindow{parent}
{
    m_novelID = novelID;
    m_novelTitle = novelTitle;

    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}


void EditMainWindow::initMemberVariable()
{

}

void EditMainWindow::initLayout()
{
    QVBoxLayout *containerLayout = new QVBoxLayout;
    containerLayout->setSpacing(0);
    containerLayout->setContentsMargins(0,0,0,0);
    m_widContainer->setLayout(containerLayout);

    // 标题栏
    m_editTitleBar = new EditTitleBar(m_widContainer);
    m_editTitleBar->setFixedHeight(EDITTITLEBAR_HEIGHT);
    m_editTitleBar->installEventFilter(this);
    m_editTitleBar->setTitleText(m_novelTitle);
    containerLayout->addWidget(m_editTitleBar);

    // 工具栏
    m_editToolBar = new EditToolBar(m_widContainer);
    m_editToolBar->setFixedHeight(EDITTOOLBAR_HEIGHT);
    m_editToolBar->installEventFilter(this);
    containerLayout->addWidget(m_editToolBar);

    // 工作区水平分裂器，水平布局，左边为目录区，中间为文本编辑区，右边为侧边栏
    QSplitter *splitter = new QSplitter(m_widContainer);
    splitter->setOrientation(Qt::Horizontal); // 设置为水平分隔器
    splitter->setChildrenCollapsible(false); // 设置子控件不可被压缩至小时
    splitter->setHandleWidth(0);
    containerLayout->addWidget(splitter);

    // 目录栏
    m_editCatalogBar = new EditCatalogBar(m_novelID, splitter);
    splitter->addWidget(m_editCatalogBar);

    // 工作区
    QWidget *widEditContainer = new QWidget(splitter);
    widEditContainer->setProperty("WidgetStyleClass", "Container");
    splitter->addWidget(widEditContainer);

    QVBoxLayout *workspaceLayout = new QVBoxLayout;
    workspaceLayout->setSpacing(0);
    workspaceLayout->setContentsMargins(0,0,0,0);
    widEditContainer->setLayout(workspaceLayout);

    // 查找栏
    m_findBar = new FindBar(widEditContainer);
    m_findBar->setVisible(false); // 默认隐藏
    // m_findBar->setVisible(false); // 设置默认 隐藏
    workspaceLayout->addWidget(m_findBar);

    // 替换栏
    m_replaceBar = new ReplaceBar(widEditContainer);
    m_replaceBar->setVisible(false); // 默认隐藏
    // m_replaceBar->setVisible(false); // 设置默认 隐藏
    workspaceLayout->addWidget(m_replaceBar);

    // 编辑区
    m_editWorkspace = new EditWorkspace(widEditContainer);
    workspaceLayout->addWidget(m_editWorkspace);

    // 状态栏
    m_editStatusBar = new EditStatusBar(widEditContainer);
    workspaceLayout->addWidget(m_editStatusBar);

    // 侧边栏
    m_editSideBar = new EditSideBar(splitter);
    m_editSideBar->setVisible(false); // 默认隐藏
    splitter->addWidget(m_editSideBar);

    splitter->setSizes(QList<int>()<<220<<400<<180);

    // containerLayout->addSpacerItem(new QSpacerItem(10, 10,QSizePolicy::Fixed, QSizePolicy::Expanding));
}

void EditMainWindow::initConnect()
{
    // 绑定窗口 状态改变，标题栏按钮 点击
    connect(this, &EditMainWindow::windowState_changed, this, &EditMainWindow::do_windowState_change);
    connect(m_editTitleBar, &EditTitleBar::titleBarButton_clicked, this, &EditMainWindow::do_titleBarButton_clicked);

    // 绑定 工具栏 按钮点击  切换 页面
    connect(m_editToolBar, &EditToolBar::returnHomeMainWindow, this, [=](){ emit returnHomeMainWindow();});
    connect(m_editToolBar, &EditToolBar::editToolBarButton_clicked, m_editCatalogBar, &EditCatalogBar::do_editToolBarButton_clicked);
    connect(m_editToolBar, &EditToolBar::editToolBarButton_clicked, m_findBar, &FindBar::do_editToolBarButton_clicked);
    connect(m_editToolBar, &EditToolBar::editToolBarButton_clicked, m_editSideBar, &EditSideBar::do_editToolBarButton_clicked);
    connect(m_editToolBar, &EditToolBar::editToolBarButton_clicked, m_editWorkspace, &EditWorkspace::do_editToolBarButton_clicked);

    // 绑定 查找栏、侧边栏 关闭 页面
    connect(m_findBar, &FindBar::findReplaceBar_close, m_editToolBar, &EditToolBar::do_editBarPages_close);
    connect(m_findBar, &FindBar::replaceBarVisible_changed, m_replaceBar, &ReplaceBar::do_replaceBarVisible_changed);
    connect(m_editSideBar, &EditSideBar::editSideBarPages_close, m_editToolBar, &EditToolBar::do_editBarPages_close);

    // 绑定 目录栏 双击打开文章
    connect(m_editCatalogBar, &EditCatalogBar::openChapter, m_editToolBar, &EditToolBar::do_openChapter);
    connect(m_editCatalogBar, &EditCatalogBar::openChapter, m_editWorkspace, &EditWorkspace::do_openChapter);
    connect(m_editCatalogBar, &EditCatalogBar::openChapter, m_editSideBar, &EditSideBar::do_openChapter);

    // 绑定 目录栏、工作区 修改章节标题
    connect(m_editCatalogBar, &EditCatalogBar::openChapter_renamed, m_editWorkspace, &EditWorkspace::do_chapterItem_renamed);
    connect(m_editWorkspace, &EditWorkspace::chapterTitle_changed, m_editCatalogBar, &EditCatalogBar::do_chapterTitle_changed);
    // 绑定 目录栏 保存章节
    connect(m_editCatalogBar, &EditCatalogBar::beingOpenedChapter_save, m_editWorkspace, &EditWorkspace::do_beingOpenedChapter_save);
    // 绑定 目录栏 删除
    connect(m_editCatalogBar, &EditCatalogBar::directioryItem_deleted, this, [=](){
        emit directioryItem_deleted();
    });
    // 工作区 字数改变
    connect(m_editWorkspace, &EditWorkspace::chapterWordCount_changed, m_editStatusBar, &EditStatusBar::setWordCount);
    connect(m_editWorkspace, &EditWorkspace::chapterWordCount_changed, m_editCatalogBar, &EditCatalogBar::do_chapterWordCount_changed);

    // 侧边栏 与 工作区
    connect(m_editSideBar, &EditSideBar::fontPage_font_changed, m_editWorkspace, &EditWorkspace::do_font_changed);
    connect(m_editSideBar, &EditSideBar::layoutPage_titleAlign_changed, m_editWorkspace, &EditWorkspace::do_titleAlign_changed);
    connect(m_editSideBar, &EditSideBar::contentLayout_changed, m_editWorkspace, &EditWorkspace::do_contentLayout_changed);
    connect(m_editSideBar, &EditSideBar::layout_titleVisible_changed, m_editWorkspace, &EditWorkspace::do_chapterTitleVisible_changed);
    connect(m_editSideBar, &EditSideBar::historicalVersionInfo_init, m_editWorkspace, &EditWorkspace::do_historicalVersionInfo_init);
    connect(m_editSideBar, &EditSideBar::historicalVersion_restore, m_editWorkspace, &EditWorkspace::do_historicalVersion_restore);
    connect(m_editSideBar, &EditSideBar::chapterTitleVisible_changed, m_editWorkspace, &EditWorkspace::do_chapterTitleVisible_changed);

    connect(m_editWorkspace, &EditWorkspace::layoutSliderValues_init, m_editSideBar, &EditSideBar::do_layoutSliderValues_init);
    connect(m_editWorkspace, &EditWorkspace::historicalVersionPage_update, m_editSideBar, &EditSideBar::do_historicalVersionPage_update);

    // 绑定 查找栏、替换栏、工作区
    connect(m_findBar,&FindBar::searchText, m_replaceBar, [=](){ m_replaceBar->setLineEditReplaceEnabled(true);});
    // 绑定 发送要搜索的文本
    connect(m_findBar, &FindBar::searchText, m_editWorkspace, &EditWorkspace::do_searchText);


    // 绑定 搜索输入框内容 改变
    connect(m_findBar, &FindBar::searchText_changed, m_editWorkspace, &EditWorkspace::do_searchText_cleared);
    // 绑定 搜索输入框内容 清空
    connect(m_findBar, &FindBar::searchText_cleared, m_editWorkspace, &EditWorkspace::do_searchText_cleared);
    connect(m_findBar, &FindBar::searchText_cleared, m_replaceBar, [=](){ m_replaceBar->setLineEditReplaceEnabled(false);});
    // 绑定 查找上一个
    connect(m_findBar, &FindBar::searchText_previous, m_editWorkspace, &EditWorkspace::do_searchText_previous);
    // 绑定 查找下一个
    connect(m_findBar, &FindBar::searchText_next, m_editWorkspace, &EditWorkspace::do_searchText_next);
    // 绑定 替换当前
    connect(m_replaceBar, &ReplaceBar::replaceCurrent, m_editWorkspace, &EditWorkspace::do_replaceCurrent);
    // 绑定 替换全部
    connect(m_replaceBar, &ReplaceBar::replaceAll, m_editWorkspace, &EditWorkspace::do_replaceAll);

    // 工作区 关闭 文件
    connect(m_editWorkspace, &EditWorkspace::editWorkspace_noFile, m_editToolBar, &EditToolBar::do_editWorkspace_noFile);
    connect(m_editWorkspace, &EditWorkspace::editWorkspace_noFile, m_editCatalogBar, &EditCatalogBar::do_editWorkspace_noFile);
    connect(m_editWorkspace, &EditWorkspace::editWorkspace_noFile, m_editSideBar, &EditSideBar::do_editWorkspace_noFile);
    connect(m_editWorkspace, &EditWorkspace::editWorkspace_noFile, m_editStatusBar, &EditStatusBar::do_editWorkspace_noFile);
}

void EditMainWindow::do_windowState_change(bool isMaximized)
{
    m_editTitleBar->toggleMaxRestoreButtonVisible(isMaximized);
}

void EditMainWindow::setNovelID(QString novelID)
{
    m_novelID = novelID;
    update();
}

QString EditMainWindow::getNovelID() const
{
    return m_novelID;
}

void EditMainWindow::setTitleText(QString titleText)
{
    m_novelTitle = titleText;
    update();
}

QString EditMainWindow::getTitleText() const
{
    return m_novelTitle;
}


