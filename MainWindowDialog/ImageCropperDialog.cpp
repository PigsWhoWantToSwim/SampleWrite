#include "ImageCropperDialog.h"

// 构造函数
ImageCropperDialog::ImageCropperDialog(QWidget *parent)
    : QDialog{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void ImageCropperDialog::initMemberVariable()
{
    // 设置透明
    this->setAttribute(Qt::WA_TranslucentBackground); // 隐藏4个角
    // 关闭后删除
    //this->setAttribute(Qt::WA_DeleteOnClose, true);
    //设置无边框属性
    this->setWindowFlags(Qt::FramelessWindowHint);//  | windowFlags()
    this->setFixedSize(IMAGECROPPERDIALOG_WIDTH, IMAGECROPPERDIALOG_HEIGHT);

    m_isLoadError = false;
}
// 初始化 布局
void ImageCropperDialog::initLayout()
{
    // 主布局 ，水平布局，用于放置 页面容器
    QHBoxLayout *mainLayot = new QHBoxLayout;
    mainLayot->setSpacing(0);
    mainLayot->setContentsMargins(0,0,0,0);
    this->setLayout(mainLayot);

    // 页面容器
    m_frmContainer = new QFrame(this);
    m_frmContainer->setProperty("WidgetStyleClass", "DialogContainerFrame");
    mainLayot->addWidget(m_frmContainer);

    // 页面容器布局，垂直布局，从上往下依次是 对话框标题，中间为截取控件，下方为按钮区
    QVBoxLayout *containerLayout = new QVBoxLayout;
    containerLayout->setSpacing(10);
    containerLayout->setContentsMargins((IMAGECROPPERDIALOG_WIDTH - IMAGECROPPERWIDGET_WIDTH)/2,
                                        10,
                                        (IMAGECROPPERDIALOG_WIDTH - IMAGECROPPERWIDGET_WIDTH)/2,
                                        10);
    m_frmContainer->setLayout(containerLayout);

    QLabel *labDialogTitle = new QLabel(m_frmContainer);
    labDialogTitle->setProperty("WidgetStyleClass", "DialogTitleLabel");
    labDialogTitle->setText("裁剪图片");
    containerLayout->addWidget(labDialogTitle);

    m_imgCropper = new ImageCropperWidget(IMAGECROPPERWIDGET_WIDTH, IMAGECROPPERWIDGET_HEIGHT, m_frmContainer);
    containerLayout->addWidget(m_imgCropper);

    // containerLayout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Fixed,QSizePolicy::Expanding));

    // 按钮区，水平布局，左边是弹簧，中间是取消按钮，右边是确认按钮
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(30);
    btnLayout->setContentsMargins(0,0,0,0);
    containerLayout->addLayout(btnLayout);

    // 添加弹簧
    btnLayout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed));

    m_btnCancel=new QPushButton(m_frmContainer);
    m_btnCancel->setProperty("WidgetStyleClass", "CancelButton");
    m_btnCancel->setText("取消");
    m_btnCancel->setCursor(QCursor(Qt::PointingHandCursor));
    btnLayout->addWidget(m_btnCancel);

    m_btnSure=new QPushButton(m_frmContainer);
    m_btnSure->setProperty("WidgetStyleClass", "SureButton");
    m_btnSure->setText("确定");
    m_btnSure->setCursor(QCursor(Qt::PointingHandCursor));
    btnLayout->addWidget(m_btnSure);

}
// 初始化 绑定
void ImageCropperDialog::initConnect()
{
    connect(m_btnCancel, &QPushButton::clicked, this, [=](){
        this->close();
    });

    connect(m_btnSure, &QPushButton::clicked, this, &ImageCropperDialog::do_btnSure_clicked);
}

// 设置 要裁剪的图片
void ImageCropperDialog::setCroppedImage(QString croppedImage)
{
    QPixmap pixmap;
    if (!pixmap.load(croppedImage)) { // 加载图片失败
        // 警告
        QMessageBox::critical(this, "错误", "图片加载失败", QMessageBox::Ok);
        m_isLoadError = true;
        return;
    }

   m_imgCropper->setPixmapToCrop(pixmap);
}

// 是否加载 失败
bool ImageCropperDialog::isLoadImageError()
{
    return m_isLoadError;
}

void ImageCropperDialog::do_btnSure_clicked()
{
    // 获取 裁剪后的图片
    QPixmap croppedPixmap = m_imgCropper->getCroppedPixmap();

    // 将 裁剪后的图片发送出去
    emit croppedImage_sent(croppedPixmap);

    // 关闭 对话框
    this->close();
}

