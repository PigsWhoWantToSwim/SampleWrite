#include "EditToolBar.h"

EditToolBar::EditToolBar(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void EditToolBar::initMemberVariable()
{
    m_backgroundColor = QColor(27, 178, 139); // 设置 默认背景颜色
}
// 初始化 布局
void EditToolBar::initLayout()
{
    // 总布局，水平布局，依次放置 各按钮
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(4);
    mainLayout->setContentsMargins(5,0,5,0);
    this->setLayout(mainLayout); // 将 主布局 设置 为控件布局

    // 返回书架按钮
    m_tbtnNovelShelf = new QToolButton(this); // 新建 工具按钮
    m_tbtnNovelShelf->setObjectName("EditToolBar_tbtnNovelShelf");
    m_tbtnNovelShelf->setProperty("WidgetStyleClass", "EditToolBar_ToolButton");
    // m_tbtnNovelShelf->setIconSize(QSize(EDITTOOLBAR_BUTTONICON_SIZE,EDITTOOLBAR_BUTTONICON_SIZE)); // 设置 图标尺寸
    m_tbtnNovelShelf->setText("书架"); // 设置 文字内容
    m_tbtnNovelShelf->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // 设置 文字在图标下方
    m_tbtnNovelShelf->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标为手形
    // m_tbtnNovelShelf->setFixedSize(EDITTOOLBAR_BUTTON_WIDTH, EDITTOOLBAR_BUTTON_HEIGHT); // 设置 固定宽度
    mainLayout->addWidget(m_tbtnNovelShelf); // 添加到 主布局

    // 目录按钮
    m_tbtnDirectory = new QToolButton(this); // 新建 工具按钮
    m_tbtnDirectory->setObjectName("EditToolBar_tbtnDirectory");
    m_tbtnDirectory->setProperty("WidgetStyleClass", "EditToolBar_ToolButton");
    // m_tbtnDirectory->setIconSize(QSize(EDITTOOLBAR_BUTTONICON_SIZE, EDITTOOLBAR_BUTTONICON_SIZE)); // 设置 图标尺寸
    m_tbtnDirectory->setText("目录"); // 设置 文字内容
    m_tbtnDirectory->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // 设置 文字在图标下方
    m_tbtnDirectory->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标为手形
    // m_tbtnDirectory->setFixedSize(EDITTOOLBAR_BUTTON_WIDTH, EDITTOOLBAR_BUTTON_HEIGHT); // 设置 固定宽度
    m_tbtnDirectory->setCheckable(true); // 设置 可“勾选”
    m_tbtnDirectory->setChecked(true); // 设置为默认按钮
    mainLayout->addWidget(m_tbtnDirectory); // 添加到 主布局


    // 撤销按钮
    m_tbtnUndo = new QToolButton(this); // 新建 工具按钮
    m_tbtnUndo->setObjectName("EditToolBar_tbtnUndo");
    m_tbtnUndo->setProperty("WidgetStyleClass", "EditToolBar_ToolButton");
    // m_tbtnUndo->setIconSize(QSize(EDITTOOLBAR_BUTTONICON_SIZE, EDITTOOLBAR_BUTTONICON_SIZE)); // 设置 图标尺寸
    m_tbtnUndo->setText("撤销"); // 设置 文字内容
    m_tbtnUndo->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // 设置 文字在图标下方
    m_tbtnUndo->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标为手形
    m_tbtnUndo->setEnabled(false); // 默认 禁止
    // m_tbtnUndo->setFixedSize(EDITTOOLBAR_BUTTON_WIDTH, EDITTOOLBAR_BUTTON_HEIGHT); // 设置 固定宽度
    mainLayout->addWidget(m_tbtnUndo); // 添加到 主布局

    // 重做按钮
    m_tbtnRedo = new QToolButton(this); // 新建 工具按钮
    m_tbtnRedo->setObjectName("EditToolBar_tbtnRedo");
    m_tbtnRedo->setProperty("WidgetStyleClass", "EditToolBar_ToolButton");
    // m_tbtnRedo->setIconSize(QSize(EDITTOOLBAR_BUTTONICON_SIZE, EDITTOOLBAR_BUTTONICON_SIZE)); // 设置 图标尺寸
    m_tbtnRedo->setText("重做"); // 设置 文字内容
    m_tbtnRedo->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // 设置 文字在图标下方
    m_tbtnRedo->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标为手形
    m_tbtnRedo->setEnabled(false); // 默认 禁止
    // m_tbtnRedo->setFixedSize(EDITTOOLBAR_BUTTON_WIDTH, EDITTOOLBAR_BUTTON_HEIGHT); // 设置 固定宽度
    mainLayout->addWidget(m_tbtnRedo); // 添加到 主布局

    // 排版按钮
    m_tbtnTypesetting = new QToolButton(this); // 新建 工具按钮
    m_tbtnTypesetting->setObjectName("EditToolBar_tbtnTypesetting");
    m_tbtnTypesetting->setProperty("WidgetStyleClass", "EditToolBar_ToolButton");
    // m_tbtnTypesetting->setIconSize(QSize(EDITTOOLBAR_BUTTONICON_SIZE, EDITTOOLBAR_BUTTONICON_SIZE)); // 设置 图标尺寸
    m_tbtnTypesetting->setText("排版"); // 设置 文字内容
    m_tbtnTypesetting->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // 设置 文字在图标下方
    m_tbtnTypesetting->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标为手形
    m_tbtnTypesetting->setEnabled(false); // 默认 禁止
    // m_tbtnTypesetting->setFixedSize(EDITTOOLBAR_BUTTON_WIDTH, EDITTOOLBAR_BUTTON_HEIGHT); // 设置 固定宽度
    mainLayout->addWidget(m_tbtnTypesetting); // 添加到 主布局

    // 查找替换按钮
    m_tbtnFindReplace = new QToolButton(this); // 新建 工具按钮
    m_tbtnFindReplace->setObjectName("EditToolBar_tbtnFindReplace");
    m_tbtnFindReplace->setProperty("WidgetStyleClass", "EditToolBar_ToolButton");
    // m_tbtnFindReplace->setIconSize(QSize(EDITTOOLBAR_BUTTONICON_SIZE, EDITTOOLBAR_BUTTONICON_SIZE)); // 设置 图标尺寸
    m_tbtnFindReplace->setText("查找替换"); // 设置 文字内容
    m_tbtnFindReplace->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // 设置 文字在图标下方
    m_tbtnFindReplace->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标为手形
    m_tbtnFindReplace->setEnabled(false); // 默认 禁止
    // m_tbtnFindReplace->setFixedSize(EDITTOOLBAR_BUTTON_WIDTH, EDITTOOLBAR_BUTTON_HEIGHT); // 设置 固定宽度
    m_tbtnFindReplace->setCheckable(true); // 设置 可“勾选”
    mainLayout->addWidget(m_tbtnFindReplace); // 添加到 主布局

    // 互斥按钮组
    m_exclusiveButtonGroup = new QButtonGroup(this); // 新建 互斥按钮组
    m_exclusiveButtonGroup->setExclusive(true); // 设置 组内按钮 互斥

    // 主题按钮
    m_tbtnTheme = new QToolButton(this); // 新建 工具按钮
    m_tbtnTheme->setObjectName("EditToolBar_tbtnTheme");
    m_tbtnTheme->setProperty("WidgetStyleClass", "EditToolBar_ToolButton");
    // m_tbtnTheme->setIconSize(QSize(EDITTOOLBAR_BUTTONICON_SIZE, EDITTOOLBAR_BUTTONICON_SIZE)); // 设置 图标尺寸
    m_tbtnTheme->setText("主题"); // 设置 文字内容
    m_tbtnTheme->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // 设置 文字在图标下方
    m_tbtnTheme->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标为手形
    // m_tbtnTheme->setFixedSize(EDITTOOLBAR_BUTTON_WIDTH, EDITTOOLBAR_BUTTON_HEIGHT); // 设置 固定宽度
    m_tbtnTheme->setCheckable(true); // 设置 可“勾选”
    m_exclusiveButtonGroup->addButton(m_tbtnTheme); // 添加到 互斥按钮组
    mainLayout->addWidget(m_tbtnTheme); // 添加到 主布局

    // 字体按钮
    m_tbtnFont = new QToolButton(this); // 新建 工具按钮
    m_tbtnFont->setObjectName("EditToolBar_tbtnFont");
    m_tbtnFont->setProperty("WidgetStyleClass", "EditToolBar_ToolButton");
    // m_tbtnFont->setIconSize(QSize(EDITTOOLBAR_BUTTONICON_SIZE, EDITTOOLBAR_BUTTONICON_SIZE)); // 设置 图标尺寸
    m_tbtnFont->setText("字体"); // 设置 文字内容
    m_tbtnFont->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // 设置 文字在图标下方
    m_tbtnFont->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标为手形
    // m_tbtnFont->setFixedSize(EDITTOOLBAR_BUTTON_WIDTH, EDITTOOLBAR_BUTTON_HEIGHT); // 设置 固定宽度
    m_tbtnFont->setCheckable(true); // 设置 可“勾选”
    m_exclusiveButtonGroup->addButton(m_tbtnFont); // 添加到 互斥按钮组
    mainLayout->addWidget(m_tbtnFont); // 添加到 主布局

    // 布局按钮
    m_tbtnLayout = new QToolButton(this); // 新建 工具按钮
    m_tbtnLayout->setObjectName("EditToolBar_tbtnLayout");
    m_tbtnLayout->setProperty("WidgetStyleClass", "EditToolBar_ToolButton");
    // m_tbtnLayout->setIconSize(QSize(EDITTOOLBAR_BUTTONICON_SIZE, EDITTOOLBAR_BUTTONICON_SIZE)); // 设置 图标尺寸
    m_tbtnLayout->setText("布局"); // 设置 文字内容
    m_tbtnLayout->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // 设置 文字在图标下方
    m_tbtnLayout->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标为手形
    // m_tbtnLayout->setFixedSize(EDITTOOLBAR_BUTTON_WIDTH, EDITTOOLBAR_BUTTON_HEIGHT); // 设置 固定宽度
    m_tbtnLayout->setCheckable(true); // 设置 可“勾选”
    m_exclusiveButtonGroup->addButton(m_tbtnLayout); // 添加到 互斥按钮组
    mainLayout->addWidget(m_tbtnLayout); // 添加到 主布局

    // 历史版本按钮
    m_tbtnHistoricalVersion = new QToolButton(this); // 新建 工具按钮
    m_tbtnHistoricalVersion->setObjectName("EditToolBar_tbtnHistoricalVersion");
    m_tbtnHistoricalVersion->setProperty("WidgetStyleClass", "EditToolBar_ToolButton");
    // m_tbtnHistoricalVersion->setIconSize(QSize(EDITTOOLBAR_BUTTONICON_SIZE, EDITTOOLBAR_BUTTONICON_SIZE)); // 设置 图标尺寸
    m_tbtnHistoricalVersion->setText("历史版本"); // 设置 文字内容
    m_tbtnHistoricalVersion->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // 设置 文字在图标下方
    m_tbtnHistoricalVersion->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标为手形
    // m_tbtnHistoricalVersion->setFixedSize(EDITTOOLBAR_BUTTON_WIDTH, EDITTOOLBAR_BUTTON_HEIGHT); // 设置 固定宽度
    m_tbtnHistoricalVersion->setCheckable(true); // 设置 可“勾选”
    m_exclusiveButtonGroup->addButton(m_tbtnHistoricalVersion); // 添加到 互斥按钮组
    mainLayout->addWidget(m_tbtnHistoricalVersion); // 添加到 主布局


    // 更多设置按钮
    m_tbtnMoreSettings = new QToolButton(this); // 新建 工具按钮
    m_tbtnMoreSettings->setObjectName("EditToolBar_tbtnMoreSettings");
    m_tbtnMoreSettings->setProperty("WidgetStyleClass", "EditToolBar_ToolButton");
    // m_tbtnMoreSettings->setIconSize(QSize(EDITTOOLBAR_BUTTONICON_SIZE, EDITTOOLBAR_BUTTONICON_SIZE)); // 设置 图标尺寸
    m_tbtnMoreSettings->setText("更多设置"); // 设置 文字内容
    m_tbtnMoreSettings->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // 设置 文字在图标下方
    m_tbtnMoreSettings->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标为手形
    // m_tbtnMoreSettings->setFixedSize(EDITTOOLBAR_BUTTON_WIDTH, EDITTOOLBAR_BUTTON_HEIGHT); // 设置 固定宽度
    m_tbtnMoreSettings->setCheckable(true); // 设置 可“勾选”
    m_exclusiveButtonGroup->addButton(m_tbtnMoreSettings); // 添加到 互斥按钮组
    mainLayout->addWidget(m_tbtnMoreSettings); // 添加到 主布局

    // 添加弹簧
    mainLayout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed));

}
// 初始化 绑定
void EditToolBar::initConnect()
{
    // 绑定按钮点击
    connect(m_tbtnNovelShelf, &QToolButton::clicked, this, [=](){
        // emit editToolBarButton_clicked(EditToolBarButton::ShelfButton);
        emit returnHomeMainWindow();
    });

    // 目录
    connect(m_tbtnDirectory, &QToolButton::clicked, this, [=](bool checked){
        emit editToolBarButton_clicked(EditToolBarButton::CatalogButton, checked);
    });

    // 撤销、重做
    connect(m_tbtnUndo, &QToolButton::clicked, this, [=](){
        emit editToolBarButton_clicked(EditToolBarButton::UndoButton);
    });
    connect(m_tbtnRedo, &QToolButton::clicked, this, [=](){
        emit editToolBarButton_clicked(EditToolBarButton::RedoButton);
    });

    // 自动排版、查找替换
    connect(m_tbtnTypesetting, &QToolButton::clicked, [=](){
        emit editToolBarButton_clicked(EditToolBarButton::TypeSettingButton);
    });
    // 查找
    connect(m_tbtnFindReplace, &QToolButton::pressed, [=](){


        // 获取 以前的状态
        bool isChecked = m_tbtnFindReplace->isChecked();

        // 将 状态取反，并发送
        emit editToolBarButton_clicked(EditToolBarButton::FindReplaceButtton, !isChecked);
    });

    // 绑定 互斥按钮组 按钮点击
    connect(m_tbtnTheme, &QToolButton::pressed, this, &EditToolBar::do_exclusiveButton_clicked);
    connect(m_tbtnFont, &QToolButton::pressed, this, &EditToolBar::do_exclusiveButton_clicked);
    connect(m_tbtnLayout, &QToolButton::pressed, this, &EditToolBar::do_exclusiveButton_clicked);
    connect(m_tbtnHistoricalVersion, &QToolButton::pressed, this, &EditToolBar::do_exclusiveButton_clicked);
    connect(m_tbtnMoreSettings, &QToolButton::pressed, this, &EditToolBar::do_exclusiveButton_clicked);
}

// 设置 和 获取 背景颜色
void EditToolBar::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
QColor EditToolBar::getBackgroundColor() const
{
    return m_backgroundColor;
}

// 绘制 背景
void EditToolBar::drawBackground(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    painter->setBrush(QBrush(m_backgroundColor)); // 设置 画刷

    painter->drawRect(QRect(0,0,this->width(),this->height())); // 绘制 控件尺寸 的矩形

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}

// 绘画事件
void EditToolBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿
    //m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
}

// 互斥组按钮 点击 处理槽函数
void EditToolBar::do_exclusiveButton_clicked()
{
    // 获取 发送信号的按钮
    QToolButton *btn = qobject_cast<QToolButton *>(sender()); // 获取被点击的按钮

    // 重新设置可选中属性
    btn->setCheckable(true);
    // 获取 按钮状态
    bool isChecked = btn->isChecked();

    if(isChecked) // 已经被“勾选”
    {
        // 互斥按钮组 取消互斥，不取消互斥则必须要另一按钮代替被“勾选”
        m_exclusiveButtonGroup->setExclusive(false);
        // 按钮 设置 取消“勾选”
        btn->setCheckable(false); // 无法使用setChecked取消选中
        // 重新 设置互斥按钮组 可互斥
        m_exclusiveButtonGroup->setExclusive(true);
    }
    else // 之前未被选中
    {
        btn->setChecked(true);
    }

    // 发送 信号
    if(btn == m_tbtnTheme)
        emit editToolBarButton_clicked(EditToolBarButton::ThemeButton, !isChecked);
    else if(btn == m_tbtnFont)
        emit editToolBarButton_clicked(EditToolBarButton::FontButton, !isChecked);
    else if(btn == m_tbtnLayout)
        emit editToolBarButton_clicked(EditToolBarButton::LayoutButton, !isChecked);
    else if(btn == m_tbtnHistoricalVersion)
        emit editToolBarButton_clicked(EditToolBarButton::HistoricalButton, !isChecked);
    else if(btn == m_tbtnMoreSettings)
        emit editToolBarButton_clicked(EditToolBarButton::SettingsButton, !isChecked);
}

// 打开 章节 处理槽函数
void EditToolBar::do_openChapter(QString openChapterID, QString openChapterTitle)
{
    Q_UNUSED(openChapterID);
    Q_UNUSED(openChapterTitle);

    // 使能 撤销、重做、自动排版、查找替换
    m_tbtnUndo->setEnabled(true);
    m_tbtnRedo->setEnabled(true);
    m_tbtnTypesetting->setEnabled(true);
    m_tbtnFindReplace->setEnabled(true);
}

// 页面关闭 处理 槽函数
void EditToolBar::do_editBarPages_close(EditToolBarButton toolButton)
{
    switch (toolButton) {
    case EditToolBarButton::CatalogButton: // 关闭 目录页面
        emit m_tbtnDirectory->pressed();
        break;
    case EditToolBarButton::FindReplaceButtton: // 关闭 查找替换页面
        emit m_tbtnFindReplace->pressed();
        break;
    case EditToolBarButton::ThemeButton: // 关闭 主题页面
        emit m_tbtnTheme->pressed();
        break;
    case EditToolBarButton::FontButton: // 关闭 字体页面
        emit m_tbtnFont->pressed();
        break;
    case EditToolBarButton::LayoutButton: // 关闭 布局页面
        emit m_tbtnLayout->pressed();
        break;
    case EditToolBarButton::HistoricalButton: // 关闭 历史页面
        emit m_tbtnHistoricalVersion->pressed();
        break;
    case EditToolBarButton::SettingsButton: // 关闭 设置页面
        emit m_tbtnMoreSettings->pressed();
        break;
    case EditToolBarButton::ShelfButton: // 其他 不用处理
    case EditToolBarButton::UndoButton:
    case EditToolBarButton::RedoButton:
    case EditToolBarButton::TypeSettingButton:
        break;
    default:
        break;
    }

    // 更新
    update();
}

// 文本编辑编辑页 关闭 处理槽函数
void EditToolBar::do_editWorkspace_noFile()
{
    // 禁用 撤销、重做、自动排版、查找替换
    m_tbtnUndo->setEnabled(false);
    m_tbtnRedo->setEnabled(false);
    m_tbtnTypesetting->setEnabled(false);
    m_tbtnFindReplace->setEnabled(false);
}


