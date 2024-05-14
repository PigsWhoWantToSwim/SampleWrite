#ifndef BLANKPAGE_H
#define BLANKPAGE_H

/********************************************************************
 ****************   编辑工作区 空白页   ******************************
*********************************************************************/

#include <QWidget>
#include <QLabel>
#include <QStackedWidget>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include "GlobalClass/AppEnumClass.h"

class BlankPage : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor BlankPageBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
public:
    explicit BlankPage(QWidget *parent = nullptr);

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor& backgroundColor);
    QColor getBackgroundColor() const;

    // 设置 和 获取 提示文本
    void setTipsText(const QString tipsText);
    QString getTipsText() const;

    // QWidget interface
protected:
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

private:
    // 成员控件
    QLabel *m_labImage;
    QLabel *m_labTips;

    // 成员变量
    QColor m_backgroundColor;
    QString m_tipsText;

};

#endif // BLANKPAGE_H
