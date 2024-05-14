#ifndef RESTOREHISTORICALVERSIONDIALOG_H
#define RESTOREHISTORICALVERSIONDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include "GlobalClass/AppEnumClass.h"


class RestoreHistoricalVersionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RestoreHistoricalVersionDialog(QWidget *parent = nullptr);

    // 设置 文章内容
    void setTextEditContent(QString filePath);

signals:
    // 是否取消 恢复历史版本
    void isCancelRestoreVersion(bool cancel);

private:
    // 初始化
    void initMemberVariable();
    void initLayout();
    void initConnect();

private:
    // 成员控件
    QFrame *m_frmContainer;

    QLabel *m_labTitle;
    QTextEdit *m_tedBackupChapter;

    QPushButton *m_btnSure;
    QPushButton *m_btnCancel;
};

#endif // RESTOREHISTORICALVERSIONDIALOG_H
