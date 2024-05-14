#ifndef BASEDSIDERBARPAGE_H
#define BASEDSIDERBARPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include "GlobalClass/AppEnumClass.h"

class BasedSiderBarPage : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor SiderBarPageBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
public:
    explicit BasedSiderBarPage(QWidget *parent = nullptr);

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor &backgroundColor);
    QColor getBackgroundColor() const;

    // 设置 和 获取 侧边栏页面 标题
    void setSideBarPageTitle(QString title);
    QString getSideBarPageTitle() const;

    // QWidget interface
protected:
    // 重写绘画事件，绘制背景
    virtual void paintEvent(QPaintEvent *event) override;

protected:
    // 初始化 成员函数
    virtual void initMemberVariable() = 0;
    // 初始化 布局
    virtual void initLayout() = 0;
    // 初始化 绑定事件
    virtual void initConnect() = 0;

    // 绘制背景
    void drawBackground(QPainter *painter);

signals:
    // 关闭 页面
    void closePage();

protected:
    // 成员控件
    QLabel *m_labTitle;
    QPushButton *m_btnClose;
    QFrame *m_frmContainer;

    // 成员变量
    QColor m_backgroundColor;
};

#endif // BASEDSIDERBARPAGE_H
