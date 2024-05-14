#include "BlankPage.h"

/********************************************************************
 ****************   编辑工作区 空白页   ******************************
*********************************************************************/

BlankPage::BlankPage(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void BlankPage::initMemberVariable()
{
    m_backgroundColor = QColor(245, 246, 249);
    m_tipsText = "暂无内容";
}
// 初始化 布局
void BlankPage::initLayout()
{
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setVerticalSpacing(10);
    mainLayout->setHorizontalSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    this->setLayout(mainLayout);

    // 添加 弹簧
    mainLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Fixed, QSizePolicy::Expanding),0,0,1,3);
    mainLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding, QSizePolicy::Fixed),1,0,2,1);


    // 提示图片标签
    m_labImage = new QLabel(this);
    m_labImage->setProperty("WidgetStyleClass", "BlankPage_ImageLabel");
    // QPixmap imgPaper(":/image/paper - green.png");
    // imgPaper = imgPaper.scaled(QSize(80,80),Qt::KeepAspectRatio);
    // m_labImage->setPixmap(imgPaper);
    m_labImage->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_labImage,1,1,1,1);

    // 提示文本标签
    m_labTips = new QLabel(m_tipsText, this);
    m_labTips->setProperty("WidgetStyleClass", "BlankPage_TipsLabel");
    m_labTips->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(m_labTips,2,1,1,1);

    // 添加 弹簧
    mainLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding, QSizePolicy::Fixed),1,2,2,1);
    mainLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Fixed, QSizePolicy::Expanding),3,0,1,3);

}
// 初始化 绑定
void BlankPage::initConnect()
{

}

// 设置 和 获取 背景颜色
void BlankPage::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
QColor BlankPage::getBackgroundColor() const
{
    return m_backgroundColor;
}
// 设置 和 获取 提示文本
void BlankPage::setTipsText(const QString tipsText)
{
    m_tipsText = tipsText;
    m_labTips->setText(tipsText);
    update();
}
QString BlankPage::getTipsText() const
{
    return m_tipsText;
}

// 绘制 背景
void BlankPage::drawBackground(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    painter->setBrush(QBrush(m_backgroundColor)); // 设置 画刷

    painter->drawRect(QRect(0,0,this->width(),this->height())); // 绘制 控件尺寸 的矩形

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}

// 重写 绘画事件
void BlankPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿
    //m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
}

