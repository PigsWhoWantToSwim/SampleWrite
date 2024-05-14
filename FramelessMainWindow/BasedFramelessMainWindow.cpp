#include "BasedFramelessMainWindow.h"

// 构造函数
BasedFramelessMainWindow::BasedFramelessMainWindow(QWidget *parent)
    : QWidget{parent}
{
    this->resize(600, 400); // 设置 窗口初始尺寸
    this->setAttribute(Qt::WA_TranslucentBackground); // 设置 为透明
    // this->setAttribute(Qt::WA_NativeWindow, true);
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint); // 设置 无边框
    //安装事件过滤器识别拖动
    this->installEventFilter(this); // 为自身安装事件过滤器

    //设置属性产生win窗体效果,移动到边缘半屏或者最大化等
    //设置以后会产生标题栏,需要在下面拦截消息WM_NCCALCSIZE重新去掉
    HWND hwnd = (HWND)this->winId(); // ?????????????????????????????????
    DWORD style = ::GetWindowLong(hwnd, GWL_STYLE); // ?????????????????????????
    ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);


    QHBoxLayout *m_mainWindowLayout = new QHBoxLayout; // 主布局，用于将容器控件，置于 窗口中心
    m_mainWindowLayout->setSpacing(0);
    m_mainWindowLayout->setContentsMargins(SHADOWEFFECT_WIDTH, SHADOWEFFECT_WIDTH, SHADOWEFFECT_WIDTH, SHADOWEFFECT_WIDTH);
    this->setLayout(m_mainWindowLayout); // 将 主布局 设置为窗口的布局

    m_widContainer = new QWidget(this); // 创建 容器控件
    m_widContainer->setProperty("MainWindowWidgets", "Container");
    m_widContainer->installEventFilter(this); // 安装事件过滤器
    m_widContainer->setMouseTracking(true);
    m_mainWindowLayout->addWidget(m_widContainer); // 将 容器控件 添加到主布局

    QGraphicsDropShadowEffect *containerEffect = new QGraphicsDropShadowEffect(m_widContainer); // 创建 阴影效果
    containerEffect->setOffset(0, 0); // 阴影效果 不偏移
    containerEffect->setColor(QColor(QStringLiteral("black"))); // 设置阴影效果 的颜色
    containerEffect->setBlurRadius(15); // 设置 模糊半径
    m_widContainer->setGraphicsEffect(containerEffect); // 为 容器控件设置阴影效果
}
// 析构函数
BasedFramelessMainWindow::~BasedFramelessMainWindow()
{
}

// 重写 事件过滤器，避免子控件覆盖，无法移动拉伸窗口
bool BasedFramelessMainWindow::eventFilter(QObject *watched, QEvent *event)
{
    // 获取事件的类型
    int type = event->type();
    if (watched == this)
    {
        // 在当前窗口发生
        if (type == QEvent::WindowStateChange) // 事件类型为 窗口状态变化事件
        {
            if(this->windowState() == Qt::WindowMaximized) // 改变后的状态为最大化
                emit windowState_changed(true); // 发送状态改变信号
            else // 其他状态
                emit windowState_changed(false); // 发送状态改变信号
        }
    }
    // 返回 父类事件过滤器，继续处理
    return QWidget::eventFilter(watched, event);
}

// nativeEvent主要用于进程间通信-消息传递，使用这种方式后来实现窗体的缩放 [加上了这函数，窗口也能移动了]
bool BasedFramelessMainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
{
    MSG *msg = static_cast<MSG *>(message);

    if (msg->message == WM_NCCALCSIZE) {
        // WM_NCCALCSIZE,"当某窗口的客户区的大小和位置须被计算时发送本消息"
        *result = 0;
        return true;
    }
    else if (msg->message == WM_SYSKEYDOWN)
    {
        // WM_SYSKEYDOWN,"当用户按住<ALT>键的同时又按下其它键时,发送本消息给拥有焦点的窗口",
        //屏蔽alt键按下
    }
    else if (msg->message == WM_SYSKEYUP)
    {
        // WM_SYSKEYUP,"当用户释放一个按键的同时<ALT>键还按着时,发送本消息给拥有焦点的窗口"
        //屏蔽alt键松开
    }
    else if(msg->message == WM_NCHITTEST)
    {
        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);

        // 将全局坐标映射为识图坐标，窗口左上角坐标为(0,0)，但需要的是容器的左上角
        QPoint pos = mapFromGlobal(QPoint(x, y));

        //判断当前鼠标位置在哪个区域
        /*
         *  * * * * * * * * * * * * *
         *  *                       *
         *  *   -----------------   *
         *  *   |               |   *
         *  *   | +++++++++++++ |   *
         *  *   | +           + |   *
         *  *   | +           + |   *
         *  *   | +           + |   *
         *  *   | +           + |   *
         *  *   | +++++++++++++ |   *
         *  *   |               |   *
         *  *   -----------------   *
         *  *                       *
         *  * * * * * * * * * * * * *
        */
        /* 如上图，*框是窗口，*框与-框之间是阴影，-框与+框之间才是用于判断拉伸
         * 此时，pos是相对与窗口左上角
        */
        bool left = (pos.x() > SHADOWEFFECT_WIDTH - STRETCH_WIDTH/2) && (pos.x() < SHADOWEFFECT_WIDTH + STRETCH_WIDTH/2);
        bool right = (pos.x() > this->width() - SHADOWEFFECT_WIDTH - STRETCH_WIDTH/2) && (pos.x() < this->width() - SHADOWEFFECT_WIDTH + STRETCH_WIDTH/2);
        bool top = (pos.y() > SHADOWEFFECT_WIDTH - STRETCH_WIDTH/2) && (pos.y() < SHADOWEFFECT_WIDTH + STRETCH_WIDTH/2);
        bool bottom = (pos.y() > this->height() - SHADOWEFFECT_WIDTH - STRETCH_WIDTH/2) && (pos.y() < this->height() - SHADOWEFFECT_WIDTH + STRETCH_WIDTH/2);


        // 鼠标移动到四个角,这个消息是当鼠标移动或者有鼠标键按下时候发出的
        *result = 0; // 将结果置空

        // 可拉伸
        if (left && top)
        { // 左上角
            *result = HTTOPLEFT; // HTTOPLEFT，值：13，在窗口边框的左上角
        }
        else if (left && bottom)
        { // 左下角
            *result = HTBOTTOMLEFT; //  HTBOTTOMLEFT，值：16，在可调整大小的窗口的边框左下角（用户可以单击鼠标以对角线调整窗口大小）
        }
        else if (right && top)
        { // 右上角
            *result = HTTOPRIGHT; // HTTOPRIGHT，值：14，在窗口边框的右上角
        }
        else if (right && bottom)
        { // 右下角
            *result = HTBOTTOMRIGHT; // HTBOTTOMRIGHT，值：17，在可调整大小的窗口的边框右下角（用户可以单击鼠标以对角线调整窗口大小）
        }
        else if (left)
        { // 左侧
            *result = HTLEFT; // HTLEFT，值：10，在可调整大小的窗口的左边框中（用户可以单击鼠标以水平调整窗口大小）
        }
        else if (right)
        { // 右侧
            *result = HTRIGHT; // HTRIGHT，值：11，在可调整大小的窗口的右左边框中（用户可以单击鼠标以水平调整窗口大小）
        }
        else if (top)
        { // 上侧
            *result = HTTOP; // HTTOP，值：12，在窗口的上水平边框中
        }
        else if (bottom)
        { // 下侧
            *result = HTBOTTOM; // HTBOTTM，值：15，在可调整大小的窗口的下水平边框中（用户可以单击鼠标以垂直调整窗口大小）
        }

        //先处理掉拉伸
        if (0 != *result)
        {
            // HTNOWHERE，值：0，在屏幕背景上，或在窗口之间的分隔线上。
            // 不在边框位置，则不是拉伸
            return true; // 不再向下传递
        }

        // 识别容器拖动产生半屏全屏效果
        int width = m_widContainer->rect().width() - STRETCH_WIDTH;
        QRect moveWindowArea(STRETCH_WIDTH/2,SHADOWEFFECT_WIDTH + STRETCH_WIDTH/2,width,AREA_FORMOEWINDOW_HEIGHT);
        if (m_widContainer && moveWindowArea.contains(pos)) // 容器 不为空 且 光标在容器允许移动窗口区域内
        {
            // 获取 光标所在位置的 控件
            QWidget *child = m_widContainer->childAt(pos);
            if((child && child->property("WidgetType").toString() == "MainWindowTitleBar") || (!child))
            {
                // 子控件（容器里的所有子控件）存在，且为标题栏
                // (!child) 是用于未添加标题栏时，也能让窗口移动，可以删掉

                *result = HTCAPTION; // HTCAPTION，值2，即在标题栏中。
                return true;
            }
        }
    }
    else if (msg->wParam == PBT_APMSUSPEND && msg->message == WM_POWERBROADCAST)
    {
        /* wParam，电源管理事件
             * PBT_APMSUSPEND，值：4，系统正在暂停操作
             * WM_POWERBROADCAST，通知应用程序发生了某个电源管理事件
            */
        // 系统休眠，且通知应用程序发生了某个电源管理事件
        // 系统休眠的时候自动最小化可以规避程序可能出现的问题
        this->showMinimized(); // 最小化
    }
    else if (msg->wParam == PBT_APMRESUMEAUTOMATIC)
    {
        /* PBT_APMRESUMEAUTOMATIC，值：18，操作正在从低功耗状态自动恢复。
         * 每次系统恢复时都会发送此消息。
        */

        //休眠唤醒后自动打开
        this->showNormal(); // 正常显示
    }
    // 事件继续传播给事件接收者做进一步处理
    return false;
}
// 重写 显示事件 解决有时候窗体重新显示的时候假死不刷新的bug
void BasedFramelessMainWindow::showEvent(QShowEvent *event)
{
    //解决有时候窗体重新显示的时候假死不刷新的bug
    setAttribute(Qt::WA_Mapped); // ??????????????????????
    // QWindowsWindowFunctions::setHasBorderInFullScreen(this->windowHandle(), true);
    QWidget::showEvent(event); // 调用父类的显示事件
}

// 标题栏按钮点击信号
void BasedFramelessMainWindow::do_titleBarButton_clicked(TitleBarClickedButton clickedButton)
{
    // 判断 被点击的按钮
    switch (clickedButton)
    {
    case TitleBarClickedButton::MinimizedButton:
        this->do_window_minimized(); // 最小化
        break;
    case TitleBarClickedButton::MaximizedButton:
        this->do_window_maximized(); // 最大化
        break;
    case TitleBarClickedButton::RestoreButton:
        this->do_window_restore(); // 向下还原
        break;
    case TitleBarClickedButton::CloseButton:
        this->do_window_close(); // 关闭
        break;
    default:
        break;
    }
}

// 最小化窗口
void BasedFramelessMainWindow::do_window_minimized()
{
    if(!this->isMinimized()) // 不处于最小化状态
        this->showMinimized(); // 最小化
}
// 最大化窗口
void BasedFramelessMainWindow::do_window_maximized()
{
    if(!this->isMaximized()) // 不处于最大化状态
        this->showMaximized(); // 最大化
}
// 向下还原窗口
void BasedFramelessMainWindow::do_window_restore()
{
    if(this->windowState() != Qt::WindowNoState) // 不处于无状态
        this->showNormal(); // 正常显示
}
// 关闭窗口
void BasedFramelessMainWindow::do_window_close()
{
    // 打开 对话框
    QMessageBox::StandardButton resultBtn = QMessageBox::information(this,"提示","确认要退出吗？",QMessageBox::Cancel|QMessageBox::Yes);

    // 点击 取消按钮
    if(resultBtn == QMessageBox::Cancel)
        return; // 直接结束，不做删除操作

    // 释放 settings
    if(settings != nullptr)
    {
        delete settings;
        settings = nullptr;
    }

    // 关闭
    this->close();
}




