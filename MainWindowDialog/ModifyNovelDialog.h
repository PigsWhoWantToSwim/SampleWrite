#ifndef MODIFYNOVELDIALOG_H
#define MODIFYNOVELDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QEvent>
#include <QMessageBox>
#include <QFileDialog>
#include "GlobalClass/AppEnumClass.h"
#include "GlobalClass/AppDataBase.h"
#include "ImageCropperDialog.h"
#include "GlobalClass/MaskFrame.h"



class ModifyNovelDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ModifyNovelDialog(QWidget *parent = nullptr);

    // 设置 小说信息，用于打开修改小说信息对化框
    void setNovelInfo(QString novelID, QString novelTitle, QString author,
                      QString novelType, QString novelCoverPath);

    // QObject interface
public:
    // 事件过滤器
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    // 添加封面按钮 点击 处理槽函数
    void do_btnAddCover_clicked();
    // 取消封面按钮 点击 处理槽函数
    void do_btnCancelCover_clicked();

    // 接收截取后的图片
    void do_croppedImage_sent(QPixmap croppedPixmap);

    // 确认按钮 点击 处理槽函数
    void do_btnSure_clicked();

private:
    // 初始化
    void initMemberVariable();
    void initLayout();
    void initConnect();

    // 新建小说
    void newNovel();
    // 修改小说
    void editNovel();

signals:
    // 小说发生修改
    void updateNovels();

    // 添加小说
    void novelAdded(QString novelID);
    // 编辑小说
    void novelEdited(QString novelID);

private:
    // 成员控件
    QFrame *m_frmContainer;
    QLabel *m_labDialogTitle;

    // 封面容器、添加封面按钮、取消封面按钮
    QFrame *m_frmCoverContainer;
    QPushButton *m_btnAddCover;
    QPushButton *m_btnCancelCover;

    // 小说名、作者名、类型 标签和输入框
    QLabel *m_labNoverTitle;
    QLabel *m_labAuthor;
    QLabel *m_labNovelType;

    QLineEdit *m_ledNoverTitle;
    QLineEdit *m_ledAuthor;
    QLineEdit *m_ledNovelType;

    // 确认、取消按钮
    QPushButton *m_btnSure;
    QPushButton *m_btnCancel;

    // 成员 变量
    QString m_novelID;
    QString m_novelCoverPath;
    QPixmap m_novelCoverPixmap;

    bool m_isEdited;
    bool m_isCoverExisted;



};

#endif // MODIFYNOVELDIALOG_H
