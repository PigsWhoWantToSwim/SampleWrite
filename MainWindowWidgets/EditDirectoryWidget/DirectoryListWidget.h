#ifndef DIRECTORYLISTWIDGET_H
#define DIRECTORYLISTWIDGET_H

#include <QWidget>
#include "BasedDirectoryWidget.h"

class DirectoryListWidget : public BasedDirectoryWidget
{
    Q_OBJECT
public:
    // 构造函数
    explicit DirectoryListWidget(QString novel, QWidget *parent = nullptr);
    // 析构函数
    ~DirectoryListWidget();

    // 文本框的章节标题 发生变化，处理槽函数，更新对应章节点的标题
    void setChapterNewTitle(QString chapterID,QString newChapterTitle);
    // 设置 章节点 字数
    void setChapterNodeWordCount(QString chapterID,int wordCount);

    // 重新排序
    void reorder();

signals:

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void dragMoveEvent(QDragMoveEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;

    // BasedDirectoryWidget interface
protected:
    // 初始化
    virtual void initMemberVariable() override;
    virtual void initLayout() override;
    virtual void initConnect() override;

    // 读取、更新数据库和节点
    virtual void readDataBase() override;
    virtual void updateDateBase() override;
    virtual void updateItemData() override;

    // 添加
    void addChapterItem();
    // 修改
    // 删除
    void romoveChapterItem(QTreeWidgetItem *chapterItem);
    // 导出

private slots:
    // 右键 弹出菜单 处理槽函数
    void do_directoryList_MenuRequested(const QPoint &pos);


};

#endif // DIRECTORYLISTWIDGET_H
