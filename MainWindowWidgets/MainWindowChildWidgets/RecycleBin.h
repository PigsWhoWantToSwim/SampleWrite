#ifndef RECYCLEBIN_H
#define RECYCLEBIN_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPainter>
#include <QMessageBox>
#include <QDateTime>
#include "GlobalClass/AppDataBase.h"
#include "GlobalClass/AppEnumClass.h"
#include "RecycleBinTitleBar.h"
#include "RecycledWork.h"


class RecycleBin : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor RecycleBinBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
public:
    explicit RecycleBin(QWidget *parent = nullptr);

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor& backgroundColor);
    QColor getBackgroundColor() const;

    // 获取 总数
    int getTotalCount() const;

    // 更新内容
    void updateRecycleBin();

public slots:
    // 恢复 回收项
    void do_recycleBin_restore();
    // 彻底删除 回收项
    void do_recycleBin_clear();
    // 清空 回收站
    void do_recycleBin_clearAll();

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

    // 读取 数据库数据
    void readDataBase();

    // 彻底清空小说、分卷、章节、作品相关
    void novel_clear(QString novelID);
    void volume_clear(QString volumeID);
    void chapter_clear(QString chapterID);
    // void workRelated_clear(QString workRelatedID); 与章节合并

    // 恢复 小说、分卷、章节、作品相关
    void novel_restore(QString novelID);
    void volume_restore(QString volumeID);
    void chapter_restore(QString chapterID);
    void workRelated_restore(QString workRelatedID);

    // 检查到期
    void checkRecycledWorksExpiration();

private slots:
    // 回收项 状态改变
    void do_recycleTitleBar_checkStateChanged(bool checked);
    // 回收项 状态改变
    void do_recycledWork_checkStateChanged(bool checked);

signals:
    // 选中项 数量改变
    void selectedCount_changed(int selectCount);

    // 总数量改变
    void totalCount_changed(int totalCount);

    // 恢复回收项
    void recycledWork_restored();

private:
    // 成员控件
    RecycleBinTitleBar *m_recycleBinTitleBar;

    // 滚动区
    QScrollArea *m_scrollArea;
    // 滚动区 幕布容器
    QFrame *m_frmScrollAreaContainer;
    // 滚动区 幕布容器内的布局
    QVBoxLayout *m_containerLayout;
    // 弹簧
    QSpacerItem *m_spacer;

    QList<RecycledWork*> m_listRecycledWorks;

    // 成员变量
    QColor m_backgroundColor;

    int m_totalCount;
};

#endif // RECYCLEBIN_H
