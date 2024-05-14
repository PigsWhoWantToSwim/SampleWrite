#ifndef RECYCLEBINTITLEBAR_H
#define RECYCLEBINTITLEBAR_H

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QPainter>
#include "GlobalClass/AppEnumClass.h"

class RecycleBinTitleBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor RecycleBinTitleBarBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor RecycleBinTitleBarLineColor READ getLineColor WRITE setLineColor)
public:
    explicit RecycleBinTitleBar(QWidget *parent = nullptr);

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor& backgroundColor);
    QColor getBackgroundColor() const;

    // 设置 和 获取 线条颜色
    void setLineColor(const QColor& lineColor);
    QColor getLineColor() const;

    // 设置 和 获取 复选框状态
    void setCheckState(const Qt::CheckState checkState);
    Qt::CheckState getCheckState() const;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

private slots:
    // // 复选框 状态改变 处理槽函数
    // void do_ckbSelect_stateChanged();

private:
    // 初始化 成员函数
    void initMemberVariable();
    // 初始化 布局
    void initLayout();
    // 初始化 绑定事件
    void initConnect();

    // 绘制背景
    void drawBackground(QPainter *painter);
    // 绘制线条
    void drawLines(QPainter *painter);

signals:
    // 标题复选框 状态改变
    void recycleTitleBar_checkStateChanged(bool checked);

private:
    // 成员控件
    QCheckBox *m_ckbSelect;

    // 成员变量
    QColor m_backgroundColor;
    QColor m_lineColor;

    Qt::CheckState m_checkState; // 勾选状态，选中、部分选中、未选中
};

#endif // RECYCLEBINTITLEBAR_H
