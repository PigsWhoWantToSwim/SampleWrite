#include "EditStatusBar.h"

EditStatusBar::EditStatusBar(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void EditStatusBar::initMemberVariable()
{
    this->setFixedHeight(EDITSTATUSBAR_HEIGHT);

    m_backgroundColor = QColor(255, 255, 255);
    m_lineColor = QColor(245, 245, 249);

    m_wordCount = 0;
}
// 初始化 布局
void EditStatusBar::initLayout()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,2,20,0);
    this->setLayout(mainLayout);

    // 添加 弹簧
    mainLayout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding, QSizePolicy::Fixed));

    // 字数标签
    m_labWordCount = new QLabel("",this);
    m_labWordCount->setObjectName("EditStatusBar_labWordCount");
    m_labWordCount->setAlignment(Qt::AlignRight|Qt::AlignVCenter); // 设置 右对齐，垂直方向居中
    mainLayout->addWidget(m_labWordCount);
}
// 初始化 绑定
void EditStatusBar::initConnect()
{

}

// 设置 背景颜色
void EditStatusBar::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
// 获取 背景颜色
QColor EditStatusBar::getBackgroundColor() const
{
    return m_backgroundColor;
}
// 设置 线条颜色
void EditStatusBar::setLineColor(const QColor &lineColor)
{
    m_lineColor = lineColor;
    update();
}
// 获取 线条颜色
QColor EditStatusBar::getLineColor() const
{
    return m_lineColor;
}

// 设置 和 获取 字数
void EditStatusBar::setWordCount(int wordCount)
{
    m_wordCount = wordCount;
    m_labWordCount->setText(QString::number(wordCount)+"字");
    update();
}

int EditStatusBar::getWordCount() const
{
    return m_wordCount;
}

// 绘制 背景
void EditStatusBar::drawBackground(QPainter *painter)
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
void EditStatusBar::drawBorder(QPainter *painter)
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

    // QPoint leftBottom(0,this->height());
    QPoint leftTop(0,0);
    QPoint rightTop(this->width(),0);
    // QPoint rightBottom(this->width(),this->height());

    // painter->drawLine(leftBottom, leftTop);
    painter->drawLine(leftTop, rightTop);
    // painter->drawLine(rightTop, rightBottom);

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}


// 重写绘画事件，绘制背景
void EditStatusBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿
    //m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
    drawBorder(&painter); // 绘制 边框
}

// 文本编辑编辑页 关闭 处理槽函数
void EditStatusBar::do_editWorkspace_noFile()
{
    m_wordCount = 0;
    m_labWordCount->setText("");
    update();
}
