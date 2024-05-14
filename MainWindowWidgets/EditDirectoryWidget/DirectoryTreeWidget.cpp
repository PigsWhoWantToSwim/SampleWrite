#include "DirectoryTreeWidget.h"

DirectoryTreeWidget::DirectoryTreeWidget(QString novel, QWidget *parent)
    : BasedDirectoryWidget{parent}
{
    // 设置 小说ID
    m_novelID = novel;

    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}
DirectoryTreeWidget::~DirectoryTreeWidget()
{
    // 在销毁前更新保存
    updateItemData();
    updateDateBase();
}

void DirectoryTreeWidget::initMemberVariable()
{
    // 默认升序
    m_order = Qt::AscendingOrder;
}

void DirectoryTreeWidget::initLayout()
{
    // 读取数据库
    readDataBase();

    // 更新 列宽
    this->updateDirectoryColumnWidth();

    // 展开节点
    this->expandAll();
}

void DirectoryTreeWidget::initConnect()
{
    // 绑定 右键弹出菜单
    connect(this, &DirectoryTreeWidget::customContextMenuRequested,
            this, &DirectoryTreeWidget::do_directoryTree_MenuRequested);

    // 绑定 项双击信号，发送章ID槽函数
    connect(this,&DirectoryTreeWidget::itemDoubleClicked,this,[=](QTreeWidgetItem *item, int column){
        Q_UNUSED(column);
        if(item->type() == int(DirectoryItemType::ChapterItem)) // 如果是 章节点
        {
            // 发送 章ID，章标题
            QString ID = item->data(int(DirectoryDateColume::IDColume),Qt::UserRole).toString();
            QString title = item->text(int(DirectoryTextColume::TitleColume));
            m_chapterIDSent = ID;
            emit chapterItem_DoubleClicked(ID, title);
        }
    });
}

// 读取数据库数据
void DirectoryTreeWidget::readDataBase()
{
    if(AppDatabase::getInstance()->openDatabase()) // 打开数据库成功
    {
        // 查询volumes表中 属于novel_id的所有volume记录
        QSqlQuery volumes_sql;	//执行操作类对象
        // 准备sql语句
        volumes_sql.prepare("SELECT * FROM volumes WHERE novel_id = :novelID AND is_deleted = 0 ORDER BY volume_index ASC;");
        volumes_sql.bindValue(":novelID", m_novelID);
        // 执行sql语句
        if(volumes_sql.exec()) // 执行成功
        {
            // 读取每一条 volumes记录
            while(volumes_sql.next())
            {
                // 获取信息
                QString volumeID = volumes_sql.value("volume_id").toString();
                QString volumeTitle = volumes_sql.value("volume_title").toString();
                int chaptersCount = volumes_sql.value("chapters_count").toInt();
                int volumeIndex = volumes_sql.value("volume_index").toInt();

                // 创建一行项目 为卷节点
                QTreeWidgetItem* volumeItem = new QTreeWidgetItem(int(DirectoryItemType::VolumeItem));

                // 设置数据 idCol列，第1个数据，为id。indexCol列，第1个数据，为索引
                volumeItem->setData(int(DirectoryDateColume::IDColume),Qt::UserRole,volumeID);
                volumeItem->setData(int(DirectoryDateColume::IndexColume),Qt::UserRole,volumeIndex);

                // 设置文本显示内容 titleCol列 为卷标题，numberCol列 为章节数
                volumeItem->setText(int(DirectoryTextColume::TitleColume),volumeTitle);
                volumeItem->setText(int(DirectoryTextColume::CountColume),QString::number(chaptersCount)+"章");

                // 设置 光标悬停显示
                volumeItem->setToolTip(int(DirectoryTextColume::TitleColume),volumeTitle); // 显示 卷标题

                // 设置 数量列 右对齐
                volumeItem->setTextAlignment(int(DirectoryTextColume::CountColume),Qt::AlignRight);

                // 添加成为 顶层节点
                this->addTopLevelItem(volumeItem);

                // 卷下 是否存在 章节
                bool isExistChapter = false;

                // 在行下创建子项目
                // 准备sql语句
                QSqlQuery chapters_sql;	//执行操作类对象
                // 准备 sql 语句
                chapters_sql.prepare("SELECT * FROM chapters WHERE volume_id = :volumeID AND is_deleted = 0 ORDER BY chapter_index ASC"); // 使用排序
                chapters_sql.bindValue(":volumeID",volumeID); // 绑定值
                // 执行sql语句
                if(chapters_sql.exec())
                {
                    // 读取每一条 chapter记录
                    while(chapters_sql.next())
                    {
                        // 获取信息
                        QString chapterID = chapters_sql.value("chapter_id").toString();
                        QString chapterTitle = chapters_sql.value("chapter_title").toString();
                        int wordsCount = chapters_sql.value("words_count").toInt();
                        int chapterIndex= chapters_sql.value("chapter_index").toInt();

                        // 创建一行项目 为章节点
                        QTreeWidgetItem* chapterItem=new QTreeWidgetItem(int(DirectoryItemType::ChapterItem));

                        // titleCol列，第1个数据，为id。indexCol列，第1个数据，为索引
                        chapterItem->setData(int(DirectoryDateColume::IDColume),Qt::UserRole,chapterID);
                        chapterItem->setData(int(DirectoryDateColume::ParentIDColume),Qt::UserRole,volumeID);
                        chapterItem->setData(int(DirectoryDateColume::IndexColume),Qt::UserRole,chapterIndex);


                        // 设置文本显示内容 titleCol列 为章标题，numberCol列 为字数
                        chapterItem->setText(int(DirectoryTextColume::TitleColume),chapterTitle);
                        chapterItem->setText(int(DirectoryTextColume::CountColume),QString::number(wordsCount)+"字");

                        // 设置 光标悬停提示
                        chapterItem->setToolTip(int(DirectoryTextColume::TitleColume),chapterTitle); // 显示 章标题

                        // 设置图标
                        // chapterItem->setIcon(int(DirectoryTextColume::TitleColume),QIcon(""));

                        // 设置 数量列 右对齐
                        chapterItem->setTextAlignment(int(DirectoryTextColume::CountColume),Qt::AlignRight);

                        // 添加到父项目 卷
                        volumeItem->addChild(chapterItem);

                        isExistChapter = true;
                    }
                }
                else // 执行 失败
                {

                }
                // 结束 章sql语句
                chapters_sql.finish();

                // 设置 卷节点 图标
                // QString volumeIconPath = isExistChapter? "":"";
                // volumeItem->setIcon(int(DirectoryTextColume::TitleColume),QIcon(volumeIconPath));
            }

            // 结束 卷sql语句
            volumes_sql.finish();
        }
        else
        {
            QMessageBox::warning(this, "错误", "打开数据库失败");

            AppDatabase::getInstance()->quit();
            return;
        }
    }
    else // 打开数据库 失败
    {
        QMessageBox::warning(this, "错误", "打开数据库失败");
    }

    // 关闭数据库
    AppDatabase::getInstance()->quit();
}
// 更新数据库数据
void DirectoryTreeWidget::updateDateBase()
{
    // 打开数据库
    if (AppDatabase::getInstance()->openDatabase()) // 数据库打开成功
    {
        // 获取 总卷数
        int totalVolumesCount = this->topLevelItemCount();
        // 获取 总章数
        int totalChaptersCount = 0;
        // 获取 总字数
        int totalWordsCount = 0;

        // 查询volumes表中 属于novel_id的所有volume记录
        QSqlQuery novels_sql;	// 更新volumes表 执行操作类对象
        QSqlQuery volumes_sql;	// 更新volumes表 执行操作类对象
        QSqlQuery chapters_sql;	// 更新chapters表 执行操作类对象

        // 遍历 每一个 卷节点
        for(int realVolumeIndex = 0; realVolumeIndex < totalVolumesCount; realVolumeIndex++)
        {
            // 获取卷节点 存储的数据
            // 卷ID 用于查询，更新 卷标题，卷索引，章节数
            QString volumeID = topLevelItem(realVolumeIndex)->data(int(DirectoryDateColume::IDColume),Qt::UserRole).toString();
            QString realVolumeTitle = topLevelItem(realVolumeIndex)->text(int(DirectoryTextColume::TitleColume));
            int realChaptersCount = topLevelItem(realVolumeIndex)->childCount();

            // 获取 卷节点的 章节点数量
            totalChaptersCount += realChaptersCount;

            // 准备sql语句
            volumes_sql.prepare("UPDATE volumes SET "
                                "volume_title = :realVolumeTitle, "
                                "chapters_count = :realChaptersCount, "
                                "volume_index = :realVolumeIndex "
                                "WHERE volume_id = :VolumeID;");
            // 绑定数据
            volumes_sql.bindValue(":realVolumeTitle", realVolumeTitle);
            volumes_sql.bindValue(":realChaptersCount", realChaptersCount);
            volumes_sql.bindValue(":realVolumeIndex", realVolumeIndex);
            volumes_sql.bindValue(":VolumeID", volumeID);

            // 执行sql语句
            if(!volumes_sql.exec())
                qDebug()<<volumes_sql.lastError(); // 执行 sql语句 失败

            // 遍历 每一个 章节点
            for(int realChapterIndex = 0; realChapterIndex<realChaptersCount; realChapterIndex++)
            {
                // 获取章节点 存储的数据
                // 章ID 用于查询，更新 卷标题，卷索引，章节数
                QString chapterID = topLevelItem(realVolumeIndex)->child(realChapterIndex)->data(int(DirectoryDateColume::IDColume),Qt::UserRole).toString();
                QString realChapterTitle = topLevelItem(realVolumeIndex)->child(realChapterIndex)->text(int(DirectoryTextColume::TitleColume));
                // 获取顶层节点 存储的 字数 数据
                QString storedWordCountString = topLevelItem(realVolumeIndex)->child(realChapterIndex)->text(int(DirectoryTextColume::CountColume)); // 获取 字符串
                storedWordCountString.chop(1); // 去除 “字”字
                int realWordsCount = storedWordCountString.toInt(); // 转为 整型

                // 获取 卷节点的 章节点数量
                totalWordsCount += realWordsCount;

                // 准备sql语句
                chapters_sql.prepare("UPDATE chapters SET "
                                     "chapter_title = :realChapterTitle, "
                                     "volume_id = :volumeID, "
                                     "words_count = :realWordsCount, "
                                     "chapter_index = :realChapterIndex "
                                     "WHERE chapter_id = :chapterID;");
                // 绑定 数据
                chapters_sql.bindValue(":realChapterTitle", realChapterTitle);
                chapters_sql.bindValue(":volumeID", volumeID);
                chapters_sql.bindValue(":realWordsCount", realWordsCount);
                chapters_sql.bindValue(":realChapterIndex", realChapterIndex);
                chapters_sql.bindValue(":chapterID", chapterID);

                // 执行sql语句
                if(!chapters_sql.exec())
                    qDebug()<<chapters_sql.lastError(); // 执行 sql语句 失败
            }
        }

        // 更新 小说表
        // 准备sql语句
        novels_sql.prepare("UPDATE novels SET volumes_count = :realVolumesCount, chapters_count = :realChaptersCount, words_count = :realWordsCount WHERE novel_id = :novelID");

        // 绑定数据
        novels_sql.bindValue(":realVolumesCount", totalVolumesCount);
        novels_sql.bindValue(":realChaptersCount", totalChaptersCount);
        novels_sql.bindValue(":realWordsCount", totalWordsCount);
        novels_sql.bindValue(":novelID", m_novelID);

        // 执行sql语句
        if(!novels_sql.exec())
            qDebug()<<novels_sql.lastError(); // 执行 sql语句 失败

        // 结束 chapters_sql语句
        chapters_sql.finish();
        // 结束 volumes_sql语句
        volumes_sql.finish();
        // 结束 novels_sql语句
        novels_sql.finish();
    }
    else // 数据库 打开 失败
        QMessageBox::warning(this, "错误", "打开数据库失败");

    // 关闭数据库
    AppDatabase::getInstance()->quit();
}
// 更新节点数据
void DirectoryTreeWidget::updateItemData()
{
    // 遍历 所有 卷节点
    for(int realVolumeIndex=0; realVolumeIndex<this->topLevelItemCount();realVolumeIndex++)
    {
        // 获取卷节点 存储的 卷ID，索引，章节数 数据
        QString realVolumeID = topLevelItem(realVolumeIndex)->data(int(DirectoryDateColume::IDColume),Qt::UserRole).toString();
        int storedVolumeIndex = topLevelItem(realVolumeIndex)->data(int(DirectoryDateColume::IDColume),Qt::UserRole).toInt();

        QString storedChaptesCountStr = topLevelItem(realVolumeIndex)->text(int(DirectoryTextColume::CountColume));
        storedChaptesCountStr.chop(1); // 去除 “章”字
        int storedChaptersCount = storedChaptesCountStr.toInt(); // 转为 整型 获得 存储 章节数
        int realChaptersCount = topLevelItem(realVolumeIndex)->childCount();


        // 卷节点 储存的卷索引 与 实际索引 不同，更新卷索引
        if(storedVolumeIndex != realVolumeIndex)
        {
            topLevelItem(realVolumeIndex)->setData(int(DirectoryDateColume::IndexColume), Qt::UserRole, realVolumeIndex);
        }

        // 卷节点 储存的章数 与 实际章数 不同，更新章数
        if(storedChaptersCount != realChaptersCount)
        {
            topLevelItem(realVolumeIndex)->setText(int(DirectoryTextColume::CountColume), QString::number(realChaptersCount)+"章");
        }

        // 遍历 卷下 所有 章节点
        for(int realChapterIndex=0; realChapterIndex < topLevelItem(realVolumeIndex)->childCount(); realChapterIndex++)
        {
            // 获取章节点 存储的 父卷ID，索引数据
            QString storedVolumeID = topLevelItem(realVolumeIndex)->child(realChapterIndex)->data(int(DirectoryDateColume::IDColume), Qt::UserRole).toString();
            int storedChapterIndex = topLevelItem(realVolumeIndex)->child(realChapterIndex)->data(int(DirectoryDateColume::IndexColume), Qt::UserRole).toInt();

            // 章节点存储的父卷 与 实际所在的卷节点 不同，更新存储的父卷
            if(storedVolumeID != realVolumeID)
            {
                topLevelItem(realVolumeIndex)->child(realChapterIndex)->setData(int(DirectoryDateColume::ParentIDColume), Qt::UserRole, realVolumeID);
            }

            // 章节点存储的索引 与 实际所在的索引 不同，更新存储的索引
            if(storedChapterIndex != realChapterIndex)
            {
                topLevelItem(realVolumeIndex)->child(realChapterIndex)->setData(int(DirectoryDateColume::IndexColume), Qt::UserRole, realChapterIndex);
            }
        }
    }
}

// 添加章
void DirectoryTreeWidget::addChapterItem(QTreeWidgetItem *volumeItem)
{
    // 获取 当前 章数量
    int chaptersCount = volumeItem->childCount();

    // 获取 初始 标题
    QString orignalTitelText = "第"+QString::number(chaptersCount+1)+"章"; // 默认 章标题
    QString newTitleText = orignalTitelText; // 用于接收 新标题，默认为 第i章，防止点击取消

    // 是否取消新建 章
    bool isCancelNewChapter = false;

    // 获取 新分卷的标题
    newTitleText = getNewTitleFromDialog("新建章节",orignalTitelText,"请输入章节名",isCancelNewChapter);

    // 确认或取消
    if(isCancelNewChapter) // 取消新建 卷
        return; // 直接结束

    // 确认新建
    if (AppDatabase::getInstance()->openDatabase()) // 数据库打开成功
    {
        // 时间戳
        QDateTime dateTime = QDateTime::currentDateTime(); // 获取 当前时间
        QString timestamp = dateTime.toString("yyMMddhhmmss"); // 字符串格式化
        QString timestamp_birthTime = dateTime.toString("yyyy-MM-dd hh:mm:ss"); // 创建时间

        // 准备信息
        QString ID_encoding = timestamp + getRandomLetterString(2);

        // 准备信息
        QString parentID = volumeItem->data(int(DirectoryDateColume::IDColume),Qt::UserRole).toString();
        QString chapterID = "c" + ID_encoding;
        QString chapterFilePath = QDir::currentPath() + "/Config/LocalChapters/" + chapterID + ".txt";

        // 备份文件版本ID、文件路径
        QString backupVersionID = "b" + ID_encoding;
        QString backupFilePath = QDir::currentPath() + "/Config/LocalChapters/" + backupVersionID + ".txt";

        // ****************新建节点 为章************************
        QTreeWidgetItem *newChapterItem = new QTreeWidgetItem(int(DirectoryItemType::ChapterItem));

        // idCol列，第1个数据，为id。indexCol列，第1个数据，为索引
        newChapterItem->setData(int(DirectoryDateColume::IDColume), Qt::UserRole, chapterID);
        newChapterItem->setData(int(DirectoryDateColume::ParentIDColume), Qt::UserRole, parentID);
        newChapterItem->setData(int(DirectoryDateColume::IndexColume), Qt::UserRole, chaptersCount);

        // 设置文本显示内容
        newChapterItem->setText(int(DirectoryTextColume::TitleColume),newTitleText);
        newChapterItem->setText(int(DirectoryTextColume::CountColume),"0字");

        // 设置 光标悬停提示
        newChapterItem->setToolTip(int(DirectoryTextColume::TitleColume),newTitleText);

        // 设置图标
        // newChapterItem->setIcon(int(DirectoryTextColume::TitleColume),QIcon(":/Image/paper - gray.png"));

        // 设置 数量列 右对齐
        newChapterItem->setTextAlignment(int(DirectoryTextColume::CountColume),Qt::AlignRight);

        // 添加到父项目 卷
        volumeItem->addChild(newChapterItem);

        // 更新显示的 章节数
        chaptersCount = volumeItem->childCount();
        volumeItem->setText(int(DirectoryTextColume::CountColume),QString::number(chaptersCount)+"章");

        // 卷节点 默认 展开
        volumeItem->setExpanded(true);

        // ********************************* 新建 文件 ****************************

        // 新建 主文本文件
        QFile chapterFile(chapterFilePath); // 文件操作对象
        chapterFile.open(QIODevice::WriteOnly|QIODevice::Text); // 创建文件
        chapterFile.close(); // 关闭 文件
        // 新建 备份文件
        QFile backupFile(backupFilePath);
        backupFile.open(QIODevice::WriteOnly|QIODevice::Text);
        backupFile.close();

        // ************************** 保存到数据库 *************************
        // 保存 章 信息
        QSqlQuery chapters_sql;	//执行操作类对象
        // 准备sql语句
        chapters_sql.prepare("INSERT INTO chapters ("
                             "chapter_id, chapter_title, novel_id, volume_id, "
                             "file_pathURL, words_count, chapter_index, is_deleted"
                             ") VALUES ("
                             ":chapterID, :chapterTitel, :novelID, :volumeID, "
                             ":filePathURL, 0, :chapterIndex, 0);");

        // 绑定数据
        chapters_sql.bindValue(":chapterID", chapterID);
        chapters_sql.bindValue(":chapterTitel", newTitleText);
        chapters_sql.bindValue(":novelID", m_novelID);
        chapters_sql.bindValue(":volumeID", parentID);
        chapters_sql.bindValue(":filePathURL", chapterFilePath);
        chapters_sql.bindValue(":chapterIndex", chaptersCount);

        // 执行sql语句
        if(!chapters_sql.exec())
            qDebug()<<chapters_sql.lastError(); // 执行失败


        // 创建 备份文件 保存到数据库
        chapters_sql.prepare("INSERT INTO backup_chapters ("
                             "version_id, chapter_id, birth_time, words_count, file_pathURL"
                             ") VALUES ("
                             ":version_id, :chapterID, :birthTime, 0, :backupFilePath);");

        // 绑定数据
        chapters_sql.bindValue(":version_id", backupVersionID);
        chapters_sql.bindValue(":chapterID", chapterID);
        chapters_sql.bindValue(":birthTime", timestamp_birthTime);
        chapters_sql.bindValue(":backupFilePath", backupFilePath);

        // 执行sql语句
        if(!chapters_sql.exec())
            qDebug()<<chapters_sql.lastError(); // 执行失败

        // 结束sql语句
        chapters_sql.finish();

        // 更新 卷节点 存储的信息
        int chaptersCount = volumeItem->childCount();// 获取 章节数

        // 更新 卷 的章节数
        QSqlQuery volumes_sql;	//执行操作类对象
        // 准备sql语句
        volumes_sql.prepare("UPDATE volumes SET chapters_count = :chaptersCount WHERE volume_id = :volumeID");

        // 绑定数据
        volumes_sql.bindValue(":chaptersCount", chaptersCount);
        volumes_sql.bindValue(":volumeID", parentID);

        // 执行sql语句
        if(!volumes_sql.exec())
            qDebug()<<volumes_sql.lastError(); // 执行失败

        // 结束sql语句
        volumes_sql.finish();
    }
    else // 数据库打开失败
        QMessageBox::warning(this, "错误", "打开数据库失败");

    // 关闭数据库
    AppDatabase::getInstance()->quit();

}
// 添加卷
void DirectoryTreeWidget::addVolumeItem()
{
    // 获取 当前卷数量
    int volumesCount = this->topLevelItemCount();

    // 获取 初始 标题
    QString orignalTitelText = "第"+QString::number(volumesCount+1)+"卷"; // 默认标题
    QString newTitleText = orignalTitelText; // 用于接收 新标题，默认为 第i卷，防止点击取消

    // 是否取消新建 卷
    bool isCancelNewVolume = false;

    // 获取 新分卷的标题
    newTitleText = getNewTitleFromDialog("新建分卷", orignalTitelText,"请输入分卷名", isCancelNewVolume);

    // 确认或取消
    if(isCancelNewVolume) // 取消新建 卷
        return;

    // 点击确认，已获取到新标题
    if (AppDatabase::getInstance()->openDatabase()) // 打开 数据库成功
    {
        // 时间戳
        QDateTime dateTime = QDateTime::currentDateTime(); // 获取 当前时间
        QString timestamp_id = dateTime.toString("yyMMddhhmmss");// 字符串格式化


        // ******************* 新创建一行节点 为卷节点 *************************
        QTreeWidgetItem *newVolumeItem=new QTreeWidgetItem(int(DirectoryItemType::VolumeItem));

        // 准备信息
        QString volumeID = "v" + timestamp_id + getRandomLetterString(2); // 卷ID

        // 保存节点信息
        // idCol列，第1个数据，为id。indexCol列，第1个数据，为索引
        newVolumeItem->setData(int(DirectoryDateColume::IDColume), Qt::UserRole, volumeID);
        newVolumeItem->setData(int(DirectoryDateColume::IndexColume),Qt::UserRole,volumesCount);

        // 设置文本显示内容
        newVolumeItem->setText(int(DirectoryTextColume::TitleColume), newTitleText);
        newVolumeItem->setText(int(DirectoryTextColume::CountColume),"0章");

        // 设置 光标悬停提示
        newVolumeItem->setToolTip(int(DirectoryTextColume::TitleColume), newTitleText);

        // 设置图标
        // newVolumeItem->setIcon(int(DirectoryTextColume::TitleColume),QIcon(":/Image/folder - gray.png"));

        // 设置 数量列 右对齐
        newVolumeItem->setTextAlignment(int(DirectoryTextColume::CountColume),Qt::AlignRight);

        // 设置 节点默认 展开
        newVolumeItem->setExpanded(true);

        // 添加成为 顶层节点
        this->addTopLevelItem(newVolumeItem);

        // ************************** 保存到 数据库 ********************************
        //执行操作类对象
        QSqlQuery volumes_sql;
        // 准备sql语句
        volumes_sql.prepare("INSERT INTO volumes ("
                            "volume_id, volume_title, novel_id, chapters_count, volume_index, is_deleted"
                            ") VALUES ("
                            ":volumeID, :volumeTitle, :novelID, :chaptersCount, :volumeIndex, 0);");

        // 绑定数据
        volumes_sql.bindValue(":volumeID", volumeID);
        volumes_sql.bindValue(":volumeTitle", newTitleText);
        volumes_sql.bindValue(":novelID", m_novelID);
        volumes_sql.bindValue(":chaptersCount", 0);
        volumes_sql.bindValue(":volumeIndex", volumesCount);

        // 执行sql语句
        if(!volumes_sql.exec())
            qDebug()<<volumes_sql.lastError(); // 执行 sql语句 失败

        // 结束sql语句
        volumes_sql.finish();

        // 更新 novel表
        // 在 updateDataBase()函数已实现，关闭时自动更新
    }
    else // 打开 数据库 失败
        QMessageBox::warning(this, "错误", "打开数据库失败");

    // 关闭数据库
    AppDatabase::getInstance()->quit();
}
// 修改
// 移除 章
void DirectoryTreeWidget::romoveChapterItem(QTreeWidgetItem *chapterItem)
{
    // 打开数据库
    if (AppDatabase::getInstance()->openDatabase()) // 数据库打开成功
    {
        // ******************* 更新 chapters表 *****************************
        // 获取时间
        QDateTime dateTime = QDateTime::currentDateTime();
        QString timestamp = dateTime.toString("yyyy-MM-dd hh:mm:ss");// 字符串格式化

        //执行操作类对象
        QSqlQuery chapter_sql;

        // 获取 章ID
        QString chapterID = chapterItem->data(int(DirectoryDateColume::IDColume),Qt::UserRole).toString();

        // 准备sql语句
        chapter_sql.prepare("UPDATE chapters SET is_deleted = 1, deleted_time = :deletedTime WHERE chapter_id = :chapterID");
        // 绑定数据
        chapter_sql.bindValue(":deletedTime", timestamp);
        chapter_sql.bindValue(":chapterID", chapterID);

        // 执行sql语句
        if(!chapter_sql.exec())
            qDebug()<<chapter_sql.lastError(); // 执行 sql语句 失败

        // 结束 sql 语句
        chapter_sql.finish();

        // 移除 子节点
        chapterItem->parent()->removeChild(chapterItem);
        // 手动释放 内容
        delete chapterItem;
        // 指针置空
        chapterItem = nullptr;

        // *************** 更新 volumes表 novels表
        // 在 updateDataBase()函数已实现，关闭时自动更新
    }
    else // 打开数据库失败
        QMessageBox::warning(this, "错误", "打开数据库失败");

    // 关闭数据库
    AppDatabase::getInstance()->quit();

    // 目录项 已删除
    emit directioryItem_deleted();
}
// 移除 卷
void DirectoryTreeWidget::removeVolumeItem(QTreeWidgetItem *volumeItem)
{
    // 获取 该卷节点 下的 章节点数量
    int childCount = volumeItem->childCount();

    if(childCount > 0) // 存在 卷下存在章节
    {
        // 有子节点，循环删除
        for(int i=0; i<childCount; i++)
        {
            // 获取要删除的 子节点，每次删除 第1个
            QTreeWidgetItem *childItem = volumeItem->child(0);
            // 移除 章节点
            volumeItem->removeChild(childItem);

            if(childItem)
            {
                delete childItem; // 释放内存
                childItem = nullptr;
            }
        }
    }


    if (AppDatabase::getInstance()->openDatabase()) // 数据库打开成功
    {
        // ***************** 更新 volumes表 **************************

        // 获取时间
        QDateTime dateTime = QDateTime::currentDateTime();
        QString timestamp = dateTime.toString("yyyy-MM-dd hh:mm:ss");// 字符串格式化

        // 获取 卷ID
        QString volumeID = volumeItem->data(int(DirectoryDateColume::IDColume),Qt::UserRole).toString();

        // 更新 卷
        QSqlQuery volume_sql;	//执行操作类对象

        // 准备sql语句
        volume_sql.prepare("UPDATE volumes SET is_deleted = 1, deleted_time = :deletedTime  WHERE volume_id = :volumeID");
        // 绑定数据
        volume_sql.bindValue(":deletedTime", timestamp);
        volume_sql.bindValue(":volumeID", volumeID);

        // 执行sql语句
        if(!volume_sql.exec())
            qDebug()<<volume_sql.lastError(); // 执行 sql语句

        // 结束 sql 语句
        volume_sql.finish();

        // 获取 卷节点 的索引，再根据 索引 移除卷节点
        this->takeTopLevelItem(this->indexOfTopLevelItem(volumeItem));
        // 释放 卷节点 的内存
        delete volumeItem;
        // 指针置空
        volumeItem = nullptr;

        // *************** 更新 novels表 *****************************
        // 在 updateDataBase()函数已实现，关闭时自动更新
    }
    else // 打开数据库失败
        QMessageBox::warning(this, "错误", "打开数据库失败");

    // 关闭数据库
    AppDatabase::getInstance()->quit();

    // 目录项 已删除
    emit directioryItem_deleted();
}
// 导出 卷
void DirectoryTreeWidget::exportVolumeItemAsTXT(QTreeWidgetItem *volumeItem, QString savedFilePath)
{
    // 新建 文件
    QFile volumeFile(savedFilePath); // 文件操作对象
    volumeFile.open(QIODevice::WriteOnly|QIODevice::Text); // 创建文件

    // 使用文本流 写入文件
    QTextStream writeStream(&volumeFile);

    // 遍历 卷下 每一章
    for(int chapterIndex=0; chapterIndex<volumeItem->childCount(); chapterIndex++)
    {
        // 获取分卷下 每一章节 的ID，标题
        QString chapterID = volumeItem->child(chapterIndex)->data(int(DirectoryDateColume::IDColume),Qt::UserRole).toString();
        QString chapterTitle = volumeItem->child(chapterIndex)->text(int(DirectoryTextColume::TitleColume));

        // 要导出的 章节是正在编辑的章节，以 当前文本框的内容为准
        if(chapterID == m_chapterIDSent)
            emit saveBeingOpenedChapter(); // 发送信号 保存 当前打开着 章节

        // 等待 文本框 保存章节内容

        // 章节 对应 文件路径
        QString chapterFilePath = QDir::currentPath() + "/Config/LocalChapters/" + chapterID + ".txt";

        // 打开文件
        QFile file(chapterFilePath); // 通过文件路径创建文件对象
        file.open(QFile::ReadOnly|QIODevice::Text); // 文件打开方式
        QString chapterContent = file.readAll(); // 获取 章节文件中的 全部字符
        file.close(); // 关闭文件

        // 写入 章节标题，章节内容，并与下一章 再空一行
        writeStream << chapterTitle << "\n" << chapterContent << "\n\n";
    }

    // 关闭 文件
    volumeFile.close();
}

// 鼠标按下事件
void DirectoryTreeWidget::mousePressEvent(QMouseEvent *event)
{
    if(itemAt(event->pos()) != nullptr) // 在 节点处 按下
    {
        // 有节点被拖拽
        m_draggedItem = itemAt(event->pos()); // 获取被拖动的节点
    }

    // 调用 父类 鼠标按下 事件
    QTreeWidget::mousePressEvent(event);
}
// 拖拽移动事件
void DirectoryTreeWidget::dragMoveEvent(QDragMoveEvent *event)
{

}
// 拖拽释放事件
void DirectoryTreeWidget::dropEvent(QDropEvent *event)
{
    if(m_draggedItem == nullptr) // 无要拖拽项目
    {
        event->ignore(); // 忽略事件
        return; // 直接返回
    }

    // 获取 释放时，鼠标所在的目标项目
    QTreeWidgetItem *targetItem = itemAt(event->pos());


    // 根据 目标节点 和 要拖拽节点 的类型 进行不同拖拽处理
    if(targetItem == nullptr)
    {
        // 目标项目为空，即 项目被拖拽到空白位置
        if(m_draggedItem->type() == int(DirectoryItemType::VolumeItem))
        {
            // 卷到空白，可以释放在空白位置
            QTreeWidget::dropEvent(event); // 调用父类事件，正常释放
        }
        else if(m_draggedItem->type() == int(DirectoryItemType::ChapterItem))
        {
            // 章到空白，不可以释放在空白位置
            event->ignore(); // 忽略事件，即不可释放
        }
    }
    else if(targetItem->type() == int(DirectoryItemType::VolumeItem))
    {
        // 目标项目为卷节点，卷到卷
        if(m_draggedItem->type() == int(DirectoryItemType::VolumeItem))
        {
            // 卷节点 被拖拽到 原来位置
            if(m_draggedItem == targetItem)
            {
                // 拖拽到原来位置
                event->ignore();// 忽略事件，不可释放
                return;
            }

            // 卷节点被拖拽到其他卷节点位置，插入到该位置
            int targetItemIndex=this->indexOfTopLevelItem(targetItem); // 目标节点的索引
            int draggedItemIndex=this->indexOfTopLevelItem(m_draggedItem); // 拖拽节点的索引

            // 移除要拖拽的卷节点，但要拖拽的卷节点 仍存在
            this->takeTopLevelItem(draggedItemIndex);

            // 将拖拽的卷，插入到目标索引
            this->insertTopLevelItem(targetItemIndex, m_draggedItem);

            // 忽略事件
            event->ignore(); // 已完成拖拽，不用释放。避免卷节点成为子节点，即卷节点不能成为子节点
        }
        else if(m_draggedItem->type() == int(DirectoryItemType::ChapterItem))
        {
            // 章到卷，可以释放
            QTreeWidget::dropEvent(event); // 调用父类事件，正常释放
        }
    }
    else if(targetItem->type() == int(DirectoryItemType::ChapterItem))
    {
        // 目标项目为章节点
        if(m_draggedItem->type() == int(DirectoryItemType::VolumeItem))
        {
            // 卷到章，不可以释放
            event->ignore(); // 忽略事件，即不可释放
        }
        else if(m_draggedItem->type() == int(DirectoryItemType::ChapterItem))
        {
            // 章到章，则要插入到目标节点位置
            if(m_draggedItem == targetItem)
            {
                // 章节点 被拖拽到 原来位置
                event->ignore();// 忽略事件
                return;
            }

            // 获取目标节点在其父节点的索引
            int targetIndex = targetItem->parent()->indexOfChild(targetItem);
            // 移除要拖拽 的章节点
            m_draggedItem->parent()->removeChild(m_draggedItem);
            // 将要拖拽章节点 插入到 新父卷下 索引位置
            targetItem->parent()->insertChild(targetIndex, m_draggedItem);
        }
    }

    // 更新节点数据
    updateItemData();
}


// 文本框的章节标题 发生变化，处理槽函数，更新对应章节点的标题
void DirectoryTreeWidget::setChapterNewTitle(QString chapterID,QString newChapterTitle)
{
    // ********************* chapterID == m_chapterIDSent *************************************

    // 遍历 每一个节点 查找要修改的章节点
    for(int volumeIndex=0; volumeIndex < this->topLevelItemCount(); volumeIndex++)
    {
        for(int chapterIndex=0; chapterIndex < this->topLevelItem(volumeIndex)->childCount(); chapterIndex++)
        {
            // 查找 章节点
            if(this->topLevelItem(volumeIndex)->child(chapterIndex)->data(int(DirectoryDateColume::IDColume), Qt::UserRole).toString() == chapterID)
            {
                // 设置 章节点 的标题
                this->topLevelItem(volumeIndex)->child(chapterIndex)->setText(int(DirectoryTextColume::TitleColume), newChapterTitle);
                break; // 退出循环
            }
        }
    }
}
// 设置 章节点 字数
void DirectoryTreeWidget::setChapterNodeWordCount(QString chapterID,int wordCount)
{
    // ********************* chapterID == m_chapterIDSent *************************************

    // 遍历 每一个节点 查找要修改的章节点
    for(int volumeIndex=0; volumeIndex<this->topLevelItemCount(); volumeIndex++)
    {
        for(int chapterIndex=0; chapterIndex<this->topLevelItem(volumeIndex)->childCount(); chapterIndex++)
        {
            // 查找 章节点
            if(this->topLevelItem(volumeIndex)->child(chapterIndex)->data(int(DirectoryDateColume::IDColume),Qt::UserRole).toString() == chapterID)
            {
                // 设置 章节点 的标题
                this->topLevelItem(volumeIndex)->child(chapterIndex)->setText(int(DirectoryTextColume::CountColume),QString::number(wordCount)+"字");
                break; // 退出循环
            }
        }
    }
}

// 右键弹出菜单 处理槽函数
void DirectoryTreeWidget::do_directoryTree_MenuRequested(const QPoint &pos)
{
    // 不使用 pos
    Q_UNUSED(pos);

    // 新建 菜单
    QMenu *menu=new QMenu(this);
    QMenu *exportMenu;

    // 获取 被选中的项目
    QList<QTreeWidgetItem*> itemList = this->selectedItems();

    // 根据 被选中的项目类型，进行 不同弹出菜单处理
    if (itemList.size() < 1 || itemList.first() == nullptr)
    {
        // 在空白处 右键
        menu->addAction("新建分卷",this,[=](){
            this->addVolumeItem();
        });
    }
    else // 存在 被选中的项目
    {
        // 获取节点 类型，节点 标题
        int itemType = itemList.first()->type();
        QString itemText = itemList.first()->text(int(DirectoryTextColume::TitleColume));

        // 判断 节点类型
        if(itemType == int(DirectoryItemType::VolumeItem))
        {
            // 在 卷 右键
            menu->addAction("新建章节",this,[=](){
                this->addChapterItem(itemList.first());
            });
            menu->addAction("重命名分卷",this,[=](){
                this->renameItem(itemList.first());
            });
            exportMenu = new QMenu("导出分卷",menu);
            exportMenu->addAction("导出为TXT",this,[=](){
                // 当前文件路径，使用 节点名为 默认文件名
                QString curPath= QCoreApplication::applicationDirPath() + QString("/%1.txt").arg(itemText);
                // 文件保存对话框，获取文件路径
                QString dlgTitle = "导出分卷"; // 对话框标题
                QString filter = "TXT(*.txt)"; // 文件类型
                QString filePath = QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);

                if(filePath != "") // 文件路径不为空，进行导出
                {
                    this->exportVolumeItemAsTXT(itemList.first(), filePath);

                    // 导出完成，弹出 提示对话框
                    QMessageBox::information(this,"导出分卷",QString("分卷: %1 导出完成!").arg(itemText));
                }
            });
            // exportMenu->addAction("导出成HTML");
            // exportMenu->addAction("导出成PDF");
            // 将 导出菜单 添加到 主菜单
            menu->addMenu(exportMenu);
            menu->addAction("删除",this,[=](){
                // 打开 对话框
                QMessageBox::StandardButton resultBtn = QMessageBox::warning(this,"删除分卷",QString("确认删除：%1 ?").arg(itemText),QMessageBox::Cancel|QMessageBox::Yes);

                // 点击 取消按钮
                if(resultBtn == QMessageBox::Cancel)
                    return; // 直接结束，不做删除操作

                // 执行 删除卷 及 卷下所有章
                this->removeVolumeItem(itemList.first());
            });
        }
        else if(itemType == int(DirectoryItemType::ChapterItem))
        {
            // 在 章 右键
            menu->addAction("重命名章节",this,[=](){
                this->renameItem(itemList.first());
            });
            exportMenu = new QMenu("导出章节",menu);
            exportMenu->addAction("导出为TXT",this,[=](){
                // 当前文件路径，使用 节点名为 默认文件名
                QString curPath= QCoreApplication::applicationDirPath()+ QString("/%1.txt").arg(itemText);
                // 文件保存对话框，获取文件路径
                QString dlgTitle = "导出章节"; // 对话框标题
                QString filter = "TXT(*.txt)"; // 文件类型
                QString filePath = QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);

                if(filePath != "") // 文件路径不为空，进行导出
                {
                    this->exportChapterItemAsTXT(itemList.first(), filePath);

                    // 导出完成，弹出 提示对话框
                    QMessageBox::information(this,"导出分卷",QString("分卷: %1 导出完成!").arg(itemText));
                }
            });
            // exportMenu->addAction("导出成HTML");
            // exportMenu->addAction("导出成PDF");
            // 将 导出菜单 添加到 主菜单
            menu->addMenu(exportMenu);

            menu->addAction("删除",this,[=](){
                // 打开 对话框
                QMessageBox::StandardButton resultBtn = QMessageBox::warning(this,"删除章节",
                                                                             QString("确认删除：%1 ?").arg(itemText),
                                                                             QMessageBox::Cancel|QMessageBox::Yes);

                // 点击 取消按钮
                if(resultBtn == QMessageBox::Cancel)
                    return; // 直接结束，不做删除操作

                // 执行 删除 章节
                this->romoveChapterItem(itemList.first());
            });
        }
    }

    // 运行 弹出菜单
    menu->exec(QCursor::pos());
}
