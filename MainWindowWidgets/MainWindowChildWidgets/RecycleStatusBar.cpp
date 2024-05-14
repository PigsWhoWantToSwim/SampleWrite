#include "RecycleStatusBar.h"

RecycleStatusBar::RecycleStatusBar(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员函数
void RecycleStatusBar::initMemberVariable()
{
    this->setFixedHeight(HOMERECYCLEPAGE_STATUSBAR_HEIGHT);

    m_backgroundColor = QColor(255, 255, 255);
    m_lineColor = QColor(245, 246, 249);

    m_selectedCount = 0;
    m_totalCount = 0;

}
// 初始化 布局
void RecycleStatusBar::initLayout()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    // 添加弹簧
    mainLayout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed));

    m_labSelectedCount = new QLabel(this);
    m_labSelectedCount->setObjectName("RecycleStatusBar_labSelectedCount");
    m_labSelectedCount->setText(QString("已选 %1 / %2").arg(m_selectedCount).arg(m_totalCount));
    mainLayout->addWidget(m_labSelectedCount);

    m_btnRestore = new QPushButton(this);
    m_btnRestore->setObjectName("RecycleStatusBar_btnRestore");
    m_btnRestore->setText(QString("恢复%1项").arg(m_selectedCount));
    m_btnRestore->setCursor(QCursor(Qt::PointingHandCursor));
    m_btnRestore->setEnabled(false); // 默认禁止
    mainLayout->addWidget(m_btnRestore);

    m_btnDelete = new QPushButton(this);
    m_btnDelete->setObjectName("RecycleStatusBar_btnDelete");
    m_btnDelete->setText(QString("彻底删除%1项").arg(m_selectedCount));
    m_btnDelete->setCursor(QCursor(Qt::PointingHandCursor));
    m_btnDelete->setEnabled(false); // 默认禁止
    mainLayout->addWidget(m_btnDelete);

    mainLayout->addSpacing(20);
}
// 初始化 绑定事件
void RecycleStatusBar::initConnect()
{
    // 绑定 恢复按钮 和 彻底删除按钮
    connect(m_btnRestore, &QPushButton::clicked, this, [=](){
        if(m_selectedCount > 0)
            emit btnRestore_clicked();
    });
    connect(m_btnDelete, &QPushButton::clicked, this, [=](){
        if(m_selectedCount > 0)
        {
            // 打开 对话框
            QMessageBox::StandardButton resultBtn = QMessageBox::warning(this, "删除作品",
                                                                         QString("确认删除 %1 项作品吗？").arg(m_selectedCount),
                                                                         QMessageBox::Cancel|QMessageBox::Yes);

            // 点击 取消按钮
            if(resultBtn == QMessageBox::Cancel)
                return; // 直接结束，不做删除操作
            emit btnDelete_clicked();
        }
    });
}

// 设置 和 获取 背景颜色
void RecycleStatusBar::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
QColor RecycleStatusBar::getBackgroundColor() const
{
    return m_backgroundColor;
}
// 设置 和 获取 线条颜色
void RecycleStatusBar::setLineColor(const QColor &lineColor)
{
    m_lineColor = lineColor;
    update();
}
QColor RecycleStatusBar::getLineColor() const
{
    return m_lineColor;
}
// 设置 和 获取 被选中项 数目
void RecycleStatusBar::setSelectCount(const int selectCount)
{
    m_selectedCount = selectCount;
    m_labSelectedCount->setText(QString("已选 %1 / %2").arg(m_selectedCount).arg(m_totalCount));
    m_btnRestore->setText(QString("恢复%1项").arg(m_selectedCount));
    m_btnDelete->setText(QString("彻底删除%1项").arg(m_selectedCount));

    // 设置 使能禁止
    m_btnRestore->setEnabled(m_selectedCount>0? true:false);
    m_btnDelete->setEnabled(m_selectedCount>0? true:false);

    update();
}
int RecycleStatusBar::getSelectCount() const
{
    return m_selectedCount;
}
// 设置 和 获取 全部项 数目
void RecycleStatusBar::setTotalCount(const int totalCount)
{
    m_totalCount = totalCount;
    m_labSelectedCount->setText(QString("已选 %1 / %2").arg(m_selectedCount).arg(m_totalCount));
    update();
}
int RecycleStatusBar::getTotalCount() const
{
    return m_totalCount;
}

// 绘制背景
void RecycleStatusBar::drawBackground(QPainter *painter)
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
void RecycleStatusBar::drawLines(QPainter *painter)
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

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}

// 重写 绘画事件，绘制 背景
void RecycleStatusBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿
    //m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
    drawLines(&painter); // 绘制线条
}


