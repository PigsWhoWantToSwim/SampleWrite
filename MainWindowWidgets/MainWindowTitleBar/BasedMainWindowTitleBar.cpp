#include "basedmainwindowtitlebar.h"

BasedMainWindowTitleBar::BasedMainWindowTitleBar(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
}

BasedMainWindowTitleBar::~BasedMainWindowTitleBar()
{

}

// 初始化 成员变量
void BasedMainWindowTitleBar::initMemberVariable()
{
    this->setProperty("WidgetType","MainWindowTitleBar");
    this->setAttribute(Qt::WA_StyledBackground);
    m_backgroundColor = QColor(27, 178, 139);
}

// 设置 背景颜色
void BasedMainWindowTitleBar::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
// 获取 背景颜色
QColor BasedMainWindowTitleBar::getBackgroundColor() const
{
    return m_backgroundColor;
}

// 绘制 背景
void BasedMainWindowTitleBar::drawBackground(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    painter->setBrush(QBrush(m_backgroundColor)); // 设置 画刷

    painter->drawRect(QRect(0,0,this->width(),this->height())); // 绘制 控件尺寸 的矩形

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}

// 重写绘画事件，绘制背景
void BasedMainWindowTitleBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿
    //m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
}






