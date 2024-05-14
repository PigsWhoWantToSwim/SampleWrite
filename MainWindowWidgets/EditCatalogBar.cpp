#include "EditCatalogBar.h"

// EditCatalogBar::EditCatalogBar(QWidget *parent)
//     : QWidget{parent}
// {
//     m_novelID = "n240423171951ud";
//     // 初始化
//     initMemberVariable();
//     initLayout();
//     initConnect();
// }

EditCatalogBar::EditCatalogBar(QString novelID, QWidget *parent)
    : QWidget{parent}
{
    m_novelID = novelID;
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

void EditCatalogBar::initMemberVariable()
{
    // m_backgroundColor = QColor(255, 255, 255);

    // 设置 最大 最小宽度
    this->setMaximumWidth(EDITCATALOGBAR_MAX_WIDTH);
    this->setMinimumWidth(EDITCATALOGBAR_MIN_WIDTH);
}

void EditCatalogBar::initLayout()
{
    // 总布局，垂直布局，上侧为 按钮区，下侧为 容器区
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(6);
    mainLayout->setContentsMargins(0,15,10,0);

    // 按钮区总布局，水平布局，左侧为 菜单按钮区，右侧为 更多按钮
    QHBoxLayout *mainBtnLayout = new QHBoxLayout;
    mainBtnLayout->setSpacing(0);
    mainBtnLayout->setContentsMargins(0,0,0,0);

    // 添加弹簧
    mainBtnLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Fixed));

    // 按钮区，水平布局，左侧为 目录按钮，右侧为 相关按钮
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(20);
    buttonLayout->setContentsMargins(0,0,0,0);

    // 目录树按钮
    m_btnTree = new QPushButton("目录",this); // 新建 按钮
    m_btnTree->setProperty("WidgetStyleClass", "EditCatalogBar_DirectoryButton");
    m_btnTree->setObjectName("m_btnDirectory"); // 设置 对象名
    m_btnTree->setFixedWidth(EDITCATALOGBAR_DIRECTORYBUTTON_WIDTH); // 设置 固定宽度
    m_btnTree->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_btnTree->setCheckable(true); // 设置可 一直按下
    m_btnTree->setAutoExclusive(true); // 设置 自动互斥
    m_btnTree->setChecked(true); // 默认被按下
    buttonLayout->addWidget(m_btnTree); // 添加到 菜单按钮区

    // 添加弹簧
    buttonLayout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed));

    // 目录列表按钮
    m_btnList = new QPushButton("相关",this); // 新建 按钮
    m_btnList->setProperty("WidgetStyleClass", "EditCatalogBar_DirectoryButton");
    m_btnList->setFixedWidth(EDITCATALOGBAR_DIRECTORYBUTTON_WIDTH); // 设置 固定宽度
    m_btnList->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_btnList->setCheckable(true); // 设置可 一直按下
    m_btnList->setAutoExclusive(true); // 设置 自动互斥
    // m_btnRelated->setChecked(true); // 默认被按下
    buttonLayout->addWidget(m_btnList); // 添加到 菜单按钮区

    // 将按钮区 添加到 按钮区总布局
    mainBtnLayout->addLayout(buttonLayout);

    // 添加弹簧
    mainBtnLayout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed));

    // 更多按钮
    // m_btnSort = new QPushButton(this);
    // m_btnSort->setProperty("WidgetStyleClass", "EditCatalogBar_DirectoryButton");
    // m_btnSort->setObjectName("EditCatalogBar_btnSort");
    // m_btnSort->setToolTip("排序");
    // m_btnSort->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    // mainBtnLayout->addWidget(m_btnSort);

    // 将按钮区总布局 添加到 总布局
    mainLayout->addLayout(mainBtnLayout);

    // 栈容器
    m_stkContainer = new QStackedWidget(this);

    // 目录树
    m_directoryTree = new DirectoryTreeWidget(m_novelID, m_stkContainer); // 新建 目录树
    m_stkContainer->addWidget(m_directoryTree); // 添加到 栈容器
    // 设置 当前页
    m_stkContainer->setCurrentIndex(int(CatalogPage::DirectoryTreePage));

    // 相关列表
    m_directoryList = new DirectoryListWidget(m_novelID, m_stkContainer); // 新建 相关列表
    m_stkContainer->addWidget(m_directoryList); // 添加到 栈容器

    // 将栈容器 添加到 总布局
    mainLayout->addWidget(m_stkContainer);

    // 将 总布局 设置为 控件的布局
    this->setLayout(mainLayout);
}

void EditCatalogBar::initConnect()
{
    // 绑定 按钮点击 处理槽函数
    connect(m_btnTree, &QPushButton::clicked, this, [=](){
        do_catalogPage_changed(CatalogPage::DirectoryTreePage);
    });
    connect(m_btnList, &QPushButton::clicked, this, [=](){
        do_catalogPage_changed(CatalogPage::DirectoryListPage);
    });

    // 绑定 更多按钮 点击 槽函数
    // connect(m_btnSort, &QPushButton::clicked, this, &EditCatalogBar::do_btnSort_clicked);

    // 绑定 目录树、目录列表节点 双击事件
    connect(m_directoryTree, &DirectoryTreeWidget::chapterItem_DoubleClicked, this, [=](QString openChapterID, QString openChapterTitle){
        m_openedChapterID = openChapterID;
        emit openChapter(openChapterID, openChapterTitle);
    });
    connect(m_directoryList, &DirectoryListWidget::chapterItem_DoubleClicked, this, [=](QString openChapterID, QString openChapterTitle){
        m_openedChapterID = openChapterID;
        emit openChapter(openChapterID, openChapterTitle);
    });

    // 绑定 目录树和相关列表 重命名 槽函数
    connect(m_directoryTree, &DirectoryTreeWidget::chapterItem_renamed, this, [=](QString chapterID, QString newChapterTitle){
        if(chapterID == m_openedChapterID) // 重命名的章节点 是 正在打开的章节
            emit openChapter_renamed(newChapterTitle); // 发送 新标题
    });
    connect(m_directoryList, &DirectoryListWidget::chapterItem_renamed, this, [=](QString chapterID, QString newChapterTitle){
        if(chapterID == m_openedChapterID) // 重命名的章节点 是 正在打开的章节
            emit openChapter_renamed(newChapterTitle); // 发送 新标题
    });

    // 绑定 保存 当前打开章节文件
    connect(m_directoryTree, &DirectoryTreeWidget:: saveBeingOpenedChapter, this, [=](){
        emit beingOpenedChapter_save();
    });
    connect(m_directoryList, &DirectoryListWidget:: saveBeingOpenedChapter, this, [=](){
        emit beingOpenedChapter_save();
    });

    // 绑定 目录项 已删除
    connect(m_directoryTree, &DirectoryTreeWidget::directioryItem_deleted, this, [=](){
        emit directioryItem_deleted();
    });
    connect(m_directoryList, &DirectoryListWidget::directioryItem_deleted, this, [=](){
        emit directioryItem_deleted();
    });
}
// 设置 背景颜色
void EditCatalogBar::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
// 获取 背景颜色
QColor EditCatalogBar::getBackgroundColor() const
{
    return m_backgroundColor;
}

void EditCatalogBar::setNovelID(QString novelID)
{
    m_novelID = novelID;
    update();
}

QString EditCatalogBar::getNovelID() const
{
    return m_novelID;
}

// 绘制 背景
void EditCatalogBar::drawBackground(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    painter->setBrush(QBrush(m_backgroundColor)); // 设置 画刷

    painter->drawRect(QRect(0,0,this->width(),this->height())); // 绘制 控件尺寸 的矩形

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}

// 重写 绘画事件，绘制 背景
void EditCatalogBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿
    //m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
}
// 显示 事件
void EditCatalogBar::showEvent(QShowEvent *event)
{
    // 调整列宽
    m_directoryTree->updateDirectoryColumnWidth();
    m_directoryList->updateDirectoryColumnWidth();

    // 调用 父类 显示事件
    QWidget::showEvent(event);
}
// 尺寸改变事件
void EditCatalogBar::resizeEvent(QResizeEvent *event)
{
    // 调整列宽
    m_directoryTree->updateDirectoryColumnWidth();
    m_directoryList->updateDirectoryColumnWidth();

    // 调用 父类 显示事件
    QWidget::resizeEvent(event);
}


// 排序按钮 点击处理槽函数
void EditCatalogBar::do_btnSort_clicked()
{
    if(m_stkContainer->currentIndex() == int(CatalogPage::DirectoryTreePage))
    {

    }
    else if(m_stkContainer->currentIndex() == int(CatalogPage::DirectoryListPage))
    {
        m_directoryList->reorder();
    }
}

// 目录按钮，相关按钮 点击 槽函数
void EditCatalogBar::do_catalogPage_changed(CatalogPage page)
{
    switch (page) {
    case CatalogPage::DirectoryTreePage:
        m_stkContainer->setCurrentIndex(int(CatalogPage::DirectoryTreePage));
        m_directoryTree->updateDirectoryColumnWidth();
        break;
    case CatalogPage::DirectoryListPage:
        m_stkContainer->setCurrentIndex(int(CatalogPage::DirectoryListPage));
        m_directoryList->updateDirectoryColumnWidth();
        break;
    default:
        break;
    }
}

// 工具栏 目录按钮 点击 处理槽函数 // 目录按钮 点击 槽函数，实现隐藏和显示
void EditCatalogBar::do_editToolBarButton_clicked(EditToolBarButton toolButton, bool checked)
{
    // 目录按钮 被点击
    if(toolButton == EditToolBarButton::CatalogButton)
    {
        if(checked) // 目录按钮 被“勾选”
            this->setVisible(true); // 显示
        else // 目录按钮 不被“勾选”
            this->setVisible(false); // 隐藏
    }
}

// 文本框的章节标题 发生变化，处理槽函数，更新对应章节点的标题
void EditCatalogBar::do_chapterTitle_changed(QString chapterID, QString newChapterTitle)
{
    // 判断 当前打开的章节的 分类
    if(chapterID.at(0) == QChar('c'))
    {
        // 当前 打开的是普通章节，更新对应节点的标题
        m_directoryTree->setChapterNewTitle(chapterID, newChapterTitle);
    }
    else if(chapterID.at(0) == QChar('r'))
    {
        // 当前 打开的是相关文章，更新对应节点的标题
        m_directoryList->setChapterNewTitle(chapterID, newChapterTitle);
    }
}
// 章节字数改变 处理槽函数
void EditCatalogBar::do_chapterWordCount_changed(int wordCount)
{
    // 判断 当前打开的章节的 分类
    if(m_openedChapterID.at(0) == QChar('c'))
    {
        // 当前 打开的是普通章节，更新对应节点的字数
        m_directoryTree->setChapterNodeWordCount(m_openedChapterID, wordCount);
    }
    else if(m_openedChapterID.at(0) == QChar('r'))
    {
        // 当前 打开的是相关文章，更新对应节点的字数
        m_directoryList->setChapterNodeWordCount(m_openedChapterID, wordCount);
    }
}

// 关闭文本编辑编辑页
void EditCatalogBar::do_editWorkspace_noFile()
{
    // 判断 当前打开的章节的 分类
    if(m_openedChapterID.at(0) == QChar('c'))
    {
        // 当前 打开的是普通章节
        m_directoryTree->clearChapterIDSent();
    }
    else if(m_openedChapterID.at(0) == QChar('r'))
    {
        // 当前 打开的是相关文章
        m_directoryList->clearChapterIDSent();
    }

    // 将 正打开的章节ID置空
    m_openedChapterID = "";
}
