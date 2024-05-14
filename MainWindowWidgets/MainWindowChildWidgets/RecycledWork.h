#ifndef RECYCLEDWORK_H
#define RECYCLEDWORK_H

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QPainter>
#include "GlobalClass/AppEnumClass.h"

class RecycledWork : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor RecycledWorkBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor RecycledWorkHoverBackgroundColor READ getHoverBackgroundColor WRITE setHoverBackgroundColor)
    Q_PROPERTY(QColor RecycledWorkLineColor READ getLineColor WRITE setLineColor)
public:
    explicit RecycledWork(QWidget *parent = nullptr);

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor& backgroundColor);
    QColor getBackgroundColor() const;

    // 设置 和 获取 光标悬停时的背景颜色
    void setHoverBackgroundColor(const QColor& hoverBackgroundColor);
    QColor getHoverBackgroundColor() const;

    // 设置 和 获取 线条颜色
    void setLineColor(const QColor& lineColor);
    QColor getLineColor() const;

    // 设置 和 获取 选中状态
    void setChecked(const bool checked);
    bool getChecked() const;

    // 设置 和 获取 作品ID
    void setWorkID(const QString workID);
    QString getWorkID() const;

    // 设置 和 获取 作品类型
    void setWorkType(const RecycledWorkType workType);
    RecycledWorkType getWorkType() const;

    // 设置 和 获取 索引
    void setIndex(const int index);
    int getIndex() const;

    // 设置 和 获取 作品名称
    void setWorkTitle(const QString workTitle);
    QString getWorkTitle() const;

    // 设置 和 获取 作品删除时间
    void setWorkDeleteTime(const QString deleteTime);
    QString getWorkDeleteTime() const;

    // 设置 和 获取 剩余时间
    void setRemaningTime(const QString remaningTime);
    QString getRemaningTime() const;

    // 设置 和 获取 字数
    void setWordCount(const int workCount);
    int getWorkCount() const;

    // 设置 和 获取 所属的小说
    void setBelongsNovel(const QString belongsNovel);
    QString getBelongsNovel() const;

    // 设置全部信息
    void setRecycledWorkInfo(const QString workID, const QString workTitle,
                             const QString deleteTime, const QString remaningTime,
                             const RecycledWorkType workType, const int wordCount,
                             const QString belongsNovel, const int index);

public slots:
    // void do_recycleTitleBar_checkStateChanged(bool checked);

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

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

    // 绘制背景
    void drawBackground(QPainter *painter);
    // 绘制线条
    void drawLines(QPainter *painter);

signals:
    // 标题复选框 状态改变
    void recycledWork_checkStateChanged(bool checked);

private:
    // 成员控件
    QCheckBox *m_ckbSelect;
    QLabel *m_labWorkTitle; // 作品名称
    QLabel *m_labWorkType; // 作品类型
    QLabel *m_labDeleteTime; // 删除时间
    QLabel *m_labRemaningTime; // 剩余时间
    QLabel *m_labWorkCount; // 字数
    QLabel *m_labBelongsNovel; // 所属的小说

    // 成员变量
    QColor m_backgroundColor;
    QColor m_hoverBackgroundColor;
    QColor m_lineColor;

    // bool m_isChecked; // 是否被选中
    RecycledWorkType m_workType; // 作品类型
    QString m_workID; // 作品ID
    int m_wordCount; // 作品字数
    int m_index; // 索引

    bool m_isHover; // 鼠标悬停
};




#endif // RECYCLEDWORK_H
