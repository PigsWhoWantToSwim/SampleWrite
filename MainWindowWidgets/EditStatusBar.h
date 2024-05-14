#ifndef EDITSTATUSBAR_H
#define EDITSTATUSBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include "GlobalClass/AppEnumClass.h"

class EditStatusBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor EditStatusBarBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor EditStatusBarLineColor READ getLineColor WRITE setLineColor)
public:
    explicit EditStatusBar(QWidget *parent = nullptr);

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor& backgroundColor);
    QColor getBackgroundColor() const;

    // 设置 和 获取 线条颜色
    void setLineColor(const QColor& lineColor);
    QColor getLineColor() const;

    // 获取 字数
    int getWordCount() const;

public slots:
    // 设置 字数
    void setWordCount(int wordCount);

    // 文本编辑编辑页 关闭 处理槽函数
    void do_editWorkspace_noFile();

    // QWidget interface
protected:
    // 重写绘画事件，绘制背景
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
    // 绘制边框
    void drawBorder(QPainter *painter);

signals:

private:
    // 成员控件
    QLabel *m_labWordCount;

    // 成员变量
    QColor m_backgroundColor;
    QColor m_lineColor;
    int m_wordCount;
};

#endif // EDITSTATUSBAR_H
