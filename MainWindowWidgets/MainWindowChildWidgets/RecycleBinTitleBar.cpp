#include "RecycleBinTitleBar.h"

RecycleBinTitleBar::RecycleBinTitleBar(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员函数
void RecycleBinTitleBar::initMemberVariable()
{
    this->setFixedHeight(HOMERECYCLEPAGE_TITLEBAR_HEIGHT);

    m_backgroundColor = QColor(245, 246, 249);
    m_lineColor = QColor(205, 205, 205);

    m_checkState = Qt::Unchecked;
}
// 初始化 布局
void RecycleBinTitleBar::initLayout()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 5);
    this->setLayout(mainLayout);

    m_ckbSelect = new QCheckBox(this);
    m_ckbSelect->setObjectName("RecycleBinTitleBar_ckbSelect");
    m_ckbSelect->setCheckState(m_checkState);
    m_ckbSelect->setText("");
    m_ckbSelect->setCursor(QCursor(Qt::PointingHandCursor));
    mainLayout->addWidget(m_ckbSelect);

    QLabel *labWorkTitle = new QLabel("作品名称", this);
    labWorkTitle->setProperty("WidgetStyleClass", "RecycleBin_HeadLabel");
    //labWorkTitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(labWorkTitle);

    // 作品类型
    QLabel *labWorkType = new QLabel("类型", this);
    labWorkType->setProperty("WidgetStyleClass", "RecycleBin_HeadLabel");
    //labWorkType->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(labWorkType);

    // 删除时间
    QLabel *labDeleteTime = new QLabel("删除时间", this);
    labDeleteTime->setProperty("WidgetStyleClass", "RecycleBin_HeadLabel");
    //labDeleteTime->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(labDeleteTime);

    // 剩余时间
    QLabel *labRemaningTime = new QLabel("剩余", this);
    labRemaningTime->setProperty("WidgetStyleClass", "RecycleBin_HeadLabel");
    //labRemaningTime->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(labRemaningTime);

    // 字数
    QLabel *labWorkCount = new QLabel("字数", this);
    labWorkCount->setProperty("WidgetStyleClass", "RecycleBin_HeadLabel");
    //labWorkCount->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(labWorkCount);

    // 所属的小说
    QLabel *labBelongsNovel = new QLabel("所属作品", this);
    labBelongsNovel->setProperty("WidgetStyleClass", "RecycleBin_HeadLabel");
    //labBelongsNovel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(labBelongsNovel);

}
// 初始化 绑定事件
void RecycleBinTitleBar::initConnect()
{
    connect(m_ckbSelect, &QCheckBox::clicked, this, [=](bool checked){
        if(m_ckbSelect->checkState() == Qt::PartiallyChecked) // 防止 点击时出现半选
            m_ckbSelect->setCheckState(Qt::Checked);
        emit recycleTitleBar_checkStateChanged(checked);
    });
}

// 设置 和 获取 背景颜色
void RecycleBinTitleBar::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
QColor RecycleBinTitleBar::getBackgroundColor() const
{
    return m_backgroundColor;
}
// 设置 和 获取 线条颜色
void RecycleBinTitleBar::setLineColor(const QColor &lineColor)
{
    m_lineColor = lineColor;
    update();
}
QColor RecycleBinTitleBar::getLineColor() const
{
    return m_lineColor;
}
// 设置 和 获取 复选框状态
void RecycleBinTitleBar::setCheckState(const Qt::CheckState checkState)
{
    m_checkState = checkState;
    m_ckbSelect->setCheckState(checkState);
    update();
}
Qt::CheckState RecycleBinTitleBar::getCheckState() const
{
    return m_checkState;
}

// 绘制背景
void RecycleBinTitleBar::drawBackground(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    painter->setBrush(QBrush(m_backgroundColor)); // 设置 画刷

    painter->drawRect(QRect(0,0,this->width(),this->height())); // 绘制 控件尺寸 的矩形

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}
// 绘制线条
void RecycleBinTitleBar::drawLines(QPainter *painter)
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
    // QPoint leftTop(0,0);
    // QPoint rightTop(this->width(),0);
    QPoint rightBottom(this->width(),this->height());

    painter->drawLine(leftBottom, rightBottom);

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}


void RecycleBinTitleBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿
    //m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
    drawLines(&painter); // 绘制 边框
}
