#include "HomeWorkspaceRecyclePage.h"

HomeWorkspaceRecyclePage::HomeWorkspaceRecyclePage(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}



// 初始化 成员函数
void HomeWorkspaceRecyclePage::initMemberVariable()
{
    m_backgroundColor = QColor(245, 256, 249);
}
// 初始化 布局
void HomeWorkspaceRecyclePage::initLayout()
{
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setHorizontalSpacing(0);
    mainLayout->setVerticalSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    // 第一行 为 弹簧和清空按钮
    mainLayout->addItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed), 0, 0, 1, 1);

    m_btnClearAll = new QPushButton(this);
    m_btnClearAll->setObjectName("HomeWorkspaceRecyclePage_btnClear");
    m_btnClearAll->setText("清空回收站");
    m_btnClearAll->setCursor(QCursor(Qt::PointingHandCursor)); // 设置光标
    mainLayout->addWidget(m_btnClearAll, 0, 1, 1, 1);

    // 第二行 为 回收站
    m_recycleBin = new RecycleBin(this);
    // m_recycleBin->setStyleSheet("background-color: #4589ae;");
    mainLayout->addWidget(m_recycleBin, 1, 0, 1, 2);

    // 第三行 为 状态栏
    m_recycleStatusBar = new RecycleStatusBar(this);
    m_recycleStatusBar->setTotalCount(m_recycleBin->getTotalCount()); // 发送信号时，未绑定槽函数，只能手动设置
    // m_recycleStatusBar->setStyleSheet("background-color: #eee94a;");
    mainLayout->addWidget(m_recycleStatusBar, 2, 0, 1, 2);
}
// 初始化 绑定事件
void HomeWorkspaceRecyclePage::initConnect()
{
    // 绑定 清空按钮
    connect(m_btnClearAll, &QPushButton::clicked, this, [=](){
        // 打开 对话框
        QMessageBox::StandardButton resultBtn = QMessageBox::warning(this, "清空回收站",
                                                                     "确认清空回收站所以项目吗？",
                                                                     QMessageBox::Cancel|QMessageBox::Yes);

        // 点击 取消按钮
        if(resultBtn == QMessageBox::Cancel)
            return; // 直接结束，不做删除操作

        // 清空 回收站
        m_recycleBin->do_recycleBin_clearAll();
    });
    // 绑定 回收站 选中项和项总数
    connect(m_recycleBin, &RecycleBin::totalCount_changed, m_recycleStatusBar, &RecycleStatusBar::setTotalCount);
    connect(m_recycleBin, &RecycleBin::selectedCount_changed, m_recycleStatusBar, &RecycleStatusBar::setSelectCount);
    connect(m_recycleBin, &RecycleBin::recycledWork_restored, this, [=](){
        emit recycledWork_restored();
    });
    // 绑定 状态栏按钮点击
    connect(m_recycleStatusBar, &RecycleStatusBar::btnRestore_clicked, m_recycleBin, &RecycleBin::do_recycleBin_restore);  
    connect(m_recycleStatusBar, &RecycleStatusBar::btnDelete_clicked, m_recycleBin, &RecycleBin::do_recycleBin_clear);


}

// 设置 背景颜色
void HomeWorkspaceRecyclePage::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
// 获取 背景颜色
QColor HomeWorkspaceRecyclePage::getBackgroundColor() const
{
    return m_backgroundColor;
}

void HomeWorkspaceRecyclePage::do_deleteNovel(QString novelID)
{
    Q_UNUSED(novelID);
    m_recycleBin->updateRecycleBin();
}
// 目录栏 删除项 处理槽函数
void HomeWorkspaceRecyclePage::do_directoryItem_deleted()
{
    m_recycleBin->updateRecycleBin();
}

// 绘制 背景
void HomeWorkspaceRecyclePage::drawBackground(QPainter *painter)
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
void HomeWorkspaceRecyclePage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿
    //m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
}



