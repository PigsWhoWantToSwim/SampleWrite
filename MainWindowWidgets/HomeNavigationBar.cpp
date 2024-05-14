#include "HomeNavigationBar.h"

HomeNavigationBar::HomeNavigationBar(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void HomeNavigationBar::initMemberVariable()
{
    m_backgroundColor = QColor(255, 255, 255);
}
// 初始化 布局
void HomeNavigationBar::initLayout()
{
    // 主布局，垂直布局，依次是 首页，发现，回收站，设置，弹簧
    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,30,0,0);
    this->setLayout(mainLayout);

    // 互斥按钮组
    m_exclusiveButtonGroup = new QButtonGroup(this); // 新建 互斥按钮组
    m_exclusiveButtonGroup->setExclusive(true); // 设置 组内按钮 互斥

    QPushButton *btnHome = new QPushButton(this);
    btnHome->setObjectName("HomeNavigationBar_btnHome");
    btnHome->setProperty("NavigationBarButton", "NavigationBarButton");
    btnHome->setText("首页");
    btnHome->setIconSize(QSize(NAVIGATIONBARBUTTON_ICON_SIZE, NAVIGATIONBARBUTTON_ICON_SIZE));
    btnHome->setFixedHeight(NAVIGATIONBARBUTTON_HEIGHT);
    btnHome->setCheckable(true); // 设置可勾选
    btnHome->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    btnHome->setChecked(true); // 默认勾选
    m_exclusiveButtonGroup->addButton(btnHome); // 加入互斥按钮组
    connect(btnHome,&QPushButton::clicked,this,[=](){
        emit HomeWorkspacePage_changed(HomeMainWindowPage::HomePage);
    });
    m_listNavigationBarButtons << btnHome;
    mainLayout->addWidget(btnHome); // 添加到 主布局

    QPushButton *btnFind = new QPushButton(this);
    btnFind->setObjectName("HomeNavigationBar_btnFind");
    btnFind->setProperty("NavigationBarButton", "NavigationBarButton");
    btnFind->setText("发现");
    btnFind->setIconSize(QSize(NAVIGATIONBARBUTTON_ICON_SIZE, NAVIGATIONBARBUTTON_ICON_SIZE));
    btnFind->setFixedHeight(NAVIGATIONBARBUTTON_HEIGHT);
    btnFind->setCheckable(true);
    btnFind->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_exclusiveButtonGroup->addButton(btnFind); // 加入互斥按钮组
    connect(btnFind,&QPushButton::clicked,this,[=](){
        emit HomeWorkspacePage_changed(HomeMainWindowPage::FindPage);
    });
    m_listNavigationBarButtons << btnFind;
    mainLayout->addWidget(btnFind); // 添加到 主布局

    QPushButton *btnRecycle = new QPushButton(this);
    btnRecycle->setObjectName("HomeNavigationBar_btnRecycle");
    btnRecycle->setProperty("NavigationBarButton", "NavigationBarButton");
    btnRecycle->setText("回收");
    btnRecycle->setIconSize(QSize(NAVIGATIONBARBUTTON_ICON_SIZE, NAVIGATIONBARBUTTON_ICON_SIZE));
    btnRecycle->setFixedHeight(NAVIGATIONBARBUTTON_HEIGHT);
    btnRecycle->setCheckable(true);
    btnRecycle->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_exclusiveButtonGroup->addButton(btnRecycle); // 加入互斥按钮组
    connect(btnRecycle,&QPushButton::clicked,this,[=](){
        emit HomeWorkspacePage_changed(HomeMainWindowPage::RecyclePage);
    });
    m_listNavigationBarButtons << btnRecycle;
    mainLayout->addWidget(btnRecycle); // 添加到 主布局


    // 主布局中间添加弹簧，用于将 窗口按钮在最顶部
    mainLayout->addSpacerItem(new QSpacerItem(10, 10,QSizePolicy::Fixed, QSizePolicy::Expanding));
}
// 初始化 绑定
void HomeNavigationBar::initConnect()
{


}

// 设置 背景颜色
void HomeNavigationBar::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
// 获取 背景颜色
QColor HomeNavigationBar::getBackgroundColor() const
{
    return m_backgroundColor;
}

// 点击 主题、关于按钮 处理槽函数，将导航栏按钮取消勾选
void HomeNavigationBar::do_HomeTitleBar_ThemeAboutBtn_clicked()
{
    m_exclusiveButtonGroup->setExclusive(false); // 互斥按钮组 取消互斥，不取消互斥则必须要另一按钮代替被“勾选”
    // 遍历按钮组 将已勾选的按钮 取消“勾选”
    foreach (QPushButton *btn, m_listNavigationBarButtons) {
        if(btn->isChecked()) // 按钮正被勾选
            btn->setChecked(false); // 取消勾选
    }
    m_exclusiveButtonGroup->setExclusive(true); // 重新 设置互斥按钮组 可互斥
}

// 绘制 背景
void HomeNavigationBar::drawBackground(QPainter *painter)
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
void HomeNavigationBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿
    //  m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
}




