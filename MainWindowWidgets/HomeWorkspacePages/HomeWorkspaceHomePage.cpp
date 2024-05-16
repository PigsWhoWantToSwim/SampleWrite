#include "HomeWorkspaceHomePage.h"

#include <QPushButton>

HomeWorkspaceHomePage::HomeWorkspaceHomePage(QWidget *parent)
    : BasedHomeWorkspacePage{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void HomeWorkspaceHomePage::initMemberVariable()
{
    // 右键 弹出自定义 菜单
    m_widContainer->setContextMenuPolicy(Qt::CustomContextMenu);

    // 容器布局
    m_containerLayout = new FlowLayout;
    // 弹簧
    // m_horizontalSpacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
    // m_verticalSpacer = new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding);
    // 初始 列数
    // m_novelWidgetColumeCount = 1;

    // 读取 数据库
    readDataBase();
}
// 初始化 布局
void HomeWorkspaceHomePage::initLayout()
{
    // 容器布局
    m_containerLayout->setSpacing(HOMEWORKSPACEHOMEPAGE_CONTAINERLAYOUT_HORIZONTALSPACING);
    m_containerLayout->setContentsMargins(HOMEWORKSPACEHOMEPAGE_CONTAINERLAYOUT_MARGINLEFT,
                                          HOMEWORKSPACEHOMEPAGE_CONTAINERLAYOUT_MARGINTOP,
                                          HOMEWORKSPACEHOMEPAGE_CONTAINERLAYOUT_MARGINRIGHT,
                                          HOMEWORKSPACEHOMEPAGE_CONTAINERLAYOUT_MARGINBOTTOM);


    // 小说控件 初始布局 为一列，resizeEvent时 自动调整
    foreach (NovelWidget *novelWidget, m_listNovelWidgets) {
        // 将小说控件添加到布局里
        m_containerLayout->addWidget(novelWidget);
    }

    // 添加弹簧
    // m_containerLayout->addItem(m_horizontalSpacer, 0, m_novelWidgetColumeCount, (m_listNovelWidgets.count()/m_novelWidgetColumeCount)+1, 1);
    // m_containerLayout->addItem(m_verticalSpacer, (m_listNovelWidgets.count()/m_novelWidgetColumeCount)+1, 0, 1, m_novelWidgetColumeCount+1);

    m_widContainer->setLayout(m_containerLayout);

    m_scrollAreaLayout->addWidget(m_widContainer);

    m_scrollArea->setLayout(m_scrollAreaLayout);
    m_scrollArea->setWidget(m_widContainer);

    m_mainLayout->addWidget(m_scrollArea);
    this->setLayout(m_mainLayout);
}
// 初始化 绑定
void HomeWorkspaceHomePage::initConnect()
{
    // 绑定 容器 右键 弹出自定义 菜单
    connect(m_widContainer, &QWidget::customContextMenuRequested,
            this, &HomeWorkspaceHomePage::do_blank_customContextMenuRequested);

}

// 读取数据库
void HomeWorkspaceHomePage::readDataBase()
{
    if(AppDatabase::getInstance()->openDatabase())
    {
        // 准备 接收数据的 变量
        QString novelID = "";
        QString novelTitle = "";
        QString novelAuthor = "";
        QString novelCoverPath = "";
        QString novelType = "";
        int volumesCount = 0;
        int chaptersCount = 0;
        int wordsCount = 0;
        int novelIndex = 0;


        QSqlQuery AppDB_query;	//执行操作类对象

        // 准备 查询小说 数据库语句
        AppDB_query.prepare("SELECT * FROM novels WHERE is_deleted = 0 ORDER BY novel_index ASC;");

        if(AppDB_query.exec()) // 数据库语句 运行成功
        {
            while(AppDB_query.next()) // 逐条获取 数据库信息
            {
                novelID = AppDB_query.value("novel_id").toString();
                novelTitle = AppDB_query.value("novel_title").toString();
                novelAuthor = AppDB_query.value("novel_author").toString();
                novelCoverPath = AppDB_query.value("novel_coverURL").toString();
                novelType = AppDB_query.value("novel_type").toString();

                volumesCount = AppDB_query.value("volumes_count").toInt();
                chaptersCount = AppDB_query.value("chapters_count").toInt();
                wordsCount = AppDB_query.value("words_count").toInt();
                novelIndex = AppDB_query.value("novel_index").toInt();

                // 新建 小说控件，设置信息
                NovelWidget *novel = new NovelWidget(m_widContainer);
                novel->setNovelInfo(novelID, novelTitle, novelAuthor, novelCoverPath,
                                    novelType, volumesCount, chaptersCount, wordsCount,
                                    novelIndex);
                // 绑定 小说控件 左键 发送小说ID
                connect(novel, &NovelWidget::novelWidget_mouseLeft_clicked, this, [=](QString novelID, QString novelTitle){
                    emit novelWidget_clicked(novelID, novelTitle);
                });

                // 绑定 小说控件 右键 弹出 自定义菜单
                connect(novel, &NovelWidget::customContextMenuRequested,
                        this, &HomeWorkspaceHomePage::do_novelWidget_customContextMenuRequested);

                // 添加到 小说控件 列表
                m_listNovelWidgets << novel;
            }
        }
    }
    else // 打开数据库 失败
    {
        QMessageBox::warning(this, "错误", "打开数据库失败");
    }
    // 关闭 数据库
    AppDatabase::quit();
}
// 更新数据库
void HomeWorkspaceHomePage::updateDataBase()
{

}
// 更新索引
void HomeWorkspaceHomePage::updateIndex()
{
    for(int index = 0; index  <m_listNovelWidgets.count(); index++) // 从被删除的控件的下一个开始设置新的索引
    {
        m_listNovelWidgets.at(index)->setIndex(index);
    }
}

// 删除控件
void HomeWorkspaceHomePage::deleteNovelWidget(NovelWidget *novelWidget)
{

    // 获取 小说ID
    QString novelID = novelWidget->getNovelID();

    if(AppDatabase::getInstance()->openDatabase()) // 打开数据库成功
    {
        // 获取时间
        QDateTime dateTime = QDateTime::currentDateTime();
        QString timestamp = dateTime.toString("yyyy-MM-dd hh:mm:ss");// 字符串格式化


        // 执行查询操作
        QSqlQuery novel_query;	//执行操作类对象

        // 准备 SQL语句
        novel_query.prepare("UPDATE novels SET is_deleted = 1, deleted_time = :deletedTime WHERE novel_id = :novelID;");

        // 绑定 数据
        novel_query.bindValue(":deletedTime", timestamp);
        novel_query.bindValue(":novelID", novelID);

        if(!novel_query.exec()) // 执行sql语句
            qDebug()<<novel_query.lastError(); // 执行 sql语句 失败

        // 结束 sql 语句
        novel_query.finish();
    }
    else
        QMessageBox::warning(this, "错误", "打开数据库失败");

    // 关闭数据库
    AppDatabase::getInstance()->quit();

    int index = novelWidget->getIndex();
    m_listNovelWidgets.removeAt(index);
    m_containerLayout->removeWidget(novelWidget);
    // 手动释放 内容
    delete novelWidget;
    // 指针置空
    novelWidget = nullptr;
    // 更新索引
    updateIndex();
    emit deleteNovel(novelID);
}

void HomeWorkspaceHomePage::do_novelAdded(QString novelID)
{
    if(AppDatabase::getInstance()->openDatabase())
    {
        // 准备 接收数据的 变量
        QString novelTitle = "";
        QString novelAuthor = "";
        QString novelCoverPath = "";
        QString novelType = "";
        int volumesCount = 0;
        int chaptersCount = 0;
        int wordsCount = 0;
        int novelIndex = 0;


        QSqlQuery AppDB_query;	//执行操作类对象

        // 准备 查询小说 数据库语句
        AppDB_query.prepare("SELECT * FROM novels WHERE is_deleted = 0 AND novel_id = :novelID;");
        AppDB_query.bindValue(":novelID", novelID);

        if(AppDB_query.exec()) // 数据库语句 运行成功
        {
            while(AppDB_query.next()) // 逐条获取 数据库信息
            {
                novelTitle = AppDB_query.value("novel_title").toString();
                novelAuthor = AppDB_query.value("novel_author").toString();
                novelCoverPath = AppDB_query.value("novel_coverURL").toString();
                novelType = AppDB_query.value("novel_type").toString();

                volumesCount = AppDB_query.value("volumes_count").toInt();
                chaptersCount = AppDB_query.value("chapters_count").toInt();
                wordsCount = AppDB_query.value("words_count").toInt();
                novelIndex = AppDB_query.value("novel_index").toInt();

                // 新建 小说控件，设置信息
                NovelWidget *novel = new NovelWidget(m_widContainer);
                novel->setNovelInfo(novelID, novelTitle, novelAuthor, novelCoverPath,
                                    novelType, volumesCount, chaptersCount, wordsCount,
                                    novelIndex);
                // 绑定 小说控件 左键 发送小说ID
                connect(novel, &NovelWidget::novelWidget_mouseLeft_clicked, this, [=](QString novelID, QString novelTitle){
                    emit novelWidget_clicked(novelID, novelTitle);
                });

                // 绑定 小说控件 右键 弹出 自定义菜单
                connect(novel, &NovelWidget::customContextMenuRequested,
                        this, &HomeWorkspaceHomePage::do_novelWidget_customContextMenuRequested);

                // 添加到 小说控件 列表
                m_listNovelWidgets << novel;
                m_containerLayout->addWidget(novel);
            }
        }
    }
    else // 打开数据库 失败
    {
        QMessageBox::warning(this, "错误", "打开数据库失败");
    }
    // 关闭 数据库
    AppDatabase::quit();

    // 更新
    update();
}

void HomeWorkspaceHomePage::do_novelEdited(QString novelID)
{
    // 准备 接收数据的 变量
    QString novelTitle = "";
    QString novelAuthor = "";
    QString novelCoverPath = "";
    QString novelType = "";
    int volumesCount = 0;
    int chaptersCount = 0;
    int wordsCount = 0;
    int novelIndex = 0;

    if(AppDatabase::getInstance()->openDatabase())
    {
        QSqlQuery AppDB_query;	//执行操作类对象

        // 准备 查询小说 数据库语句
        AppDB_query.prepare("SELECT * FROM novels WHERE is_deleted = 0 AND novel_id = :novelID;");
        AppDB_query.bindValue(":novelID", novelID);

        if(AppDB_query.exec()) // 数据库语句 运行成功
        {
            while(AppDB_query.next()) // 逐条获取 数据库信息
            {
                novelTitle = AppDB_query.value("novel_title").toString();
                novelAuthor = AppDB_query.value("novel_author").toString();
                novelCoverPath = AppDB_query.value("novel_coverURL").toString();
                novelType = AppDB_query.value("novel_type").toString();

                volumesCount = AppDB_query.value("volumes_count").toInt();
                chaptersCount = AppDB_query.value("chapters_count").toInt();
                wordsCount = AppDB_query.value("words_count").toInt();
                novelIndex = AppDB_query.value("novel_index").toInt();
            }
        }
    }
    else // 打开数据库 失败
    {
        QMessageBox::warning(this, "错误", "打开数据库失败");
    }
    // 关闭 数据库
    AppDatabase::quit();

    // 小说控件，设置信息
    foreach (NovelWidget *novel, m_listNovelWidgets) {
        if(novel->getNovelID() == novelID)
        {
            novel->setNovelInfo(novelID, novelTitle, novelAuthor, novelCoverPath,
                                novelType, volumesCount, chaptersCount, wordsCount,
                                novelIndex);
        }
    }

    // 更新
}
//
void HomeWorkspaceHomePage::do_updateHomePage()
{
    // 移除所有 回收项
    for(int i=0; i<m_listNovelWidgets.count(); i++)
    {
        NovelWidget *novleWidget = m_listNovelWidgets[i]; // 每次获取第一个

        // 从布局中 移除 小说控件
        m_containerLayout->removeWidget(novleWidget);

        if(novleWidget)
        {
            delete novleWidget;
            novleWidget = nullptr;
        }
    }
    m_listNovelWidgets.clear();

    // 重新读取数据库
    readDataBase();

    // 重新添加到布局
    // 添加 回收项
    foreach (NovelWidget *novleWidget, m_listNovelWidgets) {
        m_containerLayout->addWidget(novleWidget);
    }

    // 更新
    update();
}

// 在空白处 右键 弹出菜单
void HomeWorkspaceHomePage::do_blank_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu *newNovelMenu=new QMenu(m_widContainer);

    // 添加 内容
    newNovelMenu->addAction("新建作品",this,[=](){
        emit newNovel();
    });

    // 弹出 自定义菜单
    newNovelMenu->exec(QCursor::pos());
}

// 在小说控件 右键 弹出菜单
void HomeWorkspaceHomePage::do_novelWidget_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);

    // 获取要弹出菜单的书籍bookWidget
    NovelWidget *novel = qobject_cast<NovelWidget *>(sender());

    // 获取 小说信息
    QString novelID = novel->getNovelID();
    QString novelTitle = novel->getNovelTitle();
    QString author = novel->getAuthor();
    QString novelCoverPath =  novel->getNovelCoverPath();

    // 封面图片 不存在 或者 使用 默认封面
    if((!QFile::exists(novelCoverPath)) || (novelCoverPath == QDir::currentPath() + "/Config/NovelCover/default book cover.png"))
    {
        novelCoverPath = "";
    }
    QString novelType = novel->getNovelType();

    QMenu *editNovelMenu=new QMenu(m_widContainer);
    QMenu *exportMenu = new QMenu("导出作品",editNovelMenu);

    // 添加 内容
    editNovelMenu->addAction("修改作品",this,[=](){
        emit editNovel(novelID, novelTitle, author, novelType , novelCoverPath);
    });
    exportMenu->addAction("导出为TXT",this,[=](){
        // 当前文件路径，使用 小说名为 默认文件名
        QString curPath= QCoreApplication::applicationDirPath() + QString("/%1.txt").arg(novelTitle);

        QString dlgTitle = "导出章节"; // 对话框标题
        QString filter = "TXT(*.txt)"; // 文件类型
        QString filePath = QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);

        if(filePath != "") // 文件路径不为空，进行导出
        {
            // 进行 导出
            if(novel->exportNovelWidgetAsTXT(filePath)) // 导出成功，弹出 提示对话框
                QMessageBox::information(this,"导出作品", QString("作品: %1 导出完成!").arg(novelTitle));
        }
    });
    // 将 导出菜单 添加到 菜单
    editNovelMenu->addMenu(exportMenu);

    editNovelMenu->addAction("删除",this,[=](){
        // 打开 对话框
        QMessageBox::StandardButton resultBtn = QMessageBox::warning(this,"删除作品",QString("确认删除：%1 ?").arg(novelTitle),QMessageBox::Cancel|QMessageBox::Yes);

        // 点击 取消按钮
        if(resultBtn == QMessageBox::Cancel)
            return; // 直接结束，不做删除操作

        // 执行 删除
        deleteNovelWidget(novel);
    });

    // 弹出 自定义菜单
    editNovelMenu->exec(QCursor::pos());
}
