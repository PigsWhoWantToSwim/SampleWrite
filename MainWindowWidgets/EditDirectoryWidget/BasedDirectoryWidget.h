#ifndef BASEDDIRECTORYWIDGET_H
#define BASEDDIRECTORYWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QHeaderView>
#include <QDir>
#include <QFile>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QCoreApplication>
#include <QFileDialog>
#include "GlobalClass/AppEnumClass.h"
#include "GlobalClass/AppDataBase.h"
#include "GlobalClass/MaskFrame.h"
#include "MainWindowDialog/RenameDirectoryItemDialog.h"

class BasedDirectoryWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit BasedDirectoryWidget(QWidget *parent = nullptr);

    // 更新 目录 列宽度
    void updateDirectoryColumnWidth();

    // 清除 已发送的章节ID
    void clearChapterIDSent();

protected:
    // 初始化 成员函数
    virtual void initMemberVariable() = 0;
    // 初始化 布局
    virtual void initLayout() = 0;
    // 初始化 绑定
    virtual void initConnect() = 0;

    // 读取数据库数据
    virtual void readDataBase() = 0;
    // 更新 数据库数据
    virtual void updateDateBase() = 0;
    // 更新 节点数据
    virtual void updateItemData() = 0;

    // 添加
    // 修改
    QString getNewTitleFromDialog(QString dialogTitle, QString orignalTitle,
                        QString placeholderText, bool &isCancelRename); // 获取 节点新标题

    void renameItem(QTreeWidgetItem *item);
    // 删除
    // 导出
    void exportChapterItemAsTXT(QTreeWidgetItem *chapterItem, QString savedFilePath);

signals:
    // 章节点 双击，发送 章节点的 父卷索引，章节索引和章ID
    void chapterItem_DoubleClicked(QString chapterID,QString chapterTitle); // 发送 双击的章节点 的章ID

    // 章节点 重命名后，更新 文本框标题，没想好怎么中转接收
    void chapterItem_renamed(QString chapterID, QString newChapterTitle);

    // 保存 正在打开的 章节
    void saveBeingOpenedChapter();

    // 分卷、章节、作品相关已删除
    void directioryItem_deleted();

protected:
    // 成员变量
    QString m_novelID;
    QString m_chapterIDSent;

    QString m_newTitleText=""; // 用于接收 新的标题名
    QTreeWidgetItem *m_draggedItem=nullptr; // 用于暂存 要拖拽的 节点

    Qt::SortOrder m_order;
};

#endif // BASEDDIRECTORYWIDGET_H
