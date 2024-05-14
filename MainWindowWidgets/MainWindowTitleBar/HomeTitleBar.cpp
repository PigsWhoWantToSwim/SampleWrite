#include "hometitlebar.h"

HomeTitleBar::HomeTitleBar(QWidget *parent)
    : BasedMainWindowTitleBar{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}


// 初始化 成员变量
void HomeTitleBar::initMemberVariable()
{
    this->setMouseTracking(true);

    m_backgroundColor = QColor(27, 178, 139); // 设置 默认背景颜色

    m_iconPath = ":/image/icon with text.png"; // 设置 默认图标
}
// 初始化 布局
void HomeTitleBar::initLayout()
{
    // 总布局，垂直布局，上方为 窗口按钮区，中间添加 弹簧，下方为 标题栏功能区
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(HOMETITLEBAR_ICON_WIDTH + 30,0,0,10);
    this->setLayout(mainLayout);

    // 窗口控制按钮区，水平布置，依次为弹簧，最小化，最大化，向下还原，关闭
    QHBoxLayout *windowBtnLayout=new QHBoxLayout;
    windowBtnLayout->setSpacing(0);
    windowBtnLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(windowBtnLayout);

    // 添加弹簧
    windowBtnLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Fixed));

    // 创建 最小化按钮
    m_btnMinWindow = new QPushButton(this);
    m_btnMinWindow->setProperty("WindowButton","NoCloseButton"); // 设置属性
    m_btnMinWindow->setIcon(QIcon(":/image/minimized window - white.png"));
    m_btnMinWindow->setToolTip("最小化");
    windowBtnLayout->addWidget(m_btnMinWindow);

    // 创建 最大化按钮
    m_btnMaxWindow = new QPushButton(this);
    m_btnMaxWindow->setProperty("WindowButton","NoCloseButton"); // 设置属性
    m_btnMaxWindow->setIcon(QIcon(":/image/maximise window - white.png"));
    m_btnMaxWindow->setToolTip("最大化");
    windowBtnLayout->addWidget(m_btnMaxWindow);

    // 创建 向下还原按钮
    m_btnRestoreWindow = new QPushButton(this);
    m_btnRestoreWindow->setProperty("WindowButton","NoCloseButton"); // 设置属性
    m_btnRestoreWindow->setIcon(QIcon(":/image/restore window - white.png"));
    m_btnRestoreWindow->setToolTip("向下还原");
    m_btnRestoreWindow->setVisible(false); // 向下还原按钮 默认隐藏
    windowBtnLayout->addWidget(m_btnRestoreWindow);

    // 创建 关闭窗口按钮
    m_btnCloseWindow = new QPushButton(this);
    m_btnCloseWindow->setProperty("WindowButton","CloseButton"); // 设置属性
    m_btnCloseWindow->setIcon(QIcon(":/image/close window - white.png"));
    m_btnCloseWindow->setToolTip("关闭");
    windowBtnLayout->addWidget(m_btnCloseWindow);

    // 主布局中间添加弹簧，用于将 窗口按钮在最顶部
    mainLayout->addSpacerItem(new QSpacerItem(10, 10,QSizePolicy::Fixed, QSizePolicy::Expanding));
    // mainLayout->addSpacing(15);

    // 软件功能功能区，水平布置，依次为页面名标签，新建作品，弹簧，同步，设置
    QHBoxLayout *titleBarFunctionLayout = new QHBoxLayout;
    titleBarFunctionLayout->setSpacing(0);
    titleBarFunctionLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(titleBarFunctionLayout);

    // 页面名标签
    m_labPageName = new QLabel(this);
    m_labPageName->setObjectName("HomeTitleBar_labPageName");
    m_labPageName->setText("首页");
    m_labPageName->setAlignment(Qt::AlignCenter);
    titleBarFunctionLayout->addWidget(m_labPageName);

    // 添加 页面标签 与 新建按钮 之间的间距
    //titleBarFunctionLayout->addSpacing(30);

    // 新建作品按钮
    m_btnNewNovel = new QPushButton(this);
    m_btnNewNovel->setObjectName("HomeTitleBar_btnNewNovel");
    m_btnNewNovel->setText("新建作品");
    m_btnNewNovel->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    titleBarFunctionLayout->addWidget(m_btnNewNovel);

    // 添加 新建按钮 与 主题按钮 之间的间距
    //titleBarFunctionLayout->addSpacing(30);

    // 弹簧
    titleBarFunctionLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Fixed));

    // 设置
    m_btnSettings = new QPushButton(this);
    m_btnSettings->setProperty("WidgetStyleClass", "HomeTitleBar_PageButton");
    m_btnSettings->setIcon(QIcon(":/image/settings - white.png"));
    m_btnSettings->setText("设置");
    m_btnSettings->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    titleBarFunctionLayout->addWidget(m_btnSettings);

    // 关于按钮
    m_btnAbout = new QPushButton(this);
    m_btnAbout->setProperty("WidgetStyleClass", "HomeTitleBar_PageButton");
    m_btnAbout->setIcon(QIcon(":/image/about - white.png"));
    m_btnAbout->setText("关于");
    m_btnAbout->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    titleBarFunctionLayout->addWidget(m_btnAbout);
}

// 重写 初始化绑定 虚函数
void HomeTitleBar::initConnect()
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

    // 绑定 标题栏功能按钮点击 槽函数
    connect(m_btnNewNovel, &QPushButton::clicked, this, [=](){
        emit HomeTitleBar_NewNovelBtn_clicked();
    });
    connect(m_btnSettings, &QPushButton::clicked, this, [=](){
        m_labPageName->setText("设置");
        m_btnNewNovel->setVisible(false);
        // 发送信号
        emit HomeWorkspacePage_changed(HomeMainWindowPage::SettingsPage);
        emit HomeTitleBar_SettingsAboutBtn_clicked();
    });
    connect(m_btnAbout, &QPushButton::clicked, this, [=](){
        m_labPageName->setText("关于");
        m_btnNewNovel->setVisible(false);
        // 发送信号
        emit HomeWorkspacePage_changed(HomeMainWindowPage::AboutPage);
        emit HomeTitleBar_SettingsAboutBtn_clicked();
    });
}


// 设置 图标文件路径
void HomeTitleBar::setIconPath(QString iconPath)
{
    m_iconPath = iconPath;
    update();
}
// 获取 图标文件路径
QString HomeTitleBar::getIconPath() const
{
    return m_iconPath;
}

// 重写 最大化、向下还原按钮 切换
void HomeTitleBar::toggleMaxRestoreButtonVisible(bool isWindowMaximized)
{
    m_btnMaxWindow->setVisible(!isWindowMaximized); // 最大化显示时，隐藏最大化按钮
    m_btnRestoreWindow->setVisible(isWindowMaximized); // 最大化显示时，显示向下还原按钮
}

// 页面改变 处理槽函数
void HomeTitleBar::do_HomeWorkspacePage_changed(HomeMainWindowPage page)
{
    switch (page) {
    case HomeMainWindowPage::HomePage:
        m_labPageName->setText("首页");
        m_btnNewNovel->setVisible(true);
        break;
    case HomeMainWindowPage::FindPage:
        m_labPageName->setText("发现");
        m_btnNewNovel->setVisible(false);
        break;
    case HomeMainWindowPage::RecyclePage:
        m_labPageName->setText("回收");
        m_btnNewNovel->setVisible(false);
        break;
    case HomeMainWindowPage::SettingsPage:
        m_labPageName->setText("设置");
        m_btnNewNovel->setVisible(false);
        break;
    case HomeMainWindowPage::AboutPage:
        m_labPageName->setText("关于");
        m_btnNewNovel->setVisible(false);
        break;
    default:
        break;
    }
}

// 将图片缩放到rect矩形能完全显示图片
QPixmap HomeTitleBar::scaledImageToRect(QPixmap pix, QRect rect)
{
    /************************************************************
     * 图片缩放是 按高度对齐 还是 按宽度对齐 ?                     *
     * 1.缩放法                                                 *
     *  将图片按比例缩放，使图片宽度与按钮宽度相同(使高度相同同理)    *
     *  如果缩放后的图片高度小于按钮高度，则说明按宽度对齐缩放能完全  *
     *  显示图片，反之按高度对齐缩放。                             *
     * 2.比例法                                                 *
     *  分别计算按钮和图片的宽高比(高宽比同理)，如果图片宽高比大于    *
     *  按钮宽高比，则按宽度对齐缩放，反之按高度对齐。               *
     * 3.面积法(比例法变形)                                      *
     * 如果(图片宽度 * 按钮高度) > (按钮宽度 * 图片高度)，则说明按高 *
     * 度对齐缩放时，图片宽度大于按钮宽度，应该按宽度对齐缩放，反之说  *
     * 明按高度对齐缩放时，图片宽度小于按钮宽度，能完全显示图片，应该  *
     * 按高度对齐缩放。                                           *
    ************************************************************/

    // 图片原始尺寸
    int pixWidth = pix.width();
    int pixHeight = pix.height();
    // 缩放比
    double scaledRate = 1.0;
    // 图片缩放后尺寸
    int pixScaledWidth = pixWidth;
    int pixScaledHeight = pixHeight;


    // 面积法
    if (pixWidth * rect.height() > pixHeight * rect.width())
    {
        // 此时，按宽度对齐缩放能全部显示图片，但不能完全填充矩形
        scaledRate = double(rect.width()) / double(pixWidth); // 缩放比
        // 计算缩放后图片尺寸
        pixScaledWidth = rect.width();
        pixScaledHeight = pixHeight * scaledRate;
    }
    else
    {
        // 此时，按高度对齐缩放能全部显示图片，但不能完全填充矩形
        scaledRate = double(rect.height()) / double(pixHeight); // 缩放比
        // 计算缩放后图片尺寸
        pixScaledWidth = pixWidth * scaledRate;
        pixScaledHeight = rect.height();
    }

    // 缩放后的图片
    QPixmap scaledPix;
    // 进行 缩放，缩放到 填充矩形
    scaledPix = pix.scaled(QSize(pixScaledWidth*m_pixelRatio, pixScaledHeight*m_pixelRatio),
                                 Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // 返回 缩放后的图片
    return scaledPix;
}

// 绘制 图标
void HomeTitleBar::drawIcon(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    // 原始图片
    QPixmap pixIcon = QPixmap(m_iconPath);

    // 获取标题栏尺寸
    QRect titleBarRect = this->rect();
    // 获取边距
    int iconMargins = (titleBarRect.height() - HOMETITLEBAR_ICON_HEIGHT)/2;

    // 图片在标题栏的位置
    QRect iconRect(titleBarRect.left() + iconMargins,
                   titleBarRect.top() + iconMargins,
                   HOMETITLEBAR_ICON_WIDTH,
                   HOMETITLEBAR_ICON_HEIGHT);

    // 获取缩放后的图标
    pixIcon = scaledImageToRect(pixIcon, iconRect);

    int icon_X = (iconRect.width() - pixIcon.width())/2;
    int icon_Y = (iconRect.height() - pixIcon.height())/2;

    QRect iconRealRect(iconRect.left() + icon_X, iconRect.top() + icon_Y,
                       pixIcon.width(), pixIcon.height());

    painter->drawPixmap(iconRealRect, pixIcon);

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}

// 重写绘画事件，绘制背景和图标
void HomeTitleBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿

    m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
    drawIcon(&painter); // 绘制 图标
}

void HomeTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<"标题栏双击";
    if(this->windowState() == Qt::WindowMaximized) // 当前窗口 最大化显示
    {
        //
        m_btnRestoreWindow->setVisible(false);
        m_btnMaxWindow->setVisible(true);
    }
    else if(this->windowState() == Qt::WindowNoState) // 当前窗口 正常显示
    {
        // 点击 最大化按钮
        m_btnMaxWindow->setVisible(false);
        m_btnRestoreWindow->setVisible(true);
    }

    // QWidget::mouseDoubleClickEvent(event);
}
