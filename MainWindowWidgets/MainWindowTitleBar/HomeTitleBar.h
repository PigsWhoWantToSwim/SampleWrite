#ifndef HOMETITLEBAR_H
#define HOMETITLEBAR_H

#include <QWidget>
#include "BasedMainWindowTitleBar.h"
#include "GlobalClass/AppEnumClass.h"
#include <QPainter>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#define HOMETITLEBAE_HEIGHT 80
#define HOMETITLEBAR_ICON_HEIGHT 60
#define HOMETITLEBAR_ICON_WIDTH 200

class HomeTitleBar : public BasedMainWindowTitleBar
{
    Q_OBJECT
public:
    explicit HomeTitleBar(QWidget *parent = nullptr);

    // 设置 和 获取 图标文件路径
    void setIconPath(QString iconPath);
    QString getIconPath() const;

    // BasedMainWindowTitleBar interface
public:
    // 重写 最大化、向下还原按钮 切换
    virtual void toggleMaxRestoreButtonVisible(bool isWindowMaximized) override;

public slots:
    // 页面改变 处理槽函数
    void do_HomeWorkspacePage_changed(HomeMainWindowPage page);

    // QWidget interface
protected:
    // 重写绘画事件，绘制背景和图标
    virtual void paintEvent(QPaintEvent *event) override;
    // 双击最大化、还原已在窗口实现，重写双击事件用于修改最大化、还原按钮可见
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

    // BaseMainWindowTitleBar interface
protected:
    // 重写 初始化 虚函数
    virtual void initMemberVariable() override;
    virtual void initLayout() override;
    virtual void initConnect() override;

    // BaseMainWindowTitleBar interface
private:
    // 将图片缩放到rect矩形能完全显示图片
    QPixmap scaledImageToRect(QPixmap pix, QRect rect);

    // 绘制图标
    void drawIcon(QPainter *painter);

signals:
    // 工作区页面改变 信号
    void HomeWorkspacePage_changed(HomeMainWindowPage page);

    // 新建 小说按钮 点击信号
    void HomeTitleBar_NewNovelBtn_clicked();

    // 主题、关于按钮点击信号
    void HomeTitleBar_SettingsAboutBtn_clicked();

private:
    // 成员控件
    QLabel *m_labPageName;
    QPushButton *m_btnNewNovel;
    QPushButton *m_btnSettings;
    QPushButton *m_btnAbout;
    // 成员变量
    QString m_iconPath;  // 图标文件路径
    qreal m_pixelRatio; // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真
};

#endif // HOMETITLEBAR_H
