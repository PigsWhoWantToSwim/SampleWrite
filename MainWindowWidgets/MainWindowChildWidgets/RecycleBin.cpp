#include "RecycleBin.h"

RecycleBin::RecycleBin(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员函数
void RecycleBin::initMemberVariable()
{
    m_recycleBinTitleBar = new RecycleBinTitleBar(this);
    // 滚动区
    m_scrollArea = new QScrollArea(this);
    m_frmScrollAreaContainer = new QFrame(m_scrollArea);
    m_containerLayout = new QVBoxLayout;
    m_spacer = new QSpacerItem(10,10,QSizePolicy::Fixed,QSizePolicy::Expanding);

    m_backgroundColor = QColor(245, 246, 249);

    m_totalCount = 0;

    readDataBase();
}
// 初始化 布局
void RecycleBin::initLayout()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(30, 0, 10, 0);
    this->setLayout(mainLayout);


    // m_recycleBinTitleBar->setStyleSheet("background-color: #aedf04;");
    mainLayout->addWidget(m_recycleBinTitleBar);

    // 滚动区
    m_scrollArea->setProperty("WidgetStyleClass", "ScrollArea");
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setAlignment(Qt::AlignCenter);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setWidgetResizable(true);

    // 滚动区布局
    QVBoxLayout *scrollAreaLayout = new QVBoxLayout;
    scrollAreaLayout->setSpacing(0);
    scrollAreaLayout->setContentsMargins(0, 0, 0, 0);

    // 滚动区幕布
    m_frmScrollAreaContainer->setProperty("WidgetStyleClass", "ContainerFrame");
    scrollAreaLayout->addWidget(m_frmScrollAreaContainer);

    // 放置回收项的布局
    m_containerLayout->setSpacing(0);
    m_containerLayout->setContentsMargins(0, 0, 0, 0);
    m_frmScrollAreaContainer->setLayout(m_containerLayout);

    // 添加 回收项
    foreach (RecycledWork *recycleWork, m_listRecycledWorks) {
        m_containerLayout->addWidget(recycleWork);
    }

    // 检查到期
    checkRecycledWorksExpiration();

    // 添加弹簧
    m_containerLayout->addSpacerItem(m_spacer);

    // 将 放置按钮的容器 作为幕布
    m_scrollArea->setLayout(scrollAreaLayout);
    m_scrollArea->setWidget(m_frmScrollAreaContainer);
    mainLayout->addWidget(m_scrollArea);

}
// 初始化 绑定事件
void RecycleBin::initConnect()
{
    connect(m_recycleBinTitleBar, &RecycleBinTitleBar::recycleTitleBar_checkStateChanged,
            this, &RecycleBin::do_recycleTitleBar_checkStateChanged);
}

// 读取 数据库数据
void RecycleBin::readDataBase()
{
    m_totalCount = 0;

    if(AppDatabase::getInstance()->openDatabase())
    {
        // 准备 接收数据的 变量
        QString workID = "";
        QString workTitle = "";
        QString deletedTimeStr = "";

        int wordsCount = 0;
        QString belongsNovelTitle = "";

        // 时间
        QDateTime deletedTime;
        QDateTime currentTime = QDateTime::currentDateTime();
        QDateTime deadlineTime;
        // 与xxxx日期相隔秒数
        uint startSec;
        uint endSec;
        // 天数
        int dayCount = 0;

        QSqlQuery appDB_query;	//执行操作类对象

        // 准备 查询小说 数据库语句
        appDB_query.prepare("SELECT novel_id, novel_title, deleted_time, words_count FROM novels WHERE is_deleted = 1 ORDER BY deleted_time ASC;");

        if(appDB_query.exec()) // 数据库语句 运行成功
        {
            while(appDB_query.next()) // 逐条获取 数据库信息
            {
                workID = appDB_query.value("novel_id").toString();
                workTitle = appDB_query.value("novel_title").toString();
                deletedTimeStr = appDB_query.value("deleted_time").toString();
                wordsCount = appDB_query.value("words_count").toInt();
                belongsNovelTitle = workTitle;

                // 计算剩余时间
                deletedTime = QDateTime::fromString(deletedTimeStr, "yyyy-MM-dd hh:mm:ss");
                deadlineTime = deletedTime.addDays(10); // 10 天后彻底删除

                startSec = currentTime.toMSecsSinceEpoch()/1000;
                endSec = deadlineTime.toMSecsSinceEpoch()/1000;

                dayCount = (endSec - startSec) / DAY_SECOND;

                // 新建 小说控件，设置信息
                RecycledWork *recycleWork = new RecycledWork(m_frmScrollAreaContainer);
                // 设置全部信息
                recycleWork->setRecycledWorkInfo(workID, workTitle,
                                                 deletedTimeStr, QString("%1天").arg(dayCount),
                                                 RecycledWorkType::Novel, wordsCount,
                                                 belongsNovelTitle, m_totalCount);
                connect(recycleWork, &RecycledWork::recycledWork_checkStateChanged,
                        this, &RecycleBin::do_recycledWork_checkStateChanged);

                // 添加到列表里
                m_listRecycledWorks << recycleWork;
                m_totalCount++;

            }
        }

        // 准备 查询 分卷
        // 准备 查询小说 数据库语句
        appDB_query.prepare("SELECT volume_id, volume_title, deleted_time FROM volumes WHERE is_deleted = 1 ORDER BY deleted_time ASC;");

        if(appDB_query.exec()) // 数据库语句 运行成功
        {
            // 获取 所属小说和字数
            QSqlQuery minor_query;	//执行操作类对象

            while(appDB_query.next()) // 逐条获取 数据库信息
            {
                workID = appDB_query.value("volume_id").toString();
                workTitle = appDB_query.value("volume_title").toString();
                deletedTimeStr = appDB_query.value("deleted_time").toString();

                minor_query.prepare("SELECT novel_title FROM novels WHERE novel_id = (SELECT novel_id FROM volumes WHERE volume_id = :volumeID);");
                minor_query.bindValue(":volumeID", workID);
                minor_query.exec();
                if(minor_query.first()) // 数据库语句 运行成功
                {
                    belongsNovelTitle = minor_query.value("novel_title").toString();
                }

                minor_query.prepare("SELECT SUM(words_count) AS volumeWordsCount FROM chapters WHERE volume_id = :volumeID;");
                minor_query.bindValue(":volumeID", workID);
                minor_query.exec();
                if(minor_query.first()) // 数据库语句 运行成功
                {
                    wordsCount = minor_query.value("volumeWordsCount").toInt();
                }

                // 计算剩余时间
                deletedTime = QDateTime::fromString(deletedTimeStr, "yyyy-MM-dd hh:mm:ss");
                deadlineTime = deletedTime.addDays(10); // 10 天后彻底删除

                startSec = currentTime.toMSecsSinceEpoch()/1000;
                endSec = deadlineTime.toMSecsSinceEpoch()/1000;

                dayCount = (endSec - startSec) / DAY_SECOND;

                // 新建 小说控件，设置信息
                RecycledWork *recycleWork = new RecycledWork(m_frmScrollAreaContainer);
                // 设置全部信息
                recycleWork->setRecycledWorkInfo(workID, workTitle,
                                                 deletedTimeStr, QString("%1天").arg(dayCount),
                                                 RecycledWorkType::Volume, wordsCount,
                                                 belongsNovelTitle, m_totalCount);
                connect(recycleWork, &RecycledWork::recycledWork_checkStateChanged,
                        this, &RecycleBin::do_recycledWork_checkStateChanged);

                // 添加到列表里
                m_listRecycledWorks << recycleWork;
                m_totalCount++;

            }

            // 结束次要语句
            minor_query.finish();
        }

        // 准备 查询 章节
        // 准备 查询小说 数据库语句
        appDB_query.prepare("SELECT chapter_id, chapter_title, deleted_time, words_count FROM chapters WHERE is_deleted = 1 ORDER BY deleted_time ASC;");

        if(appDB_query.exec()) // 数据库语句 运行成功
        {
            // 获取 所属小说和字数
            QSqlQuery minor_query;	//执行操作类对象

            while(appDB_query.next()) // 逐条获取 数据库信息
            {
                workID = appDB_query.value("chapter_id").toString();
                workTitle = appDB_query.value("chapter_title").toString();
                deletedTimeStr = appDB_query.value("deleted_time").toString();
                wordsCount = appDB_query.value("words_count").toInt();

                belongsNovelTitle = workTitle;

                minor_query.prepare("SELECT novel_title FROM novels WHERE novel_id = (SELECT novel_id FROM chapters WHERE chapter_id = :chapterID);");
                minor_query.bindValue(":chapterID", workID);

                minor_query.exec();
                if(minor_query.first()) // 数据库语句 运行成功
                {
                    belongsNovelTitle = minor_query.value("novel_title").toString();
                }

                // 计算剩余时间
                deletedTime = QDateTime::fromString(deletedTimeStr, "yyyy-MM-dd hh:mm:ss");
                deadlineTime = deletedTime.addDays(10); // 10 天后彻底删除

                startSec = currentTime.toMSecsSinceEpoch()/1000;
                endSec = deadlineTime.toMSecsSinceEpoch()/1000;

                dayCount = (endSec - startSec) / DAY_SECOND;

                // 新建 小说控件，设置信息
                RecycledWork *recycleWork = new RecycledWork(m_frmScrollAreaContainer);
                // 设置全部信息
                if(workID.at(0) == QChar('c')) // 属于章节
                {
                    recycleWork->setRecycledWorkInfo(workID, workTitle,
                                                     deletedTimeStr, QString("%1天").arg(dayCount),
                                                     RecycledWorkType::Chapter, wordsCount,
                                                     belongsNovelTitle, m_totalCount);
                }
                else if(workID.at(0) == QChar('r')) // 属于作品相关
                {
                    recycleWork->setRecycledWorkInfo(workID, workTitle,
                                                     deletedTimeStr, QString("%1天").arg(dayCount),
                                                     RecycledWorkType::WorkRelated, wordsCount,
                                                     belongsNovelTitle, m_totalCount);
                }
                connect(recycleWork, &RecycledWork::recycledWork_checkStateChanged,
                        this, &RecycleBin::do_recycledWork_checkStateChanged);

                // 添加到列表里
                m_listRecycledWorks << recycleWork;
                m_totalCount++;
            }

            // 结束次要语句
            minor_query.finish();
        }

    }
    else // 打开数据库 失败
    {
        QMessageBox::warning(this, "错误", "打开数据库失败");
    }
    // 关闭 数据库
    AppDatabase::quit();


    // 发送 总数改变信号
    emit totalCount_changed(m_totalCount);
}

// 设置 和 获取 背景颜色
void RecycleBin::setBackgroundColor(const QColor &backgroundColor)
{
    m_backgroundColor = backgroundColor;
    update();
}
QColor RecycleBin::getBackgroundColor() const
{
    return m_backgroundColor;
}

// 获取总数
int RecycleBin::getTotalCount() const
{
    return m_totalCount;
}
// 更新回收站
void RecycleBin::updateRecycleBin()
{
    // 移除所有 回收项
    for(int i=0; i<m_listRecycledWorks.count(); i++)
    {
        RecycledWork *recycleWork = m_listRecycledWorks.at(0); // 每次获取第一个

        // 移除
        m_containerLayout->removeWidget(recycleWork);
        m_listRecycledWorks.removeAt(0); // 每次移除第一个

        if(recycleWork)
        {
            delete recycleWork;
            recycleWork = nullptr;
        }
    }
    // 移除弹簧
    m_containerLayout->removeItem(m_spacer);

    // 重新读取数据库
    readDataBase();

    // 重新添加到布局
    // 添加 回收项
    foreach (RecycledWork *recycleWork, m_listRecycledWorks) {
        m_containerLayout->addWidget(recycleWork);
    }
    // 添加弹簧
    m_containerLayout->addSpacerItem(m_spacer);

    // 更新
    update();
}

// 绘制背景
void RecycleBin::drawBackground(QPainter *painter)
{
    // 保存 画家类对象 当前坐标状态
    painter->save();
    painter->setPen(Qt::NoPen); // 边框 不设置 画笔

    painter->setBrush(QBrush(m_backgroundColor)); // 设置 画刷

    painter->drawRect(QRect(0,0,this->width(),this->height())); // 绘制 控件尺寸 的矩形

    // 恢复 画家类对象 上一次保存的坐标状态
    painter->restore();
}

// 重写 绘画事件，绘制 背景
void RecycleBin::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event); // 不使用 事件
    //绘制准备工作
    QPainter painter(this); // 画家类 对象
    painter.setRenderHint(QPainter::Antialiasing); // 设置 抗锯齿
    //m_pixelRatio = painter.device()->devicePixelRatioF(); // 获取不同显示器的分辨率比例，防止不同分辨率下图片失真

    drawBackground(&painter); // 绘制 背景
}

// 标题栏 复选框状态改变 处理槽函数
void RecycleBin::do_recycleTitleBar_checkStateChanged(bool checked)
{
    foreach (RecycledWork *recycleWork, m_listRecycledWorks) {
        recycleWork->setChecked(checked);
    }

    if(checked) // 全选
        emit selectedCount_changed(m_totalCount);
    else // 全不选
        emit selectedCount_changed(0);
}
// 回收项 复选框状态改变 处理槽函数
void RecycleBin::do_recycledWork_checkStateChanged(bool checked)
{
    Qt::CheckState checkState = checked? Qt::Checked:Qt::Unchecked;

    int selectedCount = 0;
    foreach (RecycledWork *recycleWork, m_listRecycledWorks) {
        if(recycleWork->getChecked() != checked) // 存在不同的状态，设为半选
        {
            checkState = Qt::PartiallyChecked;
        }

        if(recycleWork->getChecked() == true) // 计算 被选中的数量
            selectedCount++;
    }

    // 设置 标题栏复选框的状态
    m_recycleBinTitleBar->setCheckState(checkState);

    // 发送 选中的数量
    emit selectedCount_changed(selectedCount);
}

// 彻底章节、作品相关
void RecycleBin::chapter_clear(QString chapterID)
{
    // 获取 所有备份文件的ID
    QSqlQuery chapter_query;	//执行操作类对象

    // 准备 查询备份文件ID
    chapter_query.prepare("SELECT version_id FROM backup_chapters WHERE chapter_id = :chapterID;");

    // 绑定数据
    chapter_query.bindValue("chapterID", chapterID);

    if(chapter_query.exec()) // 数据库语句 运行成功
    {
        while(chapter_query.next()) // 逐条获取 备份文件ID
        {
            // 1、获取 备份文件ID 和 文件路径
            QString versionID = chapter_query.value("version_id").toString();
            QString backupFile_path = QDir::currentPath() + "/Config/LocalChapters/" + versionID + ".txt";

            // 2、根据备份文件ID 删除txt文件
            if(QFile::exists(backupFile_path)) // 之前存在封面，进行删除
                QFile::moveToTrash(backupFile_path); // 移动到回收站
        }
    }

    // 3、清除数据库的 备份文件信息
    // 准备 查询小说 数据库语句
    chapter_query.prepare("DELETE FROM backup_chapters WHERE chapter_id = :chapterID;");
    // 绑定数据
    chapter_query.bindValue("chapterID", chapterID);
    // 执行SQL语句
    chapter_query.exec();

    // 4、删除 章节主文件
    QString mainFile_path = QDir::currentPath() + "/Config/LocalChapters/" + chapterID + ".txt";
    if(QFile::exists(mainFile_path)) // 之前存在封面，进行删除
        QFile::moveToTrash(mainFile_path); // 移动到回收站

    // 5、清除数据库的 主文件信息
    // 准备 查询小说 数据库语句
    chapter_query.prepare("DELETE FROM chapters WHERE chapter_id = :chapterID;");
    // 绑定数据
    chapter_query.bindValue(":chapterID", chapterID);
    // 执行SQL语句
    chapter_query.exec();

    // 结束 语句
    chapter_query.finish();
}
// 彻底清空分卷
void RecycleBin::volume_clear(QString volumeID)
{
    // 获取 所有章节文件的ID
    QSqlQuery volume_query;	//执行操作类对象

    // 准备 查询章节ID
    volume_query.prepare("SELECT chapter_id FROM chapters WHERE volume_id = :volumeID;");

    // 绑定数据
    volume_query.bindValue(":volumeID", volumeID);

    if(volume_query.exec()) // 数据库语句 运行成功
    {
        while(volume_query.next()) // 逐条获取 章节文件ID
        {
            // 1、获取 章节ID
            QString chapterID = volume_query.value("chapter_id").toString();
            // 2、删除数据库章节信息、备份文件信息、删除章节文件、备份文件
            chapter_clear(chapterID);
        }
    }

    // 3、清除数据库的 分卷信息
    // 准备 查询小说 数据库语句
    volume_query.prepare("DELETE FROM volumes WHERE volume_id = :volumeID;");
    // 绑定数据
    volume_query.bindValue(":volumeID", volumeID);
    // 执行SQL语句
    volume_query.exec();

    // 结束 语句
    volume_query.finish();
}
// 彻底清空小说
void RecycleBin::novel_clear(QString novelID)
{
    // 获取 所有分卷的ID
    QSqlQuery novel_query;	//执行操作类对象

    // 准备 查询分卷ID
    novel_query.prepare("SELECT volume_id FROM volumes WHERE novel_id = :novelID;");

    // 绑定数据
    novel_query.bindValue(":novelID", novelID);

    if(novel_query.exec()) // 数据库语句 运行成功
    {
        while(novel_query.next()) // 逐条获取 分卷ID
        {
            // 1、获取 章节ID
            QString volumeID = novel_query.value("volume_id").toString();
            // 2、删除数据库分卷信息
            volume_clear(volumeID);
        }
    }
    // 3、清除数据库的 分卷信息
    // 准备 查询小说 数据库语句
    novel_query.prepare("DELETE FROM novels WHERE novel_id = :novelID;");
    // 绑定数据
    novel_query.bindValue(":novelID", novelID);
    // 执行SQL语句
    novel_query.exec();
    // 结束 语句
    novel_query.finish();

    // 4、删除封面
    // 获取 封面路径
    QString novelCover_path = QDir::currentPath() + "/Config/NovelCover/"+ novelID + ".png";

    // 根据封面路径删除
    if(QFile::exists(novelCover_path)) // 之前存在封面，进行删除
        QFile::moveToTrash(novelCover_path); // 移动到回收站
}
// 检查到期
void RecycleBin::checkRecycledWorksExpiration()
{
    // 确认删除
    if(AppDatabase::getInstance()->openDatabase())
    {
        QString workID;
        foreach (RecycledWork *recycleWork, m_listRecycledWorks) {
            if(recycleWork->getRemaningTime() == "0天")
            {
                // 清空数据库和文件
                workID = recycleWork->getWorkID(); // 获取回收项的ID
                switch (recycleWork->getWorkType()) { // 根据类型 进行处理
                case RecycledWorkType::Novel: // 清除小说
                    novel_clear(workID);
                    break;
                case RecycledWorkType::Volume: // 清除分卷
                    volume_clear(workID);
                    break;
                case RecycledWorkType::Chapter: // 清除章节
                case RecycledWorkType::WorkRelated: // 清除作品相关
                    chapter_clear(workID);
                    break;
                default:
                    break;
                }

                // 清除控件
                if(recycleWork)
                {
                    // 先移除布局和列表
                    m_containerLayout->removeWidget(recycleWork);
                    m_listRecycledWorks.removeOne(recycleWork);
                    // 释放内存
                    delete recycleWork;
                    recycleWork = nullptr;
                }
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
    // 回收项总数改变
    m_recycleBinTitleBar->setCheckState(Qt::Unchecked);
    m_totalCount = m_listRecycledWorks.count();
    emit selectedCount_changed(0);
    emit totalCount_changed(m_totalCount);
}

// 恢复 小说、分卷、章节、作品相关
void RecycleBin::novel_restore(QString novelID)
{
    QSqlQuery appDB_query;	//执行操作类对象

    // 准备 查询备份文件ID
    appDB_query.prepare("UPDATE novels SET is_deleted = 0, deleted_time = :deletedTime WHERE novel_id = :novelID;");
    // 绑定数据
    appDB_query.bindValue(":deletedTime", "");
    appDB_query.bindValue(":novelID", novelID);
    // 执行语句
    appDB_query.exec();

    // 结束 语句
    appDB_query.finish();
}

void RecycleBin::volume_restore(QString volumeID)
{
    QString novelID = "";
    int wordsCount_volume = 0;
    int wordsCount_novel = 0;
    int volumesCount = 0;
    int chaptersCount_volume =0;
    int chaptersCount_novel = 0;

    QSqlQuery appDB_query;	//执行操作类对象

    // 1、恢复数据库
    // 准备 查询备份文件ID
    appDB_query.prepare("UPDATE volumes SET is_deleted = 0, deleted_time = :deletedTime WHERE volume_id = :volumeID;");
    // 绑定数据
    appDB_query.bindValue(":deletedTime", "");
    appDB_query.bindValue(":volumeID", volumeID);
    // 执行语句
    appDB_query.exec();

    // 2、获取 分卷所属小说ID、章节数
    appDB_query.prepare("SELECT novel_id, chapters_count FROM volumes WHERE volume_id = :volumeID");
    // 绑定数据
    appDB_query.bindValue(":volumeID", volumeID);
    // 执行语句
    appDB_query.exec();

    // 获取 小说ID、分卷下章节数、字数
    if(appDB_query.first()) // 数据库语句 运行成功
    {
        novelID = appDB_query.value("novel_id").toString();
        chaptersCount_volume = appDB_query.value("chapters_count").toInt();
    }


    // 3、获取 分卷的总字数
    appDB_query.prepare("SELECT SUM(words_count) as volumeWordCount FROM chapters WHERE volume_id = :volumeID;");
    // 绑定数据
    appDB_query.bindValue(":volumeID", volumeID);
    // 执行语句
    if(appDB_query.exec())
    {
        qDebug()<<appDB_query.lastError();
    }

    // 获取 小说ID、分卷下章节数、字数
    if(appDB_query.first()) // 数据库语句 运行成功
    {
        wordsCount_volume = appDB_query.value("volumeWordCount").toInt();
    }

    // 4、获取 小说当前 分卷数、章节数、字数
    appDB_query.prepare("SELECT volumes_count, chapters_count, words_count FROM novels WHERE  novel_id= :novelID;");
    // 绑定数据
    appDB_query.bindValue(":novelID", novelID);
    // 执行语句
    if(appDB_query.exec())
    {
        qDebug()<<appDB_query.lastError();
    }

    // 获取 小说ID、章节字数
    if(appDB_query.first()) // 数据库语句 运行成功
    {
        volumesCount = appDB_query.value("volumes_count").toInt();
        chaptersCount_novel = appDB_query.value("chapters_count").toInt();
        wordsCount_novel = appDB_query.value("words_count").toInt();
    }

    // 5、更新 novels小说表 中的分卷数 章节数、字数
    appDB_query.prepare("UPDATE novels SET volumes_count = :volumesCount, chapters_count = :chaptersCount, words_count = :wordsCount WHERE novel_id = :novelID;");
    // 绑定数据
    appDB_query.bindValue(":volumesCount", volumesCount + 1);
    appDB_query.bindValue(":chaptersCount", chaptersCount_novel + chaptersCount_volume);
    appDB_query.bindValue(":wordsCount", wordsCount_novel + wordsCount_volume);
    appDB_query.bindValue(":novelID", novelID);

    // 执行语句
    appDB_query.exec();

    // 结束 语句
    appDB_query.finish();
}

void RecycleBin::chapter_restore(QString chapterID)
{
    QString novelID = "";
    int wordCount_chapter = 0;
    int wordCount_novel = 0;
    int chaptersCount = 0;

    QSqlQuery appDB_query;	//执行操作类对象

    // 1、恢复数据库
    appDB_query.prepare("UPDATE chapters SET is_deleted = 0, deleted_time = :deletedTime WHERE chapter_id = :chapterID;");
    // 绑定数据
    appDB_query.bindValue(":deletedTime", "");
    appDB_query.bindValue(":chapterID", chapterID);
    // 执行语句
    appDB_query.exec();

    // 2、获取 章节字数
    appDB_query.prepare("SELECT novel_id, words_count FROM chapters WHERE chapter_id = :chapterID;");
    // 绑定数据
    appDB_query.bindValue(":chapterID", chapterID);
    // 执行语句
    appDB_query.exec();

    // 获取 小说ID、章节字数
    if(appDB_query.first()) // 数据库语句 运行成功
    {
        novelID = appDB_query.value("novel_id").toString();
        wordCount_chapter = appDB_query.value("words_count").toInt();
    }

    // 3、获取 小说章节数、字数
    appDB_query.prepare("SELECT chapters_count, words_count FROM novels WHERE novel_id = :novelID;");
    // 绑定数据
    appDB_query.bindValue(":novelID", novelID);
    // 执行语句
    appDB_query.exec();

    // 获取 小说章节数、字数
    if(appDB_query.first()) // 数据库语句 运行成功
    {
        chaptersCount = appDB_query.value("chapters_count").toInt();
        wordCount_novel = appDB_query.value("words_count").toInt();
    }

    // 4、更新 novels小说表 中的章节数、字数
    appDB_query.prepare("UPDATE novels SET chapters_count = :chaptersCount, words_count = :wordsCount WHERE novel_id = :novelID;");
    // 绑定数据
    appDB_query.bindValue(":chaptersCount", chaptersCount + 1);
    appDB_query.bindValue(":wordsCount", wordCount_novel + wordCount_chapter);
    appDB_query.bindValue(":novelID", novelID);
    // 执行语句
    appDB_query.exec();

    // 结束 语句
    appDB_query.finish();
}

void RecycleBin::workRelated_restore(QString workRelatedID)
{
    QSqlQuery appDB_query;	//执行操作类对象

    // 1、恢复数据库
    appDB_query.prepare("UPDATE chapters SET is_deleted = 0, deleted_time = :deletedTime WHERE chapter_id = :chapterID;");
    // 绑定数据
    appDB_query.bindValue(":deletedTime", "");
    appDB_query.bindValue(":chapterID", workRelatedID);
    // 执行语句
    appDB_query.exec();

    // 结束 语句
    appDB_query.finish();
}

// 恢复 回收项
void RecycleBin::do_recycleBin_restore()
{
    if(AppDatabase::getInstance()->openDatabase())
    {
        QString workID;

        foreach (RecycledWork *recycleWork, m_listRecycledWorks) {
            if(recycleWork->getChecked()) // 回收项被选中
            {
                // 清空数据库和文件
                workID = recycleWork->getWorkID(); // 获取回收项的ID
                switch (recycleWork->getWorkType()) { // 根据类型 进行处理
                case RecycledWorkType::Novel: // 恢复小说
                    novel_restore(workID);
                    break;
                case RecycledWorkType::Volume: // 恢复分卷
                    volume_restore(workID);
                    break;
                case RecycledWorkType::Chapter: // 恢复章节
                    chapter_restore(workID);
                    break;
                case RecycledWorkType::WorkRelated: // 恢复作品相关
                    workRelated_restore(workID);
                    break;
                default:
                    break;
                }
            }
            // 清除控件
            if(recycleWork)
            {
                // 先移除布局和列表
                m_containerLayout->removeWidget(recycleWork);
                m_listRecycledWorks.removeOne(recycleWork);
                // 释放内存
                delete recycleWork;
                recycleWork = nullptr;
            }
        }
    }
    else // 打开数据库 失败
    {
        QMessageBox::warning(this, "错误", "打开数据库失败");
    }
    // 关闭 数据库
    AppDatabase::quit();


    // 回收项总数改变
    m_recycleBinTitleBar->setCheckState(Qt::Unchecked);
    m_totalCount = m_listRecycledWorks.count();
    emit selectedCount_changed(0);
    emit totalCount_changed(m_totalCount);
    emit recycledWork_restored();

    // 更新
    update();
}
// 彻底删除 回收项
void RecycleBin::do_recycleBin_clear()
{
    // 确认删除
    if(AppDatabase::getInstance()->openDatabase())
    {
        QString workID;
        foreach (RecycledWork *recycleWork, m_listRecycledWorks) {
            if(recycleWork->getChecked()) // 回收项被选中
            {
                // 清空数据库和文件
                workID = recycleWork->getWorkID(); // 获取回收项的ID
                switch (recycleWork->getWorkType()) { // 根据类型 进行处理
                case RecycledWorkType::Novel: // 清除小说
                    novel_clear(workID);
                    break;
                case RecycledWorkType::Volume: // 清除分卷
                    volume_clear(workID);
                    break;
                case RecycledWorkType::Chapter: // 清除章节
                case RecycledWorkType::WorkRelated: // 清除作品相关
                    chapter_clear(workID);
                    break;
                default:
                    break;
                }

                // 清除控件
                if(recycleWork)
                {
                    // 先移除布局和列表
                    m_containerLayout->removeWidget(recycleWork);
                    m_listRecycledWorks.removeOne(recycleWork);
                    // 释放内存
                    delete recycleWork;
                    recycleWork = nullptr;
                }
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
    // 回收项总数改变
    m_recycleBinTitleBar->setCheckState(Qt::Unchecked);
    m_totalCount = m_listRecycledWorks.count();
    emit selectedCount_changed(0);
    emit totalCount_changed(m_totalCount);
}
// 清空 回收站
void RecycleBin::do_recycleBin_clearAll()
{
    // 确认删除
    if(AppDatabase::getInstance()->openDatabase())
    {
        QString workID;
        foreach (RecycledWork *recycleWork, m_listRecycledWorks) {
            // 清空数据库和文件
            workID = recycleWork->getWorkID(); // 获取回收项的ID
            switch (recycleWork->getWorkType()) { // 根据类型 进行处理
            case RecycledWorkType::Novel: // 清除小说
                novel_clear(workID);
                break;
            case RecycledWorkType::Volume: // 清除分卷
                volume_clear(workID);
                break;
            case RecycledWorkType::Chapter: // 清除章节
            case RecycledWorkType::WorkRelated: // 清除作品相关
                chapter_clear(workID);
                break;
            default:
                break;
            }

            // 清除控件
            if(recycleWork)
            {
                // 先移除布局和列表
                m_containerLayout->removeWidget(recycleWork);
                m_listRecycledWorks.removeOne(recycleWork);
                // 释放内存
                delete recycleWork;
                recycleWork = nullptr;
            }
        }
        // 清空列表
        m_listRecycledWorks.clear();
    }
    else // 打开数据库 失败
    {
        QMessageBox::warning(this, "错误", "打开数据库失败");
    }
    // 关闭 数据库
    AppDatabase::quit();

    // 更新
    update();
    m_recycleBinTitleBar->setCheckState(Qt::Unchecked);
    // 回收项总数改变
    m_totalCount = 0;
    emit selectedCount_changed(0);
    emit totalCount_changed(0);
}


