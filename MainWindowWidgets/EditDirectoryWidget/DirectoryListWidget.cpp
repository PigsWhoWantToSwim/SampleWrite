#include "DirectoryListWidget.h"

DirectoryListWidget::DirectoryListWidget(QString novel, QWidget *parent)
    : BasedDirectoryWidget{parent}
{
    // 设置 小说ID
    m_novelID = novel;

    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

DirectoryListWidget::~DirectoryListWidget()
{
    // 在销毁前更新保存
    updateItemData();
    updateDateBase();
}



// 初始化 成员变量
void DirectoryListWidget::initMemberVariable()
{
    // 默认升序
    m_order = Qt::AscendingOrder;
}
// 初始化 布局
void DirectoryListWidget::initLayout()
{
    // 读取 数据库
    readDataBase();

    // 更新列宽
    this->updateDirectoryColumnWidth();

}
// 初始化 绑定
void DirectoryListWidget::initConnect()
{
    connect(this, &DirectoryListWidget::customContextMenuRequested,
            this, &DirectoryListWidget::do_directoryList_MenuRequested);

    // 绑定 项双击信号，发送章ID槽函数
    connect(this,&DirectoryListWidget::itemDoubleClicked,this,[=](QTreeWidgetItem *item, int column){
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

// 读取 数据库
void DirectoryListWidget::readDataBase()
{
    if (AppDatabase::getInstance()->openDatabase()) // 数据库打开成功
    {
        // 查询relatedArticles表中 属于novel_id的 所有记录
        QSqlQuery chapters_sql;	//执行操作类对象
        // 准备sql语句
        chapters_sql.prepare("SELECT * FROM chapters "
                             "WHERE volume_id = :novelID AND is_deleted = 0 "
                             "ORDER BY chapter_index");
        // 绑定数据
        chapters_sql.bindValue(":novelID", m_novelID);
        // 执行sql语句
        if(!chapters_sql.exec())
        {
            // 执行 sql语句 失败
            qDebug() << chapters_sql.lastError();
        }
        else // 执行成功
        {
            // 读取每一条 相关文章记录
            while(chapters_sql.next())
            {
                // 获取信息
                QString chapterID = chapters_sql.value("chapter_id").toString();
                QString chapterTitle = chapters_sql.value("chapter_title").toString();
                int wordsCount = chapters_sql.value("words_count").toInt();
                int chapterIndex = chapters_sql.value("chapter_index").toInt();

                // 创建一行项目
                QTreeWidgetItem* chapterItem = new QTreeWidgetItem(int(DirectoryItemType::ChapterItem));

                // 设置数据 idCol列，第1个数据，为id。indexCol列，第1个数据，为索引
                chapterItem->setData(int(DirectoryDateColume::IDColume), Qt::UserRole,chapterID);
                chapterItem->setData(int(DirectoryDateColume::IndexColume), Qt::UserRole, chapterIndex);

                // 设置文本显示内容 titleCol列 为文章标题，numberCol列 为字数
                chapterItem->setText(int(DirectoryTextColume::TitleColume), chapterTitle);
                chapterItem->setText(int(DirectoryTextColume::CountColume), QString::number(wordsCount)+"字");


                // 设置 光标悬停显示
                chapterItem->setToolTip(int(DirectoryTextColume::TitleColume), chapterTitle); // 显示 卷标题

                // 设置图标
                // chapterItem->setIcon(int(),QIcon(":/Image/paper - gray.png"));

                // 设置 数量列 右对齐
                chapterItem->setTextAlignment(int(DirectoryTextColume::CountColume),Qt::AlignRight);

                // 添加成为 顶层节点
                this->addTopLevelItem(chapterItem);
            }
        }
        // 结束 卷sql语句
        chapters_sql.finish();
    }
    else // 数据库 打开 失败
        QMessageBox::warning(this, "错误", "打开数据库失败");

    // 关闭数据库
    AppDatabase::getInstance()->quit();
}
// 更新 数据库
void DirectoryListWidget::updateDateBase()
{
    if (AppDatabase::getInstance()->openDatabase()) // 数据库打开成功
    {
        // 查询relatedArticles表中 属于novel_id的所有记录
        QSqlQuery chapters_sql;	// 更新volumes表 执行操作类对象

        // 遍历 每一个 卷节点
        for(int realChapterIndex = 0; realChapterIndex<this->topLevelItemCount(); realChapterIndex++)
        {
            // 获取章节点 存储的数据
            // 章ID 用于查询，更新 卷标题，卷索引，章节数
            QString chapterID = topLevelItem(realChapterIndex)->data(int(DirectoryDateColume::IDColume),Qt::UserRole).toString();
            QString realChapterTitle = topLevelItem(realChapterIndex)->text(int(DirectoryTextColume::TitleColume));
            // 获取顶层节点 存储的 字数 数据
            QString storedWordsCountStr = topLevelItem(realChapterIndex)->text(int(DirectoryTextColume::CountColume)); // 获取 字符串
            storedWordsCountStr.chop(1); // 去除 “字”字
            int realWordsCount = storedWordsCountStr.toInt(); // 转为 整型

            // 准备sql语句
            chapters_sql.prepare("UPDATE chapters SET "
                                 "chapter_title = :realChapterTitle, "
                                 "chapter_index = :realChapterIndex, "
                                 "words_count = :realWordsCount "
                                 "WHERE chapter_id = :chapterID");
            // 绑定数据
            chapters_sql.bindValue(":realChapterTitle",realChapterTitle);
            chapters_sql.bindValue(":realChapterIndex",realChapterIndex);
            chapters_sql.bindValue(":realWordsCount",realWordsCount);
            chapters_sql.bindValue(":chapterID",chapterID);

            // 执行sql语句
            if(!chapters_sql.exec())
                qDebug()<<chapters_sql.lastError(); // 执行 sql语句 失败

            // 结束 volumes_sql语句
            chapters_sql.finish();
        }
    }
    else // 数据库 打开 失败
        QMessageBox::warning(this, "错误", "打开数据库失败");

    // 关闭数据库
    AppDatabase::getInstance()->quit();
}
// 更新 节点数据
void DirectoryListWidget::updateItemData()
{
    // 遍历 所有 相关作品节点
    for(int realChapterIndex=0; realChapterIndex < this->topLevelItemCount(); realChapterIndex++)
    {
        // 节点存储的数据有，ID，index，title，words
        // ID一直不变。title和words始终都是正确的，不用更新。index会变化，因此要更新index

        // 获取相关作品 存储的 相关作品ID，索引数据
        int storedRelatedIndex = topLevelItem(realChapterIndex)->data(int(DirectoryDateColume::IndexColume),Qt::UserRole).toInt();

        // 相关作品 储存的索引 与 实际索引 不同，更新卷索引
        if(storedRelatedIndex != realChapterIndex)
        {
            topLevelItem(realChapterIndex)->setData(int(DirectoryDateColume::IndexColume),Qt::UserRole,realChapterIndex);
        }
    }
}


void DirectoryListWidget::addChapterItem()
{
    // 获取 当前 文章数量
    int chaptersCount = this->topLevelItemCount();

    // 获取 初始 标题
    QString orignalTitelText = "相关"+QString::number(chaptersCount+1); // 默认标题
    QString newTitleText = orignalTitelText; // 用于接收 新标题，默认为 相关i，防止点击取消

    // 是否取消新建 文章
    bool isCancelNewArticle = false;

    newTitleText = getNewTitleFromDialog("新建相关",orignalTitelText,"请输入相关名",isCancelNewArticle);

    // 确认或取消
    if(isCancelNewArticle) // 取消新建 卷
        return;

    // 点击确认，已获取到新标题
    // 打开数据库
    if (AppDatabase::getInstance()->openDatabase()) // 打开 数据库成功
    {
        // 时间戳
        QDateTime dateTime = QDateTime::currentDateTime(); // 获取 当前时间
        QString timestamp = dateTime.toString("yyMMddhhmmss");// 字符串格式化
        QString timestamp_birthTime = dateTime.toString("yyyy-MM-dd hh:mm:ss"); // 创建时间

        // 准备信息
        QString ID_encoding = timestamp + getRandomLetterString(2);
        QString chapterID = "r" + ID_encoding; // 相关文章ID
        QString chapterFilePath = QDir::currentPath() + "/Config/LocalChapters/" + chapterID + ".txt";

        // 备份文件版本ID、文件路径
        QString backupVersionID = "b" + ID_encoding;
        QString backupFilePath = QDir::currentPath() + "/Config/LocalChapters/" + backupVersionID + ".txt";

        // *************************** 添加节点 **********************
        // 新创建一行节点 为卷节点
        QTreeWidgetItem *newChapterItem=new QTreeWidgetItem(int(DirectoryItemType::ChapterItem));

        // 保存节点信息
        // idCol列，第1个数据，为id。indexCol列，第1个数据，为索引
        newChapterItem->setData(int(DirectoryDateColume::IDColume),Qt::UserRole,chapterID);
        newChapterItem->setData(int(DirectoryDateColume::IndexColume),Qt::UserRole, chaptersCount);

        // 设置文本显示内容
        newChapterItem->setText(int(DirectoryTextColume::TitleColume),newTitleText);
        newChapterItem->setText(int(DirectoryTextColume::CountColume),"0字");

        // 设置 光标悬停提示
        newChapterItem->setToolTip(int(DirectoryTextColume::TitleColume),newTitleText);

        // 设置图标
        // newChapterItem->setIcon(int(DirectoryTextColume::TitleColume),QIcon(":/Image/paper - gray.png"));

        // 设置 数量列 右对齐
        newChapterItem->setTextAlignment(int(DirectoryTextColume::CountColume),Qt::AlignRight);

        // 添加成为 顶层节点
        this->addTopLevelItem(newChapterItem);

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
        // 更新 chapters表
        QSqlQuery chapters_sql;
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
        chapters_sql.bindValue(":volumeID", m_novelID);
        chapters_sql.bindValue(":filePathURL", chapterFilePath);
        chapters_sql.bindValue(":chapterIndex", chaptersCount);

        // 执行sql语句
        if(!chapters_sql.exec())
            qDebug()<<chapters_sql.lastError(); // 执行 sql语句 失败

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

    }
    else // 打开 数据库 失败
        QMessageBox::warning(this, "错误", "打开数据库失败");

    // 关闭数据库
    AppDatabase::getInstance()->quit();
}

void DirectoryListWidget::romoveChapterItem(QTreeWidgetItem *chapterItem)
{
    // 打开数据库
    if (AppDatabase::getInstance()->openDatabase()) // 数据库打开成功
    {
        // ******************* 更新 chapters表 *****************************
        //执行操作类对象
        QSqlQuery chapter_sql;

        // 获取 章ID
        QString chapterID = chapterItem->data(int(DirectoryDateColume::IDColume),Qt::UserRole).toString();

        // 准备sql语句
        chapter_sql.prepare("UPDATE chapters SET is_deleted = 1 WHERE chapter_id = :chapterID");
        // 绑定数据
        chapter_sql.bindValue(":chapterID", chapterID);

        // 执行sql语句
        if(!chapter_sql.exec())
            qDebug()<<chapter_sql.lastError(); // 执行 sql语句 失败

        // 结束 sql 语句
        chapter_sql.finish();

        // 移除 子节点
        this->takeTopLevelItem(this->indexOfTopLevelItem(chapterItem));

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

void DirectoryListWidget::setChapterNewTitle(QString chapterID, QString newChapterTitle)
{
    // 遍历 每一个节点 查找要修改的章节点
    for(int chapterIndex=0; chapterIndex < this->topLevelItemCount(); chapterIndex++)
    {
        // 查找 章节点
        if(this->topLevelItem(chapterIndex)->data(int(DirectoryDateColume::IDColume), Qt::UserRole).toString() == chapterID)
        {
            // 设置 章节点 的标题
            this->topLevelItem(chapterIndex)->setText(int(DirectoryTextColume::TitleColume), newChapterTitle);
            break; // 退出循环
        }
    }
}

void DirectoryListWidget::setChapterNodeWordCount(QString chapterID, int wordCount)
{
    // 遍历 每一个节点 查找要修改的章节点
    for(int chapterIndex=0; chapterIndex<this->topLevelItemCount(); chapterIndex++)
    {
        // 查找 章节点
        if(this->topLevelItem(chapterIndex)->data(int(DirectoryDateColume::IDColume),Qt::UserRole).toString() == chapterID)
        {
            // 设置 章节点 的标题
            this->topLevelItem(chapterIndex)->setText(int(DirectoryTextColume::CountColume),QString::number(wordCount)+"字");
            break; // 退出循环
        }
    }
}


// 鼠标按下事件
void DirectoryListWidget::mousePressEvent(QMouseEvent *event)
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
void DirectoryListWidget::dragMoveEvent(QDragMoveEvent *event)
{

}
// 拖拽释放事件
void DirectoryListWidget::dropEvent(QDropEvent *event)
{
    if(m_draggedItem == nullptr)
    {
        // 无要拖拽项目
        event->ignore(); // 忽略事件
        return; // 直接返回
    }

    // 获取 释放时，鼠标所在的目标项目
    QTreeWidgetItem *targetItem = itemAt(event->pos());

    // 根据 目标节点类型 进行不同处理
    if(targetItem == nullptr)
    {
        // 目标项目为空，即释放在空白位置
        // 章到空白，不可以释放在空白位置
        event->ignore(); // 忽略事件，即不可释放
    }
    else
    {
        // 文章到文章，则要插入到目标节点位置
        if(m_draggedItem == targetItem)
        {
            // 拖拽到原来位置
            event->ignore();// 忽略事件，不可释放
            return;
        }

        // 章到章，插入到该位置
        int targetItemIndex=this->indexOfTopLevelItem(targetItem); // 目标节点的索引
        int draggedItemIndex=this->indexOfTopLevelItem(m_draggedItem); // 拖拽节点的索引

        // 移除要拖拽的卷节点
        this->takeTopLevelItem(draggedItemIndex);

        // 将拖拽的卷，插入到目标索引
        this->insertTopLevelItem(targetItemIndex, m_draggedItem);

        // 忽略事件
        event->ignore(); // 不可释放成为子节点，即文章节点不能成为子节点
    }

    // 更新 节点数据
    updateItemData();
}

// 右键 弹出菜单处理槽函数
void DirectoryListWidget::do_directoryList_MenuRequested(const QPoint &pos)
{
    // 不使用pos
    Q_UNUSED(pos);
    // 新建 弹出菜单
    QMenu *menu=new QMenu(this);
    QMenu *exportMenu;

    // 获取 相关列表 中被选中的项 列表
    QList<QTreeWidgetItem*> itemList = this->selectedItems();

    // 根据 被选中项目 类型 进行不同处理
    if (itemList.size() < 1 || nullptr == itemList.first()) // 在空白处 点击，或项目列表为空
    {
        // 在空白处 右键
        menu->addAction("新建相关",this,[=](){
            this->addChapterItem();
        });
    }
    else // 选中 项目
    {
        QString itemText = itemList.first()->text(int(DirectoryTextColume::TitleColume));
        // 在 文章 右键
        // 重命名 文章节点
        menu->addAction("重命名相关",this,[=](){
            this->renameItem(itemList.first());
        });

        // 导出 文章节点
        exportMenu = new QMenu("导出章节",menu);
        exportMenu->addAction("导出为TXT",this,[=](){
            // 当前文件路径，使用 节点名为 默认文件名
            QString curPath= QCoreApplication::applicationDirPath()+ QString("/%1.txt").arg(itemText);
            // 文件保存对话框，获取文件路径
            QString dlgTitle = "导出相关"; // 对话框标题
            QString filter = "TXT(*.txt)"; // 文件类型
            QString filePath = QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);

            if(filePath != "") // 文件路径不为空，进行导出
            {
                this->exportChapterItemAsTXT(itemList.first(), filePath);

                // 导出完成，弹出 提示对话框
                QMessageBox::information(this,"导出相关",QString("相关: %1 导出完成!").arg(itemText));
            }
        });
        // exportMenu->addAction("导出成HTML");
        // exportMenu->addAction("导出成PDF");
        // 将 导出菜单 添加到 主菜单
        menu->addMenu(exportMenu);

        menu->addAction("删除",this,[=](){
            // 获取 文章 标题
            QString chapterTitle = itemList.first()->text(int(DirectoryTextColume::TitleColume));
            // 打开 对话框
            QMessageBox::StandardButton resultBtn = QMessageBox::warning(this,"删除相关",
                                                                         "确认删除："+chapterTitle+" ?",
                                                                         QMessageBox::Cancel|QMessageBox::Yes);

            // 点击 取消按钮
            if(resultBtn == QMessageBox::Cancel)
                return; // 直接结束，不做删除操作

            // 执行 删除章
            this->romoveChapterItem(itemList.first());
        });
    }
    // 显示
    menu->exec(QCursor::pos());
}

// 重新排序
void DirectoryListWidget::reorder()
{
    if(m_order == Qt::AscendingOrder) // 当前为 升序
    {
        // 进行 降序
        this->sortByColumn(int(DirectoryDateColume::IndexColume), Qt::DescendingOrder);


        // this->sortItems(int(DirectoryDateColume::IndexColume), Qt::DescendingOrder);

        m_order = Qt::DescendingOrder;
    }
    else if(m_order == Qt::DescendingOrder) // 当前为 降序
    {
        // 进行 升序
        this->sortByColumn(int(DirectoryDateColume::IndexColume), Qt::AscendingOrder);
        // this->sortItems(int(DirectoryDateColume::IndexColume), Qt::AscendingOrder);
        m_order = Qt::AscendingOrder;
    }
}
