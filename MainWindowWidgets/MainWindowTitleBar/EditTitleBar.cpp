#include "EditTitleBar.h"

EditTitleBar::EditTitleBar(QWidget *parent)
    : BasedMainWindowTitleBar{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

void EditTitleBar::initMemberVariable()
{
    this->setMouseTracking(true);
    m_titleText = "未知作品";

    m_backgroundColor = QColor(27, 178, 139); // 设置 默认背景颜色
}
void EditTitleBar::initLayout()
{
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(150,0,0,0);
    this->setLayout(mainLayout);

    mainLayout->addSpacerItem(new QSpacerItem(1, 1,QSizePolicy::Expanding, QSizePolicy::Fixed));

    // 标题栏标签
    m_labTitle = new QLabel(this);
    m_labTitle->setObjectName("EditTitleBar_labTitle");
    m_labTitle->setAlignment(Qt::AlignCenter);
    m_labTitle->setText(m_titleText);
    mainLayout->addWidget(m_labTitle);

    mainLayout->addSpacerItem(new QSpacerItem(1, 1,QSizePolicy::Expanding, QSizePolicy::Fixed));

    // 窗口按钮
    // 创建 最小化按钮
    m_btnMinWindow = new QPushButton(this);
    m_btnMinWindow->setProperty("WindowButton","NoCloseButton"); // 设置属性
    m_btnMinWindow->setIcon(QIcon(":/image/minimized window - white.png"));
    m_btnMinWindow->setToolTip("最小化");
    mainLayout->addWidget(m_btnMinWindow);

    // 创建 最大化按钮
    m_btnMaxWindow = new QPushButton(this);
    m_btnMaxWindow->setProperty("WindowButton","NoCloseButton"); // 设置属性
    m_btnMaxWindow->setIcon(QIcon(":/image/maximise window - white.png"));
    m_btnMaxWindow->setToolTip("最大化");
    mainLayout->addWidget(m_btnMaxWindow);

    // 创建 向下还原按钮
    m_btnRestoreWindow = new QPushButton(this);
    m_btnRestoreWindow->setProperty("WindowButton","NoCloseButton"); // 设置属性
    m_btnRestoreWindow->setIcon(QIcon(":/image/restore window - white.png"));
    m_btnRestoreWindow->setToolTip("向下还原");
    m_btnRestoreWindow->setVisible(false); // 向下还原按钮 默认隐藏
    mainLayout->addWidget(m_btnRestoreWindow);

    // 创建 关闭窗口按钮
    m_btnCloseWindow = new QPushButton(this);
    m_btnCloseWindow->setProperty("WindowButton","CloseButton"); // 设置属性
    m_btnCloseWindow->setIcon(QIcon(":/image/close window - white.png"));
    m_btnCloseWindow->setToolTip("关闭");
    mainLayout->addWidget(m_btnCloseWindow);

}

void EditTitleBar::initConnect()
{
    // 绑定 窗口按钮点击 槽函数
    connect(m_btnMinWindow, &QPushButton::clicked, this, [=](){
        emit titleBarButton_clicked(TitleBarClickedButton::MinimizedButton);
    });
    connect(m_btnMaxWindow, &QPushButton::clicked, this, [=](){
        // 要进行 最大化
        m_btnMaxWindow->setVisible(false);
        m_btnRestoreWindow->setVisible(true);
        emit titleBarButton_clicked(TitleBarClickedButton::MaximizedButton);
    });
    connect(m_btnRestoreWindow, &QPushButton::clicked, this, [=](){
        // 要进行 向下还原
        m_btnRestoreWindow->setVisible(false);
        m_btnMaxWindow->setVisible(true);
        emit titleBarButton_clicked(TitleBarClickedButton::RestoreButton);
    });
    connect(m_btnCloseWindow, &QPushButton::clicked, this, [=](){
        emit titleBarButton_clicked(TitleBarClickedButton::CloseButton);
    });

}

void EditTitleBar::setTitleText(QString titleText)
{
    m_titleText = titleText;
    m_labTitle->setText(titleText);

    update();
}

QString EditTitleBar::getTitleText() const
{
    return m_titleText;
}


void EditTitleBar::toggleMaxRestoreButtonVisible(bool isWindowMaximized)
{
    m_btnMaxWindow->setVisible(!isWindowMaximized); // 最大化显示时，隐藏最大化按钮
    m_btnRestoreWindow->setVisible(isWindowMaximized); // 最大化显示时，显示向下还原按钮
}

void EditTitleBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿

    // m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
}

void EditTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{

}


