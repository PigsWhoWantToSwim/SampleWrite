#include "BasedSiderBarPage.h"

// 构造函数
BasedSiderBarPage::BasedSiderBarPage(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    m_backgroundColor = QColor(255, 255, 255);

    // 主布局，垂直布局，第一区为 侧边栏标题区，第二区为 章节标题区，第三区为 章节标题区
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    this->setLayout(mainLayout);

    // 侧边栏标题区，水平布局，左边为 侧边栏标题，右边为 关闭按钮
    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(10,10,10,10);
    mainLayout->addLayout(titleLayout); // 将 侧边栏标题区 添加到 主布局

    // 添加弹簧
    titleLayout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed));

    // 侧边栏标题
    m_labTitle = new QLabel(this);
    m_labTitle->setProperty("WidgetStyleClass", "SiderBarPage_TitleLabel");
    m_labTitle->setAlignment(Qt::AlignCenter);
    m_labTitle->setText("侧边栏页面");
    titleLayout->addWidget(m_labTitle);

    // 添加弹簧
    titleLayout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed));

    // 关闭按钮
    m_btnClose = new QPushButton(this);
    m_btnClose->setProperty("WidgetStyleClass", "SiderBarPage_CloseButton");
    m_btnClose->setCursor(QCursor(Qt::PointingHandCursor));
    m_btnClose->setToolTip("关闭页面");
    connect(m_btnClose, &QPushButton::clicked, this, [=](){
        emit closePage();
    });
    titleLayout->addWidget(m_btnClose);


    // 章节标题编辑框
    m_frmContainer = new QFrame(this);
    m_frmContainer->setProperty("WidgetStyleClass", "SiderBarPage_ContainerFrame");
    mainLayout->addWidget(m_frmContainer);


}
// 设置 背景颜色
void BasedSiderBarPage::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
// 获取 背景颜色
QColor BasedSiderBarPage::getBackgroundColor() const
{
    return m_backgroundColor;
}

// 设置 和 获取 侧边栏页面 标题
void BasedSiderBarPage::setSideBarPageTitle(QString title)
{
    m_labTitle->setText(title);
    update();
}
QString BasedSiderBarPage::getSideBarPageTitle() const
{
    return m_labTitle->text();
}

// 绘制 背景
void BasedSiderBarPage::drawBackground(QPainter *painter)
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
void BasedSiderBarPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿
    //m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
}


