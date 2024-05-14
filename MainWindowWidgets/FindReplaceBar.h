#ifndef FINDREPLACEBAR_H
#define FINDREPLACEBAR_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "GlobalClass/AppEnumClass.h"
#include "GlobalClass/SwitchButton.h"

/***********************************************************
 *******************      查找栏      ******************
***********************************************************/

class FindBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor FindBarBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor FindBarLineColor READ getLineColor WRITE setLineColor)
public:
    explicit FindBar(QWidget *parent = nullptr);

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor& backgroundColor);
    QColor getBackgroundColor() const;

    // 设置 和 获取 线条颜色
    void setLineColor(const QColor& lineColor);
    QColor getLineColor() const;

public slots:
    // 工具栏按钮点击 处理槽函数
    void do_editToolBarButton_clicked(EditToolBarButton toolButton, bool checked = false);


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
    // 绘制边框
    void drawLines(QPainter *painter);

private slots:
    // 清空 查找输入框 的内容
    void do_findText_cleared();

signals:
    // 关闭 查找替换页面
    void findReplaceBar_close(EditToolBarButton button);


    // 替换页面 显示状态 改变
    void replaceBarVisible_changed(bool checked);

    // 搜索查找内容
    void searchText(QString text);

    // 查找上一个
    void searchText_previous();
    // 查找下一个
    void searchText_next();

    // 搜索输入框被清空
    void searchText_cleared();

    // 搜索内容改变
    void searchText_changed();

private:
    // 成员控件
    QLineEdit *m_ledFind; // 查找 输入框
    QPushButton *m_btnFind; // 查找搜索 按钮
    QPushButton *m_btnFindPrevious; // 查找 上一个
    QPushButton *m_btnFindNext; // 查找 下一个
    SwitchButton *m_swbtnReplaceVisible; // 开关按钮 显示隐藏替换框


    // 成员变量
    QColor m_backgroundColor;
    QColor m_lineColor;

};


/***********************************************************
 *******************      替换栏      ******************
***********************************************************/

class ReplaceBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor ReplaceBarBackgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor ReplaceBarLineColor READ getLineColor WRITE setLineColor)
public:
    explicit ReplaceBar(QWidget *parent = nullptr);

    // 设置 和 获取 背景颜色
    void setBackgroundColor(const QColor& backgroundColor);
    QColor getBackgroundColor() const;

    // 设置 和 获取 线条颜色
    void setLineColor(const QColor& lineColor);
    QColor getLineColor() const;

    // 设置 替换输入框 使能禁止
    void setLineEditReplaceEnabled(bool enabledFlag);


public slots:
    // 显示 隐藏 替换区
    void do_replaceBarVisible_changed(bool checked);

    // 清空 替换输入框 内容
    void do_lineEditReplaceText_cleared();

    // QWidget interface
protected:
    // 重写 绘画事件，绘制背景
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
    // 绘制边框
    void drawLines(QPainter *painter);

private slots:
    // 替换输入框 内容变化，替换当前和替换全部按钮 使能禁止
    void do_replaceText_changed(QString replaceText);

signals:
    // 替换 当前
    void replaceCurrent(QString replaceText);
    // 替换 全部
    void replaceAll(QString replaceText);

private:
    // 成员控件
    QLineEdit *m_ledReplace; // 替换 输入框
    QPushButton *m_btnReplaceCurrent; // 替换当前 按钮
    QPushButton *m_btnReplaceAll; // 替换 全部 按钮

    // 成员变量
    QColor m_backgroundColor;
    QColor m_lineColor;
};

#endif // FINDREPLACEBAR_H
