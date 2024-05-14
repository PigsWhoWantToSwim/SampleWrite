#ifndef EDITCATALOGBAR_H
#define EDITCATALOGBAR_H

#include <QWidget>
#include <QPushButton>
#include <QStackedWidget>
#include <QPainter>>
#include <QHBoxLayout>
#include <QMenu>
#include "GlobalClass/AppEnumClass.h"
#include "EditDirectoryWidget/DirectoryTreeWidget.h"
#include "EditDirectoryWidget/DirectoryListWidget.h"

class EditCatalogBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor EditCatalogBarBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
public:
    // explicit EditCatalogBar(QWidget *parent = nullptr);
    explicit EditCatalogBar(QString novelID, QWidget *parent = nullptr);

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor& backgroundColor);
    QColor getBackgroundColor() const;

    // 设置 和 获取 小说ID
    void setNovelID(QString novelID);
    QString getNovelID() const;

public slots:
    // 工具栏 目录按钮 点击 处理槽函数
    void do_editToolBarButton_clicked(EditToolBarButton toolButton, bool checked = false);

    // 文本框的章节标题 发生变化，处理槽函数，更新对应章节点的标题
    void do_chapterTitle_changed(QString chapterID, QString newChapterTitle);

    // 章节字数改变 处理槽函数
    void do_chapterWordCount_changed(int wordCount);

    // 文本编辑编辑页 关闭 处理槽函数
    void do_editWorkspace_noFile();

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;

private slots:
    // 更多按钮 点击 处理槽函数
    void do_btnSort_clicked();

private:
    // 切换目录页面
    void do_catalogPage_changed(CatalogPage page);

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
    // 发送 要打开的 章节
    void openChapter(QString openChapterID, QString openChapterTitle);

    // 发送 章节点重命名 的 新标题
    void openChapter_renamed(QString newOpenChapterID);

    // 发送 保存 当前打开的 章节 文件
    void beingOpenedChapter_save();

    // 目录项 已删除
    void directioryItem_deleted();

private:
    // 成员控件
    QPushButton *m_btnTree;
    QPushButton *m_btnList;
    // QPushButton *m_btnSort;
    QStackedWidget *m_stkContainer; // 栈容器

    DirectoryTreeWidget *m_directoryTree; // 目录树
    DirectoryListWidget *m_directoryList; // 目录列表

    // 成员变量
    QColor m_backgroundColor;

    QString m_novelID;
    QString m_openedChapterID;

};

#endif // EDITCATALOGBAR_H
