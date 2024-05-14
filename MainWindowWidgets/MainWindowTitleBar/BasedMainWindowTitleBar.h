#ifndef BASEDMAINWINDOWTITLEBAR_H
#define BASEDMAINWINDOWTITLEBAR_H

#include <QWidget>
#include <QPainter>
#include <QPushButton>

#include "GlobalClass/AppEnumClass.h"

class BasedMainWindowTitleBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor TitleBarBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
public:
    explicit BasedMainWindowTitleBar(QWidget *parent = nullptr);
    ~BasedMainWindowTitleBar();

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor &backgroundColor);
    QColor getBackgroundColor() const;



public:
    // 切换显示 最大化和向下还原按钮
    virtual void toggleMaxRestoreButtonVisible(bool isWindowMaximized) = 0;


    // QWidget interface
protected:
    // 重写绘画事件，绘制背景
    virtual void paintEvent(QPaintEvent *event) override;

protected:
    // 初始化 成员函数
    virtual void initMemberVariable();
    // 初始化 布局
    virtual void initLayout() = 0;
    // 初始化 绑定事件
    virtual void initConnect() = 0;

    // 绘制背景
    void drawBackground(QPainter *painter);

private:


signals:
    // 标题栏按钮点击信号
    void titleBarButton_clicked(TitleBarClickedButton clickedButton);

    void minimizedButton_clicked(); // 最小化窗口
    void maximizedButton_clicked(); // 最大化窗口
    void restoreButton_clicked(); // 向下还原窗口
    void closeButton_clicked(); // 关闭窗口



protected:
    // 控件
    QPushButton *m_btnMinWindow; // 最小化 窗口按钮
    QPushButton *m_btnMaxWindow; // 最大化 窗口按钮
    QPushButton *m_btnRestoreWindow; // 向下还原 窗口按钮
    QPushButton *m_btnCloseWindow; // 关闭 窗口按钮

    // 成员变量
    QColor m_backgroundColor; // 背景颜色

private:


};

#endif // BASEDMAINWINDOWTITLEBAR_H
