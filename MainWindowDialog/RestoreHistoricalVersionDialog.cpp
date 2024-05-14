#include "RestoreHistoricalVersionDialog.h"

// 构造函数
RestoreHistoricalVersionDialog::RestoreHistoricalVersionDialog(QWidget *parent)
    : QDialog{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}


// 初始化 成员变量
void RestoreHistoricalVersionDialog::initMemberVariable()
{
    // 设置透明
    this->setAttribute(Qt::WA_TranslucentBackground); // 隐藏4个角

    // 设置无边框属性
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->setFixedSize(RESTOREHISTORICALVERSIONDIALOG_WIDTH, RESTOREHISTORICALVERSIONDIALOG_HEIGHT);

}
// 初始化 布局
void RestoreHistoricalVersionDialog::initLayout()
{
    // 主布局，水平布局，仅安放主页面框
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    this->setLayout(mainLayout);

    // 用于设置圆角的主页面
    m_frmContainer = new QFrame(this);
    m_frmContainer->setProperty("WidgetStyleClass", "DialogContainerFrame");
    mainLayout->addWidget(m_frmContainer);

    // 主页面布局，水平布局，上方为 标题，中间为 文本框，下方为 按钮区
    QVBoxLayout *frameMainLayout = new QVBoxLayout;
    frameMainLayout->setSpacing(10);
    frameMainLayout->setContentsMargins(20,20,20,20);
    m_frmContainer->setLayout(frameMainLayout); // 将主布局 作为 主页面的布局

    // 对话框标题 标签
    m_labTitle = new QLabel("版本内容", m_frmContainer);
    m_labTitle->setProperty("WidgetStyleClass", "DialogTitleLabel");
    QFont f=m_labTitle->font();
    f.setStyleStrategy(QFont::PreferAntialias);
    m_labTitle->setFont(f);
    frameMainLayout->addWidget(m_labTitle);

    // 版本内容 文本框
    m_tedBackupChapter = new QTextEdit(m_frmContainer);
    m_tedBackupChapter->setObjectName("RestoreHistoricalVersionDialog_tedBackupChapter");
    m_tedBackupChapter->setReadOnly(true);
    m_tedBackupChapter->setFrameShape(QFrame::NoFrame);
    frameMainLayout->addWidget(m_tedBackupChapter);

    // 按钮区，水平布局，左侧为 弹簧，中间为 取消按钮，右侧为 确认按钮
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(30);
    btnLayout->setContentsMargins(0,0,0,0);
    frameMainLayout->addLayout(btnLayout); // 将按钮区 添加到 主布局

    // 添加弹簧
    btnLayout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed));

    // 取消按钮
    m_btnCancel = new QPushButton("取消", m_frmContainer);
    m_btnCancel->setProperty("WidgetStyleClass", "CancelButton");
    btnLayout->addWidget(m_btnCancel);

    // 确认按钮
    m_btnSure = new QPushButton("确认恢复", m_frmContainer);
    m_btnSure->setProperty("WidgetStyleClass", "SureButton");
    btnLayout->addWidget(m_btnSure);


}
// 初始化 绑定
void RestoreHistoricalVersionDialog::initConnect()
{
    // 绑定 取消按钮
    connect(m_btnCancel,&QPushButton::clicked,this,[=](){
        emit isCancelRestoreVersion(true);
        this->close();
    });

    // 绑定 确定按钮
    connect(m_btnSure,&QPushButton::clicked,this,[=](){
        emit isCancelRestoreVersion(false);
        this->close();
    });
}

void RestoreHistoricalVersionDialog::setTextEditContent(QString filePath)
{
    // 打开文件
    QFile file(filePath); //通过文件路径创建文件对象
    file.open(QFile::ReadOnly); //文件打开方式
    QString textContent = file.readAll(); //获取qss中全部字符
    file.close(); // 关闭文件

    // 向 标题输入框和文本框 设置内容
    m_tedBackupChapter->setText(textContent);
}




