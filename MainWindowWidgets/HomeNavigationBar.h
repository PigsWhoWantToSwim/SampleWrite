#ifndef HOMENAVIGATIONBAR_H
#define HOMENAVIGATIONBAR_H

#include <QWidget>
#include <QPainter>
#include <QButtonGroup>
#include <QPushButton>
#include <QVBoxLayout>
#include "GlobalClass/AppEnumClass.h"

#define NAVIGATIONBARBUTTON_HEIGHT 60
#define NAVIGATIONBARBUTTON_ICON_SIZE 30

class HomeNavigationBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor HomeNavigationBarBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
public:
    explicit HomeNavigationBar(QWidget *parent = nullptr);

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor &backgroundColor);
    QColor getBackgroundColor() const;

public slots:
    // 点击 主题、关于按钮 处理槽函数，将导航栏按钮取消勾选
    void do_HomeTitleBar_ThemeAboutBtn_clicked();

    // QWidget interface
protected:
    // 重写 绘画事件
    virtual void paintEvent(QPaintEvent *event) override;

private:
    // 初始化 成员函数
    void initMemberVariable();
    // 初始化 布局
    void initLayout();
    // 初始化 绑定事件
    void initConnect();

    // 绘制背景
    void drawBackground(QPainter *painter);

signals:
    // 工作区页面改变 信号
    void HomeWorkspacePage_changed(HomeMainWindowPage page);

private:
    // 控件
    // 互斥按钮组
    QList< QPushButton *> m_listNavigationBarButtons;
    QButtonGroup *m_exclusiveButtonGroup; // 互斥按钮组

    // 成员变量
    QColor m_backgroundColor;

};

#endif // HOMENAVIGATIONBAR_H
