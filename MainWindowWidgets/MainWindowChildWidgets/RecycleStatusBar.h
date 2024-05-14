#ifndef RECYCLESTATUSBAR_H
#define RECYCLESTATUSBAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPainter>
#include <QHBoxLayout>
#include <QMessageBox>
#include "GlobalClass/AppEnumClass.h"

class RecycleStatusBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor RecycleStatusBarBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor RecycleStatusBarLineColor READ getLineColor WRITE setLineColor)
public:
    explicit RecycleStatusBar(QWidget *parent = nullptr);

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor& backgroundColor);
    QColor getBackgroundColor() const;

    // 设置 和 获取 线条颜色
    void setLineColor(const QColor& lineColor);
    QColor getLineColor() const;

    // 设置 被选中项 数目
    int getSelectCount() const;

    // 获取 全部项 数目
    int getTotalCount() const;

public slots:
    // 设置 被选中项 数目
    void setSelectCount(const int selectCount);

    // 设置 全部项 数目
    void setTotalCount(const int totalCount);

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
    // 绘制线条
    void drawLines(QPainter *painter);

signals:
    // 恢复按钮被点击
    void btnRestore_clicked();
    // 彻底删除按钮被点击
    void btnDelete_clicked();

private:
    // 成员控件
    QLabel *m_labSelectedCount;
    QPushButton *m_btnRestore;
    QPushButton *m_btnDelete;

    // 成员变量
    QColor m_backgroundColor;
    QColor m_lineColor;

    int m_selectedCount;
    int m_totalCount;
};

#endif // RECYCLESTATUSBAR_H
