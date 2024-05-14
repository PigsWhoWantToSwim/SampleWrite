#ifndef RENAMEDIRECTORYITEMDIALOG_H
#define RENAMEDIRECTORYITEMDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "GlobalClass/AppEnumClass.h"

class RenameDirectoryItemDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RenameDirectoryItemDialog(QWidget *parent = nullptr);

    // 设置 对话框 标题
    void setDialogTitle(QString dialogTitle);

    // 设置 文字输入框 初始文本
    void setLineEditTitle(QString orignalTitle);
    // 设置 文字输入框 提示文本
    void setPlaceholderText(QString placeholderText);

private slots:
    // 确认 按钮点击 槽函数
    void do_btnSure_clicked();

private:
    // 初始化 成员函数
    void initMemberVariable();
    // 初始化 布局
    void initLayout();
    // 初始化 绑定
    void initConnect();

signals:
    // 发送 新标题
    void newTitle_sent(QString newTitle);

    // 发送 是否取消 重命名
    void isCancel(bool cancel);


private:
    // 成员控件
    QFrame *m_frmContainer;

    QLabel *m_labDialogTitle;
    QLineEdit *m_ledNewTitle;

    QPushButton *m_btnSure;
    QPushButton *m_btnCancel;

    // 成员变量
    QString m_dialogTitle; // 对话框标题
    QString m_orignalItemTitle; // 文本输入框 初始内容

};

#endif // RENAMEDIRECTORYITEMDIALOG_H
