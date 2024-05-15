#include "SideBarHistoricalPage.h"

// 构造函数
SideBarHistoricalPage::SideBarHistoricalPage(QWidget *parent)
    : BasedSiderBarPage{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}
// 析构函数
SideBarHistoricalPage::~SideBarHistoricalPage()
{
    // 释放 历史版本按钮列表 内存
    deleteAllHistoricalVersionButton();

    // 释放 弹簧内存
    if(m_spacer != nullptr)
    {
        delete m_spacer;
        m_spacer = nullptr;
    }
}

// 初始化 成员变量
void SideBarHistoricalPage::initMemberVariable()
{
    m_labTitle->setText("历史版本");
}
// 初始化 布局
void SideBarHistoricalPage::initLayout()
{
    // 容器布局
    QVBoxLayout *containerLayout = new QVBoxLayout;
    containerLayout->setSpacing(0);
    containerLayout->setContentsMargins(5,5,5,5);
    m_frmContainer->setLayout(containerLayout);

    // 滚动区
    m_scrollArea = new QScrollArea(m_frmContainer);
    m_scrollArea->setProperty("WidgetStyleClass", "ScrollArea");
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setWidgetResizable(true);


    // 滚动区布局
    QVBoxLayout *scrollAreaLayout = new QVBoxLayout;
    scrollAreaLayout->setSpacing(0);
    scrollAreaLayout->setContentsMargins(0,0,0,0);

    // 用于 放置按钮的容器
    m_frmScrollAreaContainer = new QFrame(m_scrollArea);
    m_frmScrollAreaContainer->setProperty("WidgetStyleClass", "SiderBarPage_ContainerFrame");
    m_frmScrollAreaContainer->setFrameShape(QFrame::NoFrame);
    scrollAreaLayout->addWidget(m_frmScrollAreaContainer);

    // 历史版本按钮区，从上到下依次是各历史版本按钮，最后是弹簧
    m_historicalVersionBtnLayout =new QVBoxLayout;
    m_historicalVersionBtnLayout->setSpacing(0);
    m_historicalVersionBtnLayout->setContentsMargins(0,0,0,0);
    m_frmScrollAreaContainer->setLayout(m_historicalVersionBtnLayout); // 将 滚动区布局 设置为 滚动区的布局

    // 添加弹簧
    m_spacer = new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Expanding);
    m_historicalVersionBtnLayout->addSpacerItem(m_spacer);

    // 将 放置按钮的容器 作为幕布
    m_scrollArea->setLayout(scrollAreaLayout);
    m_scrollArea->setWidget(m_frmScrollAreaContainer);
    containerLayout->addWidget(m_scrollArea);
}
// 初始化 绑定
void SideBarHistoricalPage::initConnect()
{

}

// 释放 旧历史版本按钮 内存，为新打开文章的历史版本做准备
void SideBarHistoricalPage::deleteAllHistoricalVersionButton()
{
    // 遍历 历史版本按钮列表
    foreach (HistoricalVersionButton *btn, m_listHistoricalVersionBtn)
    {
        if(btn != nullptr) // 历史版本按钮 不为空
        {
            // 从布局中 移除
            m_historicalVersionBtnLayout->removeWidget(btn);

            delete btn; // 释放内存
            btn = nullptr; // 指针置空
        }
    }
    // 移除弹簧 但不释放内存
    // if()
    m_historicalVersionBtnLayout->removeItem(m_spacer);

    // 清空 历史版本按钮列表
    m_listHistoricalVersionBtn.clear();
}
// 读取 数据库
void SideBarHistoricalPage::readSQLDate()
{
    // 打开数据库
    if (AppDatabase::getInstance()->openDatabase()) // 数据库打开成功
    {
        QString latestVersionID = "";
        int backupFileCount = 0;

        // 查询volumes表中 属于novel_id的所有volume记录
        QSqlQuery backupChapters_sql;	//执行操作类对象

        // 准备sql语句
        backupChapters_sql.prepare("SELECT MAX(version_id) AS latestVersionID, COUNT(version_id) AS backupFileCount FROM backup_chapters WHERE chapter_id = :chapterID;");

        // 绑定数据
        backupChapters_sql.bindValue(":chapterID",m_chapterID);

        backupChapters_sql.exec();
        if(backupChapters_sql.first()) // 数据库语句 运行成功
        {
            latestVersionID = backupChapters_sql.value("latestVersionID").toString();
            backupFileCount = backupChapters_sql.value("backupFileCount").toInt();

            // 发送 初始化信息
            emit initHistoricalVersionInfo(latestVersionID, backupFileCount);
        }

        // // 执行sql语句
        // if(!backupChapters_sql.exec())
        // {
        //     // 执行 sql语句 失败
        //     qDebug()<<backupChapters_sql.lastError();
        // }
        // else // 执行成功
        // {
        //     while(backupChapters_sql.next()) // 获取信息
        //     {
        //         latestVersionID = backupChapters_sql.value("latestVersionID").toString();
        //         backupFileCount = backupChapters_sql.value("backupFileCount").toInt();
        //     }
        //     // 发送 初始化信息
        //     emit initHistoricalVersionInfo(latestVersionID, backupFileCount);
        // }


        // 准备sql语句
        backupChapters_sql.prepare("SELECT version_id, birth_time, words_count, file_pathURL FROM backup_chapters WHERE chapter_id = :chapterID ORDER BY version_id DESC");

        // 绑定数据
        backupChapters_sql.bindValue(":chapterID", m_chapterID);

        // 执行sql语句
        if(!backupChapters_sql.exec())
        {
            // 执行 sql语句 失败
            qDebug()<<backupChapters_sql.lastError();
        }
        else // 执行成功
        {
            // 版本名
            int versionNumber = backupFileCount;
            // 读取每一条 chapter记录
            while(backupChapters_sql.next())
            {
                // 获取信息
                QString versionID = backupChapters_sql.value("version_id").toString();
                QString birthTime = backupChapters_sql.value("birth_time").toString();
                int wordCount = backupChapters_sql.value("words_count").toInt();
                QString filePath = backupChapters_sql.value("file_pathURL").toString();

                // 新建 历史版本按钮
                HistoricalVersionButton *btn = new HistoricalVersionButton(m_frmScrollAreaContainer);
                // 设置 数据
                btn->setButtonInfo(versionID, filePath, QString("版本%1").arg(versionNumber), birthTime, wordCount);
                // 设置 光标形状
                btn->setCursor(QCursor(Qt::PointingHandCursor));
                // 绑定 双击事件 槽函数
                connect(btn, &HistoricalVersionButton::historicalVersionButton_DoubleClicked, this, &SideBarHistoricalPage::do_historicalVersionButton_DoubleClicked);
                // 加入 历史版本按钮 列表
                m_listHistoricalVersionBtn << btn;

                // 版本名 +1
                versionNumber -= 1;
            }
        }
    }
    else // 数据库 打开 失败
        QMessageBox::warning(this, "错误", "打开数据库失败");

    // 关闭数据库
    AppDatabase::getInstance()->quit();
}
// 更新 历史版本按钮布局
void SideBarHistoricalPage::updateButtonArea()
{

    // 释放 历史版本按钮 内存
    deleteAllHistoricalVersionButton();

    // 读取 数据库数据
    readSQLDate();

    // 重新布置 历史版本按钮
    foreach (HistoricalVersionButton *btn, m_listHistoricalVersionBtn)
    {
        if(btn != nullptr) // 历史版本按钮 不为空
        {
            m_historicalVersionBtnLayout->addWidget(btn);
        }
    }
    // 重新添加 弹簧
    m_historicalVersionBtnLayout->addSpacerItem(m_spacer);
}

// 将历史版本 内容清空
void SideBarHistoricalPage::clearHistoricalPage()
{
    deleteAllHistoricalVersionButton();
    m_chapterID = "";
}

// 目录节点 双击 处理槽函数 重新设置 历史按钮
void SideBarHistoricalPage::do_openChapter(QString openChapterID)
{
    // 设置 章节数据
    m_chapterID = openChapterID;

    // 更新 历史版本按钮区
    updateButtonArea();
}

// 历史版本按钮 双击事件 处理槽函数，打开 恢复版本对话框
void SideBarHistoricalPage::do_historicalVersionButton_DoubleClicked(QString filePath)
{       
    // 是否 取消标志
    bool isCancel = true;

    // 打开 恢复对话框
    RestoreHistoricalVersionDialog *digRestore = new RestoreHistoricalVersionDialog;
    digRestore->setTextEditContent(filePath);
    // 绑定 是否取消，获取 确认或取消
    connect(digRestore, &RestoreHistoricalVersionDialog::isCancelRestoreVersion,
            this,[&](bool cancelFlag){
        isCancel = cancelFlag;
    });

    // 遮罩层
    MaskFrame *maskFrm = new MaskFrame(this);
    // this是 SideBarHistoricalPage历史版本页面
    // this->parentWidget()是 EditSideBar侧边栏的 QStackWidget栈容器
    // this->parentWidget()->parentWidget()是 EditSideBar侧边栏
    // this->parentWidget()->parentWidget()->parentWidget()是 splitter分裂器
    // this->parentWidget()->parentWidget()->parentWidget()->parentWidget()是 最顶层的容器
    QWidget *maskedWidget = this->parentWidget()->parentWidget()->parentWidget()->parentWidget();
    maskFrm->setMaskedWidget(maskedWidget); //设置 被遮罩的控件
    maskFrm->setUpperWidget(digRestore); // 设置 遮罩层 之上的控件

    // 设置 对话框 位置
    QPoint maskedWidgetLeftTop = maskedWidget->mapToGlobal(QPoint(0, 0));
    int x = maskedWidgetLeftTop.x() + (maskedWidget->width() - digRestore->width())/2;
    int y = maskedWidgetLeftTop.y() + (maskedWidget->height() - digRestore->height())/2;

    digRestore->move(QPoint(x, y));
    digRestore->exec(); // 显示 恢复对话框

    // 不是取消，确认 恢复
    if(!isCancel)
    {
        emit restoreVersion(filePath); // 发送要恢复文件 的文件路径
    }
}
