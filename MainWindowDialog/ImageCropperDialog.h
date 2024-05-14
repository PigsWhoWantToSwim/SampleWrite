#ifndef IMAGECROPPERDIALOG_H
#define IMAGECROPPERDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "GlobalClass/AppEnumClass.h"
#include "MainWindowWidgets/MainWindowChildWidgets/ImageCropperWidget.h"


class ImageCropperDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ImageCropperDialog(QWidget *parent = nullptr);

    // 设置要裁剪的图片
    void setCroppedImage(QString croppedImage);

    // 是否加载失败
    bool isLoadImageError();

private slots:
    void do_btnSure_clicked();

private:
    // 初始化
    void initMemberVariable();
    void initLayout();
    void initConnect();


signals:
    void croppedImage_sent(QPixmap croppedPixmap);

private:
    // 成员控件
    QFrame *m_frmContainer;

    // 裁剪图片 控件
    ImageCropperWidget *m_imgCropper;

    // 确认、取消按钮
    QPushButton *m_btnSure;
    QPushButton *m_btnCancel;

    // 成员变量
    bool m_isLoadError;
};

#endif // IMAGECROPPERDIALOG_H
