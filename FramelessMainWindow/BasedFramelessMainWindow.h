#ifndef BASEDFRAMELESSMAINWINDOW_H
#define BASEDFRAMELESSMAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QEvent>
#include <QPushButton>
#include <QMouseEvent>
#include <QMessageBox>
#include "MainWindowWidgets/MainWindowTitleBar/BasedMainWindowTitleBar.h"
#include "GlobalClass/AppEnumClass.h"

#ifdef Q_OS_WIN
#include "windows.h"
#include "windowsx.h"
#pragma comment (lib,"user32.lib")
#endif

#define SHADOWEFFECT_WIDTH 9 // 阴影效果的宽度
#define STRETCH_WIDTH 10 // 拉伸区域的宽度
#define AREA_FORMOEWINDOW_HEIGHT 20 // 用于按下鼠标移动窗口的区域的高度

class BasedFramelessMainWindow : public QWidget
{
    Q_OBJECT
public:
    // 构造函数
    explicit BasedFramelessMainWindow(QWidget *parent = nullptr);
    ~BasedFramelessMainWindow();

    // QWidget interface
protected:
    // 重写 事件过滤器
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    // nativeEvent主要用于进程间通信-消息传递，使用这种方式后来实现窗体的缩放 [加上了这函数，窗口也能移动了]
    virtual bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
    // 重写 显示事件 解决有时候窗体重新显示的时候假死不刷新的bug
    virtual void showEvent(QShowEvent *event) override;


protected:
    // 初始化 成员函数
    virtual void initMemberVariable() = 0;
    // 初始化 布局
    virtual void initLayout() = 0;
    // 初始化 绑定
    virtual void initConnect() = 0;

    // 标题栏按钮点击信号
    virtual void do_titleBarButton_clicked(TitleBarClickedButton clickedButton);

private:


protected slots:
    // 槽函数
    void do_window_minimized(); // 最小化窗口
    void do_window_maximized(); // 最大化窗口
    void do_window_restore(); // 向下还原窗口
    void do_window_close(); // 关闭窗口

private:


signals:
    // 窗口状态改变
    void windowState_changed(bool isMaximized);

protected:
    // 容器控件，取消边框后无阴影效果，所以this用来绘制阴影，容器用来放置内容
    QWidget *m_widContainer;

private:

};

#endif // BASEDFRAMELESSMAINWINDOW_H
