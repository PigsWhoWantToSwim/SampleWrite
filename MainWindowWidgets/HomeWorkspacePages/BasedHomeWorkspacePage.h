#ifndef BASEDHOMEWORKSPACEPAGE_H
#define BASEDHOMEWORKSPACEPAGE_H

#include <QWidget>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QMessageBox>

class BasedHomeWorkspacePage : public QWidget
{
    Q_OBJECT
public:
    explicit BasedHomeWorkspacePage(QWidget *parent = nullptr);

    virtual void updateContainerWidth();

    // QWidget interface
protected:
    // 重写 显示事件，在初始时 设置容器宽度与滚动区相等
    virtual void showEvent(QShowEvent *event) override;

protected:
    // 初始化
    virtual void initMemberVariable() = 0;
    virtual void initLayout() = 0;
    virtual void initConnect() = 0;

    // 解决 窗口状态发生变化，未更新容器宽度的bug
    // virtual void  do_windowStattus_changed() = 0;

signals:

protected:
    // 控件
    // 主布局
    QHBoxLayout *m_mainLayout;
    // 滚动区
    QScrollArea *m_scrollArea;
    // 滚动区布局
    QVBoxLayout *m_scrollAreaLayout;
    // 滚动区内用于放置控件的容器
    QWidget *m_widContainer;

};

#endif // BASEDHOMEWORKSPACEPAGE_H
