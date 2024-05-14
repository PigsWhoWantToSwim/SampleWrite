#ifndef HOMEWORKSPACERECYCLEPAGE_H
#define HOMEWORKSPACERECYCLEPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QPainter>
#include <QGridLayout>
#include "MainWindowWidgets/MainWindowChildWidgets/RecycleBin.h"
#include "MainWindowWidgets/MainWindowChildWidgets/RecycleStatusBar.h"


class HomeWorkspaceRecyclePage : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor HomeRecyclePageBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
public:
    explicit HomeWorkspaceRecyclePage(QWidget *parent = nullptr);

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor& backgroundColor);
    QColor getBackgroundColor() const;

public slots:
    // 删除小说处理槽函数
    void do_deleteNovel(QString novelID);
    // 目录栏 删除项 处理槽函数
    void do_directoryItem_deleted();

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

private slots:

signals:
    // 清空 回收站
    // void recycleBin_clearAll();
    // 恢复回收项
    void recycledWork_restored();

private:
    // 成员控件
    QPushButton *m_btnClearAll; // 清空按钮
    RecycleBin *m_recycleBin;
    RecycleStatusBar *m_recycleStatusBar;

    // 成员变量
    QColor m_backgroundColor;
};

#endif // HOMEWORKSPACERECYCLEPAGE_H
