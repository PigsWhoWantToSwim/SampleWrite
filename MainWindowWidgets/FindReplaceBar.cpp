#include "FindReplaceBar.h"

/***********************************************************
 *******************      查找栏      ******************
***********************************************************/

// 构造函数
FindBar::FindBar(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void FindBar::initMemberVariable()
{
    this->setFixedHeight(FINDBAR_HEIGHT);

    m_backgroundColor = QColor(240, 240, 240);
    m_lineColor = QColor(245, 245, 249);
}
// 初始化 布局
void FindBar::initLayout()
{
    // 主布局，水平布局，左侧为 搜索区，右侧为 按钮区
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(FINDBAR_LAYOUT_SPACING);
    mainLayout->setContentsMargins(FINDBAR_LAYOUT_LEFTMARGIN,
                                   FINDBAR_LAYOUT_TOPMARGIN,
                                   FINDBAR_LAYOUT_RIGHTMARGIN,
                                   FINDBAR_LAYOUT_BOTTOMMARGIN);
    this->setLayout(mainLayout); // 将 主布局 设置为 控件布局

    // 搜索区，水平布局，左侧为 搜索输入框，右侧为 搜索按钮
    QHBoxLayout *searchBoxLayout = new QHBoxLayout;
    searchBoxLayout->setSpacing(0);
    searchBoxLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(searchBoxLayout);

    // 查找输入框
    m_ledFind = new QLineEdit(this);
    m_ledFind->setObjectName("FindBar_ledFind");
    m_ledFind->setPlaceholderText("请输入查找内容");
    m_ledFind->setClearButtonEnabled(true); // 清空输入按钮
    m_ledFind->setFixedHeight(FINDBAR_LINEEDIT_HEIGHT);
    searchBoxLayout->addWidget(m_ledFind);
    // 搜索按钮
    m_btnFind = new QPushButton(this);
    m_btnFind->setObjectName("FindBar_btnFind");
    m_btnFind->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_btnFind->setEnabled(false); // 默认 禁止
    m_btnFind->setFixedSize(FINDBAR_SEARCHBUTTON_WIDTH, FINDBAR_LINEEDIT_HEIGHT);
    searchBoxLayout->addWidget(m_btnFind);

    mainLayout->addSpacing(15);

    // 按钮区，水平布局，从左开始依次是 查找上一个按钮、查找下一个按钮、关闭页面按钮，替换页面开关按钮
    QHBoxLayout *findButtonLayout = new QHBoxLayout;
    findButtonLayout->setSpacing(4);
    findButtonLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(findButtonLayout);

    // 查找上一个按钮
    m_btnFindPrevious = new QPushButton(this);
    m_btnFindPrevious->setObjectName("FindBar_btnFindPrevious");
    m_btnFindPrevious->setProperty("WidgetStyleClass", "FindBar_SquareButton");
    m_btnFindPrevious->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_btnFindPrevious->setEnabled(false); // 默认 禁止
    m_btnFindPrevious->setFixedSize(FINDBAR_BUTTON_SIZE, FINDBAR_BUTTON_SIZE);
    findButtonLayout->addWidget(m_btnFindPrevious);

    // 查找下一个按钮
    m_btnFindNext = new QPushButton(this);
    m_btnFindNext->setObjectName("FindBar_btnFindNext");
    m_btnFindNext->setProperty("WidgetStyleClass", "FindBar_SquareButton");
    m_btnFindNext->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_btnFindNext->setEnabled(false); // 默认 禁止
    m_btnFindNext->setFixedSize(FINDBAR_BUTTON_SIZE, FINDBAR_BUTTON_SIZE);
    findButtonLayout->addWidget(m_btnFindNext);


    // 添加间隔
    // findButtonLayout->addSpacing(27);
    findButtonLayout->addSpacing(4+FINDBAR_BUTTON_SIZE+7);

    // 替换区开关按钮
    m_swbtnReplaceVisible = new SwitchButton(this);
    m_swbtnReplaceVisible->setObjectName("FindBar_swbtnReplaceVisible"); // 设置 对象名
    m_swbtnReplaceVisible->setButtonStyle(SwitchButtonStyle::circleInButton);
    m_swbtnReplaceVisible->setSpace(3); // 设置 滑块与滑槽的 间距
    m_swbtnReplaceVisible->setChecked(false); // 设置 开关按钮 默认 为关闭状态
    m_swbtnReplaceVisible->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_swbtnReplaceVisible->setFixedSize(FINDBAR_SWITCHBUTTON_WIDTH, FINDBAR_LINEEDIT_HEIGHT); // 设置 固定尺寸
    findButtonLayout->addWidget(m_swbtnReplaceVisible);

}
// 初始化 绑定
void FindBar::initConnect()
{
    // 绑定 替换页面开关按钮
    connect(m_swbtnReplaceVisible, &SwitchButton::checkedChanged,this,[=](bool checked){
        emit replaceBarVisible_changed(checked);
    });

    // 绑定 搜索内容
    connect(m_btnFind,&QPushButton::clicked,this,[=](){
        if(m_ledFind->text().trimmed() != "") // 文本不为空
        {
            emit searchText(m_ledFind->text().trimmed()); // 发送要搜索的文本
            // 设置按钮使能
            m_btnFindPrevious->setEnabled(true);
            m_btnFindNext->setEnabled(true);
        }
    });
    // 绑定 查找上一个
    connect(m_btnFindPrevious,&QPushButton::clicked,this,[=](){
        emit searchText_previous();
    });
    // 绑定 查找上一个
    connect(m_btnFindNext,&QPushButton::clicked,this,[=](){
        emit searchText_next();
    });

    // 绑定 搜索输入框 文本变化，输入框为空时，自动取消查找状态
    connect(m_ledFind,&QLineEdit::textChanged,this,[=](QString findText){
        if(findText.trimmed() == "")
        {
            // 文本内容为空
            emit searchText_cleared(); // 发送 搜索内容 清空信号
            // 设置 按钮使能禁止
            m_btnFind->setEnabled(false);
            m_btnFindPrevious->setEnabled(false);
            m_btnFindNext->setEnabled(false);
        }
        else
        {
            // 查找内容 不为空
            emit searchText_changed(); // 发送 搜索内容 改变信号
            // 设置 按钮使能禁止
            m_btnFind->setEnabled(true);
            m_btnFindPrevious->setEnabled(false);
            m_btnFindNext->setEnabled(false);
        }
    });
}

// 设置 和 获取 背景颜色
void FindBar::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
QColor FindBar::getBackgroundColor() const
{
    return m_backgroundColor;
}
// 设置 和 获取 线条颜色
void FindBar::setLineColor(const QColor &lineColor)
{
    m_lineColor = lineColor;
    update();
}
QColor FindBar::getLineColor() const
{
    return m_lineColor;
}


// 绘制 背景
void FindBar::drawBackground(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    painter->setBrush(QBrush(m_backgroundColor)); // 设置 画刷

    painter->drawRect(QRect(0,0,this->width(),this->height())); // 绘制 控件尺寸 的矩形

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 绘制边框
void FindBar::drawLines(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();

    // 绘制 左侧 顶部 右侧线条
    QPen pen;
    pen.setWidth(3);
    pen.setColor(m_lineColor);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);

    QPoint leftBottom(0,this->height());
    QPoint leftTop(0,0);
    QPoint rightTop(this->width(),0);
    QPoint rightBottom(this->width(),this->height());

    painter->drawLine(leftBottom, leftTop);
    painter->drawLine(rightTop, rightBottom);

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}

// 绘画事件
void FindBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿
    //m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
    drawLines(&painter); // 绘制 边框
}

// 清空 查找输入框 的内容
void FindBar::do_findText_cleared()
{
    m_ledFind->clear();
}

// 工具栏按钮点击 处理槽函数
void FindBar::do_editToolBarButton_clicked(EditToolBarButton toolButton, bool checked)
{
    // 进行 显示 或 隐藏 查找替换栏
    if(toolButton == EditToolBarButton::FindReplaceButtton)
    {
        if(checked) // 显示
        {
            // 显示 查找页面
            this->setVisible(true);
            // 根据 替换开关按钮 设置 替换页面 显示隐藏
            emit replaceBarVisible_changed(m_swbtnReplaceVisible->getChecked());
        }
        else // 全部 隐藏
        {
            // 清空 查找输入框的内容
            this->do_findText_cleared();
            // 隐藏 查找页面
            this->setVisible(false);
            // 隐藏 替换页面
            emit replaceBarVisible_changed(false);
        }
    }
}


/***********************************************************
 *******************      替换栏      ******************
***********************************************************/

ReplaceBar::ReplaceBar(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void ReplaceBar::initMemberVariable()
{
    this->setFixedHeight(REPLACEBAR_HEIGHT);

    m_backgroundColor = QColor(240, 240, 240);
    m_lineColor = QColor(245, 245, 249);
}
// 初始化 布局
void ReplaceBar::initLayout()
{
    // 主布局，水平布局，左侧为 替换输入区，右侧为 按钮区
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(REPLACEBAR_LAYOUT_SPACING);
    mainLayout->setContentsMargins(REPLACEBAR_LAYOUT_LEFTMARGIN,
                                   REPLACEBAR_LAYOUT_TOPMARGIN,
                                   REPLACEBAR_LAYOUT_RIGHTMARGIN,
                                   REPLACEBAR_LAYOUT_BOTTOMMARGIN);
    this->setLayout(mainLayout);



    m_ledReplace = new QLineEdit(this);
    m_ledReplace->setObjectName("ReplaceBar_ledReplace");
    m_ledReplace->setPlaceholderText("请输入替换内容");
    m_ledReplace->setEnabled(false); // 默认禁止 点击搜索后 使能
    m_ledReplace->setClearButtonEnabled(true); // 清空输入按钮
    m_ledReplace->setFixedHeight(REPLACEBAR_LINEEDIT_HEIGHT);
    mainLayout->addWidget(m_ledReplace);


    mainLayout->addSpacing(15);


    // 替换当前 按钮
    m_btnReplaceCurrent = new QPushButton(this);
    m_btnReplaceCurrent->setProperty("WidgetStyleClass", "ReplaceBar_ReplaceButton");
    m_btnReplaceCurrent->setText("替换当前");
    m_btnReplaceCurrent->setEnabled(false); // 按钮默认禁止
    m_btnReplaceCurrent->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_btnReplaceCurrent->setFixedSize(REPLACEBAR_BUTTON_WIDTH, REPLACEBAR_BUTTON_HEIGHT);
    mainLayout->addWidget(m_btnReplaceCurrent);

    mainLayout->addSpacing(5);

    // 替换全部 按钮
    m_btnReplaceAll = new QPushButton(this);
    m_btnReplaceAll->setProperty("WidgetStyleClass", "ReplaceBar_ReplaceButton");
    m_btnReplaceAll->setText("替换全部");
    m_btnReplaceAll->setEnabled(false); // 按钮默认禁止
    m_btnReplaceAll->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_btnReplaceAll->setFixedSize(REPLACEBAR_BUTTON_WIDTH, REPLACEBAR_BUTTON_HEIGHT);
    mainLayout->addWidget(m_btnReplaceAll);

}
// 初始化 绑定
void ReplaceBar::initConnect()
{
    // 绑定 替换输入框 文本内容变化
    connect(m_ledReplace,&QLineEdit::textChanged,this,&ReplaceBar::do_replaceText_changed);

    // 绑定 替换当前按钮
    connect(m_btnReplaceCurrent,&QPushButton::clicked,this,[=](){
        // 获取 替换输入框的文本内容，并去掉前后空格
        QString replaceText = m_ledReplace->text().trimmed();
        if(!replaceText.isEmpty()) // 替换内容 不为空
            emit replaceCurrent(replaceText); // 发送 替换当前信号
    });
    // 绑定 替换全部按钮
    connect(m_btnReplaceAll,&QPushButton::clicked,this,[=](){
        // 获取 替换输入框的文本内容，并去掉前后空格
        QString replaceText = m_ledReplace->text().trimmed();
        if(!replaceText.isEmpty()) // 替换内容 不为空
            emit replaceAll(replaceText); // 发送 替换当前信号
    });
}

// 设置 和 获取 背景颜色
void ReplaceBar::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
QColor ReplaceBar::getBackgroundColor() const
{
    return m_backgroundColor;
}
// 设置 和 获取 线条颜色
void ReplaceBar::setLineColor(const QColor &lineColor)
{
    m_lineColor = lineColor;
    update();
}
QColor ReplaceBar::getLineColor() const
{
    return m_lineColor;
}

// 设置 替换输入框 使能
void ReplaceBar::setLineEditReplaceEnabled(bool enabledFlag)
{
    m_ledReplace->setEnabled(enabledFlag);
}
// 绘制背景
void ReplaceBar::drawBackground(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    painter->setBrush(QBrush(m_backgroundColor)); // 设置 画刷

    painter->drawRect(QRect(0,0,this->width(),this->height())); // 绘制 控件尺寸 的矩形

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 绘制边框
void ReplaceBar::drawLines(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();

    // 绘制 左侧 顶部 右侧线条
    QPen pen;
    pen.setWidth(3);
    pen.setColor(m_lineColor);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);

    QPoint leftBottom(0,this->height());
    QPoint leftTop(0,0);
    QPoint rightTop(this->width(),0);
    QPoint rightBottom(this->width(),this->height());

    painter->drawLine(leftBottom, leftTop);
    painter->drawLine(rightTop, rightBottom);

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 绘画事件
void ReplaceBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿
    //m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
    drawLines(&painter); // 绘制 边框
}

// 替换输入框 内容变化 处理槽函数
void ReplaceBar::do_replaceText_changed(QString replaceText)
{
    if(replaceText.trimmed() != "") // 替换输入框的文本 去掉 前后空格后 不为空
    {
        // 按钮使能
        m_btnReplaceCurrent->setEnabled(true);
        m_btnReplaceAll->setEnabled(true);
    }
    else
    {
        // 按钮禁止
        m_btnReplaceCurrent->setEnabled(false);
        m_btnReplaceAll->setEnabled(false);
    }
}

// 替换栏 显示隐藏 处理槽函数
void ReplaceBar::do_replaceBarVisible_changed(bool checked)
{
    this->setVisible(checked);
}

// 清空 替换输入框 内容
void ReplaceBar::do_lineEditReplaceText_cleared()
{
    m_ledReplace->clear();
}




