#include "RenameDirectoryItemDialog.h"

RenameDirectoryItemDialog::RenameDirectoryItemDialog(QWidget *parent)
    : QDialog{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void RenameDirectoryItemDialog::initMemberVariable()
{
    // 设置透明
    this->setAttribute(Qt::WA_TranslucentBackground); // 隐藏4个角
    // 关闭后删除
    //this->setAttribute(Qt::WA_DeleteOnClose); // 关闭后自动清除（调用析构函数）

    //设置无边框属性
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(RENAMEDIRECTORYITEMDIALOG_WIDTE, RENAMEDIRECTORYITEMDIALOG_HEIGHT);
}
// 初始化 布局
void RenameDirectoryItemDialog::initLayout()
{
    // 总布局，垂直布局，用来放置容器
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    // 用于设置圆角的主页面
    m_frmContainer = new QFrame(this);
    m_frmContainer->setProperty("WidgetStyleClass", "DialogContainerFrame");
    mainLayout->addWidget(m_frmContainer);

    // 容器布局，垂直布局，上方为 标题标签， 中间 小说信息区，下方为 按钮区
    QVBoxLayout *containerLayout = new QVBoxLayout;
    containerLayout->setSpacing(10);
    containerLayout->setContentsMargins(30, 10, 30, 10);
    m_frmContainer->setLayout(containerLayout);

    // 对话框标题 标签
    m_labDialogTitle = new QLabel("标题", m_frmContainer);
    m_labDialogTitle->setProperty("WidgetStyleClass", "DialogTitleLabel");
    containerLayout->addWidget(m_labDialogTitle);

    // 文本输入框
    m_ledNewTitle = new QLineEdit(m_frmContainer);
    m_ledNewTitle->setProperty("WidgetStyleClass", "RenameDirectoryItemDialog_TitleLineEdit");
    m_ledNewTitle->setPlaceholderText("请输入新标题");
    m_ledNewTitle->setClearButtonEnabled(true); // 清空输入按钮
    containerLayout->addWidget(m_ledNewTitle);

    containerLayout->addSpacing(15);

    // 按钮区，水平布局，左侧为 弹簧，中间为 取消按钮，右侧为 确认按钮
    QHBoxLayout *btnLayout=new QHBoxLayout;
    btnLayout->setSpacing(30);
    btnLayout->setContentsMargins(0,0,0,0);
    containerLayout->addLayout(btnLayout);

    // 添加弹簧
    btnLayout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding, QSizePolicy::Fixed));

    // 取消按钮
    m_btnCancel=new QPushButton("取消", m_frmContainer);
    m_btnCancel->setProperty("WidgetStyleClass", "CancelButton");
    m_btnCancel->setCursor(QCursor(Qt::PointingHandCursor));
    btnLayout->addWidget(m_btnCancel);

    // 确认按钮
    m_btnSure=new QPushButton("确定", m_frmContainer);
    m_btnSure->setProperty("WidgetStyleClass", "SureButton");
    m_btnSure->setCursor(QCursor(Qt::PointingHandCursor));
    btnLayout->addWidget(m_btnSure);
}
// 初始化 绑定
void RenameDirectoryItemDialog::initConnect()
{
    // 绑定 取消按钮，确认按钮 点击 槽函数
    connect(m_btnCancel, &QPushButton::clicked,this,[=](){
        emit isCancel(true);
        close();
    });
    connect(m_btnSure, &QPushButton::clicked, this, &RenameDirectoryItemDialog::do_btnSure_clicked);
}

// 设置对话框 标题
void RenameDirectoryItemDialog::setDialogTitle(QString dialogTitle)
{
    m_dialogTitle = dialogTitle;
    m_labDialogTitle->setText(dialogTitle);
    update();
}
// 设置 文本输入框 节点初始标题
void RenameDirectoryItemDialog::setLineEditTitle(QString orignalTitle)
{
    m_orignalItemTitle = orignalTitle;
    m_ledNewTitle->setText(orignalTitle);
    update();
}
// 设置 文本输入框 提示文本
void RenameDirectoryItemDialog::setPlaceholderText(QString placeholderText)
{
    m_ledNewTitle->setPlaceholderText(placeholderText);
    update();
}

// 确认按钮 点击 槽函数
void RenameDirectoryItemDialog::do_btnSure_clicked()
{
    if(m_ledNewTitle->text().simplified() == "") // 文本输入框 内容为空
    {
        QMessageBox::warning(this,"警告","书名不能为空");
        return;
    }

    // 将输入框的文本发送出去
    emit newTitle_sent(m_ledNewTitle->text().simplified());

    emit isCancel(false);
    // 关闭 对话框
    close();
}


