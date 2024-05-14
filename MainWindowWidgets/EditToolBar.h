#ifndef EDITTOOLBAR_H
#define EDITTOOLBAR_H

#include <QWidget>
#include <QPainter>
#include <QToolButton>
#include <QHBoxLayout>
#include <QButtonGroup>
#include "GlobalClass/AppEnumClass.h"

class EditToolBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor EditToolBarBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
public:
    explicit EditToolBar(QWidget *parent = nullptr);

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor &backgroundColor);
    QColor getBackgroundColor() const;

public slots:
    // 打开 章节 处理槽函数
    void do_openChapter(QString openChapterID, QString openChapterTitle);

    // 目录栏、查找栏、替换栏、侧边栏 页面关闭 处理 槽函数
    void do_editBarPages_close(EditToolBarButton toolButton);

    // 文本编辑编辑页 关闭 处理槽函数
    void do_editWorkspace_noFile();

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

private slots:
    // 互斥按钮组 按钮点击处理槽函数
    void do_exclusiveButton_clicked();

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
    // 按钮点击 信号
    void editToolBarButton_clicked(EditToolBarButton toolButton, bool checked = false);

    // 返回 首页 信号
    void returnHomeMainWindow();

private:
    // 成员控件
    QToolButton *m_tbtnNovelShelf;
    QToolButton *m_tbtnDirectory;

    QToolButton *m_tbtnUndo;
    QToolButton *m_tbtnRedo;
    QToolButton *m_tbtnTypesetting;

    QToolButton *m_tbtnFindReplace;

    QButtonGroup *m_exclusiveButtonGroup; // 互斥按钮组
    QToolButton *m_tbtnTheme;
    QToolButton *m_tbtnFont;
    QToolButton *m_tbtnLayout;
    QToolButton *m_tbtnHistoricalVersion;
    QToolButton *m_tbtnMoreSettings;

    // 成员变量
    QColor m_backgroundColor;


};

#endif // EDITTOOLBAR_H
