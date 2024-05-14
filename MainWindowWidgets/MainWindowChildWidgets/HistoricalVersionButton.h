#ifndef HISTORICALVERSIONBUTTON_H
#define HISTORICALVERSIONBUTTON_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QHBoxLayout>
#include "GlobalClass/AppEnumClass.h"

class HistoricalVersionButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor HistoricalVersionButtonBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor HistoricalVersionButtonHoverBackgroundColor READ getHoverBackgroundColor WRITE setHoverBackgroundColor)
    Q_PROPERTY(QColor HistoricalVersionButtonLineColor READ getLineColor WRITE setLineColor)
public:
    explicit HistoricalVersionButton(QWidget *parent = nullptr);

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor& backgroundColor);
    QColor getBackgroundColor() const;
    // 设置 和 获取 鼠标悬停时背景颜色
    void setHoverBackgroundColor(const QColor& hoverBackgroundColor);
    QColor getHoverBackgroundColor() const;

    // 设置 和 获取 线条颜色
    void setLineColor(const QColor& lineColor);
    QColor getLineColor() const;

    // 设置 控件变量数据
    void setButtonInfo(QString versionID, QString filePath, QString title, QString birthTime, int wordCount);

    // QWidget interface
protected:
    // 重写 绘画事件，绘制背景
    virtual void paintEvent(QPaintEvent *event) override;
    // 重写 鼠标 双击事件
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    // 重写 尺寸改变事件
    virtual void resizeEvent(QResizeEvent *event) override;

    // 鼠标进入和离开 事件
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

private:
    // 初始化 成员函数
    void initMemberVariable();
    // 初始化 布局
    void initLayout();
    // 初始化 绑定事件
    void initConnect();


signals:
    // 按钮双击事件
    void historicalVersionButton_DoubleClicked(QString filePath);

private:
    // 成员控件
    QLabel *m_labTitle;
    QLabel *m_labBirthTime;
    QLabel *m_labWordCount;

    // 背景颜色 和 线条颜色
    QColor m_backgroundColor;
    QColor m_hoverBackgroundColor;
    QColor m_lineColor;

    // 成员变量
    QString m_versionID; // 版本ID
    QString m_filePath; // 版本文件路径

    bool m_isHover; // 鼠标悬停
};

#endif // HISTORICALVERSIONBUTTON_H
