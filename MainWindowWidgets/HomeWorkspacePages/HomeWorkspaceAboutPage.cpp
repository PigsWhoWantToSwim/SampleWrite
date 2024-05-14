#include "HomeWorkspaceAboutPage.h"

HomeWorkspaceAboutPage::HomeWorkspaceAboutPage(QWidget *parent)
    : BasedHomeWorkspacePage{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void HomeWorkspaceAboutPage::initMemberVariable()
{
    m_APPVersion = "简单写作 for Windows v0.0.1";
    m_developerInfo = "本软件由  一只想游泳的猪  个人制作";
    m_APPProjectURL = "https://github.com/PigsWhoWantToSwim/SampleWrite.git";
}
// 初始化 布局
void HomeWorkspaceAboutPage::initLayout()
{
    // 容器布局，垂直布局，上方为 软件信息区，下方为 开发人员信息区，最下方为 弹簧
    QVBoxLayout *containerLayout = new QVBoxLayout;
    containerLayout->setSpacing(0);
    containerLayout->setContentsMargins(50,40,50,0);

    // 软件信息区
    QFrame *frmAPPInfo = new QFrame(m_widContainer);
    containerLayout->addWidget(frmAPPInfo);
    // 软件信息区布局，上方为 标题，中间为 软件信息，下方为 检查更新
    QVBoxLayout *APPInfoLayout = new QVBoxLayout;
    APPInfoLayout->setSpacing(5);
    APPInfoLayout->setContentsMargins(0,0,0,0);
    frmAPPInfo->setLayout(APPInfoLayout);

    QLabel *labAPPInfoTitle = new QLabel(frmAPPInfo);
    labAPPInfoTitle->setProperty("HomeAboutPageWidgets","TitleLabel");
    labAPPInfoTitle->setText("当前版本");
    APPInfoLayout->addWidget(labAPPInfoTitle);

    QLabel *labAPPInfoVersion = new QLabel(frmAPPInfo);
    labAPPInfoVersion->setProperty("HomeAboutPageWidgets","TextLabel");
    labAPPInfoVersion->setText(m_APPVersion);
    labAPPInfoVersion->setTextInteractionFlags(Qt::TextSelectableByMouse);
    APPInfoLayout->addWidget(labAPPInfoVersion);

    m_btnAPPUpdate = new QPushButton(frmAPPInfo);
    m_btnAPPUpdate->setProperty("HomeAboutPageWidgets","Button");
    m_btnAPPUpdate->setText("检查更新");
    m_btnAPPUpdate->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    m_btnAPPUpdate->setFixedSize(75, 30);
    APPInfoLayout->addWidget(m_btnAPPUpdate);

    QFrame *hLine_1 = new QFrame(m_widContainer);
    hLine_1->setProperty("HomeAboutPageWidgets", "Line");
    hLine_1->setFrameStyle(QFrame::HLine | QFrame::Plain);
    containerLayout->addWidget(hLine_1);

    containerLayout->addSpacing(20);

    // 开发人员信息区
    QFrame *frmDeveloperInfo = new QFrame(m_widContainer);
    containerLayout->addWidget(frmDeveloperInfo);
    // 开发人员信息区布局，上方为 标题，中间为 开发人员信息区信息，下方为 项目连接
    QVBoxLayout *frmDeveloperInfoLayout = new QVBoxLayout;
    frmDeveloperInfoLayout->setSpacing(0);
    frmDeveloperInfoLayout->setContentsMargins(0,0,0,0);
    frmDeveloperInfo->setLayout(frmDeveloperInfoLayout);

    QLabel *labDeveloperInfoTitle = new QLabel(frmDeveloperInfo);
    labDeveloperInfoTitle->setProperty("HomeAboutPageWidgets","TitleLabel");
    labDeveloperInfoTitle->setText("关于软件");
    frmDeveloperInfoLayout->addWidget(labDeveloperInfoTitle);

    QLabel *labDeveloperInfo = new QLabel(frmDeveloperInfo);
    labDeveloperInfo->setProperty("HomeAboutPageWidgets","TextLabel");
    labDeveloperInfo->setText(m_developerInfo);
    labDeveloperInfo->setTextInteractionFlags(Qt::TextSelectableByMouse);
    frmDeveloperInfoLayout->addWidget(labDeveloperInfo);

    QLabel *labProjectInfo = new QLabel(frmDeveloperInfo);
    labProjectInfo->setProperty("HomeAboutPageWidgets","URLLabel");
    labProjectInfo->setOpenExternalLinks(true);//设置为true才能打开网页
    labProjectInfo->setText(QString("<a style ='color: #1bb28b; text-decoration: none' "
                                    "href = \"%1\">项目地址：%1").arg(m_APPProjectURL));
    labProjectInfo->setCursor(QCursor(Qt::PointingHandCursor)); // 设置 光标形状
    // 超链接跳转 与 选中文本 冲突
    //连接信号槽
    connect(labProjectInfo, &QLabel::linkActivated, [=](QString url){
        QDesktopServices::openUrl(QUrl(url));
    });
    frmDeveloperInfoLayout->addWidget(labProjectInfo);

    QFrame *hLine_2 = new QFrame(m_widContainer);
    hLine_2->setProperty("HomeAboutPageWidgets", "Line");
    hLine_2->setFrameStyle(QFrame::HLine | QFrame::Plain);
    containerLayout->addWidget(hLine_2);

    // 添加弹簧
    containerLayout->addSpacerItem(new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Expanding));

    m_widContainer->setLayout(containerLayout);
    m_scrollAreaLayout->addWidget(m_widContainer);

    m_scrollArea->setLayout(m_scrollAreaLayout);
    m_scrollArea->setWidget(m_widContainer);

    m_mainLayout->addWidget(m_scrollArea);
    this->setLayout(m_mainLayout);
}

void HomeWorkspaceAboutPage::initConnect()
{
    connect(m_btnAPPUpdate, &QPushButton::clicked, this, &HomeWorkspaceAboutPage::do_btnAPPUpdate_clicked);
}

void HomeWorkspaceAboutPage::setAPPVersion(QString APPVersion)
{
    m_APPVersion = "简单写作 for Windows " + APPVersion;
    update();
}

QString HomeWorkspaceAboutPage::getAPPVersion()
{
    return m_APPVersion;
}

void HomeWorkspaceAboutPage::setDeveloperInfo(QString developerInfo)
{
    m_developerInfo = developerInfo;
    update();
}

QString HomeWorkspaceAboutPage::getDeveloperInfo()
{
    return m_developerInfo;
}

void HomeWorkspaceAboutPage::setAPPProjectURL(QString URL)
{
    m_APPProjectURL = URL;
    update();
}

QString HomeWorkspaceAboutPage::getAPPProjectURL()
{
    return m_APPProjectURL;
}

void HomeWorkspaceAboutPage::do_btnAPPUpdate_clicked()
{
    QMessageBox::information(this, "检查更新", "暂无更新", QMessageBox::Ok);
}
