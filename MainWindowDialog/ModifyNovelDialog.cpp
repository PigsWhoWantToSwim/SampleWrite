#include "ModifyNovelDialog.h"

ModifyNovelDialog::ModifyNovelDialog(QWidget *parent)
    : QDialog{parent}
{
    // 初始化
    initMemberVariable();
    initLayout();
    initConnect();
}

// 初始化 成员变量
void ModifyNovelDialog::initMemberVariable()
{
    // 设置透明
    this->setAttribute(Qt::WA_TranslucentBackground); // 隐藏4个角
    // 关闭后删除
    //this->setAttribute(Qt::WA_DeleteOnClose, true);
    //设置无边框属性
    this->setWindowFlags(Qt::FramelessWindowHint);//  | windowFlags()
    this->setFixedSize(MODIFYNOVELDIALOG_WIDTH,MODIFYNOVELDIALOG_HEIGHT);

    m_novelID = "";
    m_novelCoverPath = ":/image/add book cover.png";

    m_isEdited = false;
    m_isCoverExisted = false;

}
// 初始化 布局
void ModifyNovelDialog::initLayout()
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

    m_labDialogTitle = new QLabel(m_frmContainer);
    m_labDialogTitle->setProperty("WidgetStyleClass", "DialogTitleLabel");
    m_labDialogTitle->setText("新建作品");
    containerLayout->addWidget(m_labDialogTitle);

    // 小说信息区，左侧为 封面输入区，右侧为 文本信息输入区
    QHBoxLayout *novelInfoLayout = new QHBoxLayout;
    novelInfoLayout->setSpacing(0);
    novelInfoLayout->setContentsMargins(0, 0, 0, 0);
    containerLayout->addLayout(novelInfoLayout);

    // 封面输入区
    m_frmCoverContainer = new QFrame(m_frmContainer);
    m_frmCoverContainer->setObjectName("ModifyNovelDialog_frmCoverContainer");
    m_frmCoverContainer->setFixedSize(MODIFYNOVELDIALOG_COVERCONTAINER_WIDTH,
                                      MODIFYNOVELDIALOG_COVERCONTAINER_HEIGHT);
    novelInfoLayout->addWidget(m_frmCoverContainer);

    // 添加 封面按钮
    m_btnAddCover = new QPushButton(m_frmCoverContainer);
    m_btnAddCover->setToolTip("添加封面");
    m_btnAddCover->setIconSize(QSize(MODIFYNOVELDIALOG_COVER_WIDTH,
                                     MODIFYNOVELDIALOG_COVER_HEIGHT));
    m_btnAddCover->setIcon(QIcon(m_novelCoverPath));
    m_btnAddCover->setGeometry(QRect(0,
                                     MODIFYNOVELDIALOG_CANCELCOVERICON_MAXSIZE/2,
                                     MODIFYNOVELDIALOG_COVER_WIDTH,
                                     MODIFYNOVELDIALOG_COVER_HEIGHT));
    m_btnAddCover->setCursor(QCursor(Qt::PointingHandCursor));

    // 取消封面按钮 要变大变小，所以要预留尺寸，并且要放在m_btnAddCover后，才能在上层
    // 取消图标尺寸偏差
    int cancelIconSizeDiff = MODIFYNOVELDIALOG_CANCELCOVERICON_MAXSIZE - MODIFYNOVELDIALOG_CANCELCOVERICON_MINSIZE;
    QRect cancelIconMinRect(MODIFYNOVELDIALOG_COVERCONTAINER_WIDTH - MODIFYNOVELDIALOG_CANCELCOVERICON_MAXSIZE + cancelIconSizeDiff/2,
                            cancelIconSizeDiff/2,
                            MODIFYNOVELDIALOG_CANCELCOVERICON_MINSIZE,
                            MODIFYNOVELDIALOG_CANCELCOVERICON_MINSIZE);

    m_btnCancelCover = new QPushButton(m_frmCoverContainer);
    m_btnCancelCover->setObjectName("ModifyNovelDialog_btnCancelCover");
    m_btnCancelCover->setIcon(QIcon(":/image/circle close.png"));
    m_btnCancelCover->setToolTip("取消封面");
    m_btnCancelCover->setIconSize(QSize(MODIFYNOVELDIALOG_CANCELCOVERICON_MINSIZE,
                                        MODIFYNOVELDIALOG_CANCELCOVERICON_MINSIZE));
    m_btnCancelCover->setGeometry(cancelIconMinRect);
    m_btnCancelCover->setCursor(QCursor(Qt::PointingHandCursor));
    m_btnCancelCover->installEventFilter(this); // 用来进行 放大 缩小
    m_btnCancelCover->setVisible(false);

    // 文本信息输入区
    QGridLayout *novelTextInfoLayout = new QGridLayout;
    novelTextInfoLayout->setHorizontalSpacing(10);
    novelTextInfoLayout->setVerticalSpacing(0);
    novelTextInfoLayout->setContentsMargins(0, 0, 0, 0);
    novelInfoLayout->addLayout(novelTextInfoLayout);

    // 信息输入区
    m_labNoverTitle = new QLabel("书名:", m_frmContainer);
    m_labNoverTitle->setProperty("WidgetStyleClass", "ModifyNovelDialog_InfoLabel");
    novelTextInfoLayout->addWidget(m_labNoverTitle, 0, 0, 1, 1);

    m_ledNoverTitle = new QLineEdit(m_frmContainer);
    m_ledNoverTitle->setProperty("WidgetStyleClass", "ModifyNovelDialog_InfoLineEdit");
    m_ledNoverTitle->setPlaceholderText("请输入书名");
    novelTextInfoLayout->addWidget(m_ledNoverTitle, 0, 1, 1, 1);

    m_labAuthor = new QLabel("作者:", m_frmContainer);
    m_labAuthor->setProperty("WidgetStyleClass", "ModifyNovelDialog_InfoLabel");
    novelTextInfoLayout->addWidget(m_labAuthor, 1, 0, 1, 1);

    m_ledAuthor = new QLineEdit(m_frmContainer);
    m_ledAuthor->setProperty("WidgetStyleClass", "ModifyNovelDialog_InfoLineEdit");
    m_ledAuthor->setText("未知");
    m_ledAuthor->setPlaceholderText("请输入作者名");
    novelTextInfoLayout->addWidget(m_ledAuthor, 1, 1, 1, 1);

    m_labNovelType = new QLabel("类型:", m_frmContainer);
    m_labNovelType->setProperty("WidgetStyleClass", "ModifyNovelDialog_InfoLabel");
    novelTextInfoLayout->addWidget(m_labNovelType, 2, 0, 1, 1);

    m_ledNovelType = new QLineEdit(m_frmContainer);
    m_ledNovelType->setProperty("WidgetStyleClass", "ModifyNovelDialog_InfoLineEdit");
    m_ledNovelType->setText("新作品");
    m_ledNovelType->setPlaceholderText("请输入类别");
    m_ledNovelType->setMaxLength(5);
    novelTextInfoLayout->addWidget(m_ledNovelType, 2, 1, 1, 1);


    // 按钮区，水平布局，左边是弹簧，中间是取消按钮，右边是确认按钮
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(30);
    btnLayout->setContentsMargins(0,0,0,0);
    containerLayout->addLayout(btnLayout);

    // 添加弹簧
    btnLayout->addSpacerItem(new QSpacerItem(1,1,QSizePolicy::Expanding,QSizePolicy::Fixed));

    m_btnCancel=new QPushButton("取消", m_frmContainer);
    m_btnCancel->setProperty("WidgetStyleClass", "CancelButton");
    m_btnCancel->setCursor(QCursor(Qt::PointingHandCursor));
    btnLayout->addWidget(m_btnCancel);

    m_btnSure=new QPushButton("确定", m_frmContainer);
    m_btnSure->setProperty("WidgetStyleClass", "SureButton");
    m_btnSure->setCursor(QCursor(Qt::PointingHandCursor));
    btnLayout->addWidget(m_btnSure);
}
// 初始化 绑定
void ModifyNovelDialog::initConnect()
{
    connect(m_btnAddCover, &QPushButton::clicked, this, &ModifyNovelDialog::do_btnAddCover_clicked);
    connect(m_btnCancelCover, &QPushButton::clicked, this, &ModifyNovelDialog::do_btnCancelCover_clicked);

    connect(m_btnCancel, &QPushButton::clicked, this, [=](){
        this->close();
    });
    connect(m_btnSure, &QPushButton::clicked, this, &ModifyNovelDialog::do_btnSure_clicked);
}

// 设置 对话框小说信息
void ModifyNovelDialog::setNovelInfo(QString novelID, QString novelTitle, QString author, QString novelType, QString novelCoverPath)
{
    // 是 修改 小说信息
    m_isEdited = true;

    m_novelID = novelID;

    // 设置 页面文本
    m_labDialogTitle->setText("修改作品");
    m_ledNoverTitle->setText(novelTitle);
    m_ledAuthor->setText(author);
    m_ledNovelType->setText(novelType);

    // 判断 封面是否 存在
    if(novelCoverPath == "") // 封面为空
    {
        m_isCoverExisted = false;
        m_novelCoverPath = "";
        m_btnAddCover->setIcon(QIcon(":/image/add book cover.png"));
        m_btnCancelCover->setVisible(false);
    }
    else // 不为空
    {
        m_isCoverExisted = true;
        m_novelCoverPath = novelCoverPath;
        m_novelCoverPixmap = QPixmap(novelCoverPath);
        m_btnAddCover->setIcon(QIcon(novelCoverPath));
        m_btnCancelCover->setVisible(true);
    }
}

// 新建小说
void ModifyNovelDialog::newNovel()
{
    // 时间戳
    QDateTime dateTime = QDateTime::currentDateTime();
    QString timestamp = dateTime.toString("yyMMddhhmmss");// 字符串格式化

    // 获取 小说 信息
    m_novelID = "n" + timestamp + getRandomLetterString(2);
    QString title = m_ledNoverTitle->text().simplified();
    QString author = m_ledAuthor->text().simplified() == "" ? "未知":m_ledAuthor->text().simplified();
    QString novel_type = m_ledNovelType->text().simplified() == "" ? "新作品":m_ledNovelType->text().simplified();
    QString novelCoverPath = "";

    if(m_isCoverExisted) // 存在 封面
    {
        // 设置 文件路径，保存新 封面
        novelCoverPath = QDir::currentPath() + "/Config/NovelCover/" + m_novelID + ".png";
        // 保存 封面 图片
        if(!m_novelCoverPixmap.save(novelCoverPath,"PNG")) // 保存 失败，使用默认封面
        {
            novelCoverPath = QDir::currentPath() + "/Config/NovelCover/default book cover.png";
        }
    }
    else // 不存在 封面，使用默认封面
    {
        novelCoverPath = QDir::currentPath() + "/Config/NovelCover/default book cover.png";
    }

    // 打开 数据库，插入 小说数据
    if(AppDatabase::getInstance()->openDatabase()) // 打开数据库 成功
    {
        QSqlQuery AppDB_query;	//执行操作类对象

        // 1.获取 当前 未被删除的小说 数量
        int novelsCount = 0;
        AppDB_query.prepare("SELECT COUNT(novel_id) AS novelsCount FROM novels WHERE is_deleted = 0;");

        if(AppDB_query.exec()) // 数据库语句 运行成功
        {
            while(AppDB_query.next()) // 逐条获取 数据库信息，虽然只有一条
                novelsCount = AppDB_query.value("novelsCount").toInt();
        }

        // 2.插入 一条新的 小说数据
        AppDB_query.prepare("INSERT INTO novels ("
                             "novel_id, novel_title, novel_author, novel_coverURL, novel_type, "
                             "volumes_count, chapters_count, words_count, novel_index, is_deleted, "
                             "deleted_time) VALUES ("
                             ":novel_id, :novel_title, :novel_author, :novel_coverURL, :novel_type, "
                             ":volumes_count, :chapters_count, :words_count, :novel_index, :is_deleted, "
                             ":deleted_time);");

        // 绑定数据
        AppDB_query.bindValue(":novel_id", m_novelID);
        AppDB_query.bindValue(":novel_title", title);
        AppDB_query.bindValue(":novel_author", author);
        AppDB_query.bindValue(":novel_coverURL", novelCoverPath);
        AppDB_query.bindValue(":novel_type", novel_type);
        AppDB_query.bindValue(":volumes_count", 0);
        AppDB_query.bindValue(":chapters_count", 0);
        AppDB_query.bindValue(":words_count", 0);
        AppDB_query.bindValue(":novel_index", novelsCount);
        AppDB_query.bindValue(":is_deleted", 0);
        AppDB_query.bindValue(":deleted_time", "");

        // 执行sql语句
        if(!AppDB_query.exec())
            QMessageBox::warning(this, "错误", AppDB_query.lastError().text());

        // 3.新建 相关章节 和 备份文件
        QStringList chapterTitleList;
        chapterTitleList << "作品设定" << "作品大纲" << "角色设定" << "灵感记录";

        QString ID_encoding;
        QString chapter_ID;
        QString chapter_title;
        QString volume_ID;
        QString filePathURL;

        QString backupChapter_ID;
        QString birthTime = dateTime.toString("yyyy-MM-dd hh:mm:ss");

        // 进行 循环生成
        for(int i = 0; i < chapterTitleList.count(); i++)
        {
            // 准备信息
            ID_encoding = timestamp + QString::number(i) + getRandomLetterString(1);

            chapter_ID = "r" + ID_encoding;
            chapter_title = chapterTitleList[i];
            volume_ID = m_novelID;
            filePathURL =  QDir::currentPath() + "/Config/LocalChapters/" + chapter_ID + ".txt";

            // 准备 相关章节 数据库语句
            AppDB_query.prepare("INSERT INTO chapters ("
                                "chapter_id, chapter_title, novel_id, volume_id, file_pathURL, "
                                "words_count, chapter_index, is_deleted"
                                ") VALUES ("
                                ":chapter_id, :chapter_title, :novel_id, :volume_id, :file_pathURL, "
                                ":words_count, :chapter_index, :is_deleted);");

            // 绑定数据
            AppDB_query.bindValue(":chapter_id", chapter_ID);
            AppDB_query.bindValue(":chapter_title", chapter_title);
            AppDB_query.bindValue(":novel_id", m_novelID);
            AppDB_query.bindValue(":volume_id", volume_ID);
            AppDB_query.bindValue(":file_pathURL", filePathURL);
            AppDB_query.bindValue(":words_count", 0);
            AppDB_query.bindValue(":chapter_index", i);
            AppDB_query.bindValue(":is_deleted", 0);

            // 执行sql语句
            if(!AppDB_query.exec())
                QMessageBox::warning(this, "错误", AppDB_query.lastError().text());


            // 新建 相关章节 文件
            QFile chapterFile(filePathURL); // 文件操作对象
            chapterFile.open(QIODevice::WriteOnly|QIODevice::Text); // 创建文件
            chapterFile.close(); // 关闭 文件

            // 准备 备份 信息
            backupChapter_ID = "b" + ID_encoding;
            filePathURL =  QDir::currentPath() + "/Config/LocalChapters/" + backupChapter_ID + ".txt";

            // 准备 相关章节 数据库语句
            AppDB_query.prepare("INSERT INTO backup_chapters ("
                                "version_id, chapter_id, birth_time, words_count, file_pathURL"
                                ") VALUES ("
                                ":version_id, :chapter_id, :birth_time, :words_count, :file_pathURL);");

            // 绑定数据
            AppDB_query.bindValue(":version_id", backupChapter_ID);
            AppDB_query.bindValue(":chapter_id", chapter_ID);
            AppDB_query.bindValue(":birth_time", birthTime);
            AppDB_query.bindValue(":words_count", 0);
            AppDB_query.bindValue(":file_pathURL", filePathURL);

            // 执行sql语句
            if(!AppDB_query.exec())
                QMessageBox::warning(this, "错误", AppDB_query.lastError().text());

            // 新建 备份文件
            QFile backupChapterFile(filePathURL); // 文件操作对象
            backupChapterFile.open(QIODevice::WriteOnly|QIODevice::Text); // 创建文件
            backupChapterFile.close(); // 关闭 文件
        }

        // 关闭 数据库
        AppDB_query.finish();
    }
    else // 打开数据库 失败
    {
        QMessageBox::warning(this, "错误", "打开数据库失败");
    }
    // 关闭 数据库
    AppDatabase::getInstance()->quit();

    // 发送 小说已添加信号
    emit novelAdded(m_novelID);
}
// 修改小说
void ModifyNovelDialog::editNovel()
{
    // 获取 小说 信息，均去掉前后空格，中间多个空格变一个
    QString title = m_ledNoverTitle->text().simplified();
    QString author = m_ledAuthor->text().simplified() == "" ? "未知":m_ledAuthor->text().simplified();
    QString novel_type = m_ledNovelType->text().simplified() == "" ? "新作品":m_ledNovelType->text().simplified();
    QString novelCoverPath = "";

    novelCoverPath = QDir::currentPath() + "/Config/NovelCover/"+ m_novelID + ".png";

    if(QFile::exists(novelCoverPath)) // 之前存在封面，进行删除
        QFile::moveToTrash(novelCoverPath); // 移动到回收站

    if(m_isCoverExisted) // 小说 封面 存在
    {
        // 重新 保存 封面 图片
        if(!m_novelCoverPixmap.save(novelCoverPath)) // 保存 失败，使用默认封面
        {
            novelCoverPath = QDir::currentPath() + "/Config/NovelCover/default book cover.png";
            QMessageBox::information(this, "提示", "封面保存失败", QMessageBox::Ok); // 保存失败
        }
    }
    else // 小说 封面 不存在，使用默认封面
    {
        novelCoverPath = QDir::currentPath() + "/Config/NovelCover/default book cover.png";
    }

    // 打开 数据库，插入 小说数据
    if(AppDatabase::getInstance()->openDatabase()) // 打开数据库 成功
    {
        QSqlQuery AppDB_query;	//执行操作类对象

        // 准备 更新 数据库语句
        AppDB_query.prepare("UPDATE novels SET novel_title = :novel_title, novel_author = :novel_author, "
                            "novel_coverURL = :novel_coverURL, novel_type = :novel_type WHERE novel_id = :novel_id;");

        // 绑定数据
        AppDB_query.bindValue(":novel_title", title);
        AppDB_query.bindValue(":novel_author", author);
        AppDB_query.bindValue(":novel_coverURL", novelCoverPath);
        AppDB_query.bindValue(":novel_type", novel_type);
        AppDB_query.bindValue(":novel_id", m_novelID);

        // 执行sql语句
        if(!AppDB_query.exec())
            QMessageBox::warning(this, "错误", AppDB_query.lastError().text());

        // 关闭 数据库
        AppDB_query.finish();

    }
    else // 打开数据库 失败
    {
        QMessageBox::warning(this, "错误", "打开数据库失败");
    }
    // 关闭 数据库
    AppDatabase::getInstance()->quit();

    emit novelEdited(m_novelID);
}

// 事件过滤器
bool ModifyNovelDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == m_btnCancelCover)
    {
        if(event->type() == QEvent::Enter)
        {
            // m_btnCancelCover->setIcon(QIcon(":/Image/close-2.png"));
            m_btnCancelCover->setIconSize(QSize(MODIFYNOVELDIALOG_CANCELCOVERICON_MAXSIZE,
                                                MODIFYNOVELDIALOG_CANCELCOVERICON_MAXSIZE));
            m_btnCancelCover->setGeometry(QRect(MODIFYNOVELDIALOG_COVERCONTAINER_WIDTH - MODIFYNOVELDIALOG_CANCELCOVERICON_MAXSIZE,
                                                0,
                                                MODIFYNOVELDIALOG_CANCELCOVERICON_MAXSIZE,
                                                MODIFYNOVELDIALOG_CANCELCOVERICON_MAXSIZE));
        }
        if(event->type()==QEvent::Leave)
        {
            // 取消图标尺寸偏差
            int cancelIconSizeDiff = MODIFYNOVELDIALOG_CANCELCOVERICON_MAXSIZE - MODIFYNOVELDIALOG_CANCELCOVERICON_MINSIZE;
            QRect cancelIconMinRect(MODIFYNOVELDIALOG_COVERCONTAINER_WIDTH - MODIFYNOVELDIALOG_CANCELCOVERICON_MAXSIZE + cancelIconSizeDiff/2,
                                    cancelIconSizeDiff/2,
                                    MODIFYNOVELDIALOG_CANCELCOVERICON_MINSIZE,
                                    MODIFYNOVELDIALOG_CANCELCOVERICON_MINSIZE);

            // m_btnCancelCover->setIcon(QIcon(":/Image/close-2.png"));
            m_btnCancelCover->setIconSize(QSize(MODIFYNOVELDIALOG_CANCELCOVERICON_MINSIZE,
                                                MODIFYNOVELDIALOG_CANCELCOVERICON_MINSIZE));
            m_btnCancelCover->setGeometry(cancelIconMinRect);
        }
    }
    return QObject::eventFilter(watched, event);
}

void ModifyNovelDialog::do_btnAddCover_clicked()
{
    // 获取新封面
    QString novelCoverPath = QFileDialog::getOpenFileName(this,"选择图片文件","","图片(*.png;*.jpg)");

    if(novelCoverPath == "") // 未选择 图片 文件
    {
        if(!m_isCoverExisted) // 原先 不存在封面
            m_btnCancelCover->setVisible(false); // 不显示 取消封面按钮
        else // 原先 存在封面
            m_btnCancelCover->setVisible(true); // 显示 取消封面按钮

        return; // 退出
    }

    // 打开 截取图片对话框
    ImageCropperDialog *digCropImg = new ImageCropperDialog;
    digCropImg->setCroppedImage(novelCoverPath);

    connect(digCropImg, &ImageCropperDialog::croppedImage_sent,
            this, &ModifyNovelDialog::do_croppedImage_sent);

    if(digCropImg->isLoadImageError()) // 加载 图片失败
    {
        delete digCropImg; // 释放 截取图片对话框内存
        digCropImg = nullptr;
        return;
    }

    // 遮罩层
    MaskFrame *maskFrm = new MaskFrame(this);
    maskFrm->setMaskedWidget(this); //设置 被遮罩的控件
    maskFrm->setUpperWidget(digCropImg); // 设置 在遮罩层上的控件

    // 设置 对话框 位置
    QPoint maskedWidgetLeftTop = this->mapToGlobal(QPoint(0, 0));
    int x = maskedWidgetLeftTop.x() + (this->width() - digCropImg->width())/2;
    int y = maskedWidgetLeftTop.y() + (this->height() - digCropImg->height())/2;

    digCropImg->move(QPoint(x, y));
    digCropImg->exec();
}

void ModifyNovelDialog::do_btnCancelCover_clicked()
{
    // 之前 是否 存在封面，存在则删除

    m_novelCoverPixmap = QPixmap(":/image/add book cover.png");

    m_isCoverExisted = false;

    m_btnAddCover->setIcon(QIcon(m_novelCoverPixmap));

    m_btnCancelCover->setVisible(false);

}

void ModifyNovelDialog::do_croppedImage_sent(QPixmap croppedPixmap)
{
    // 缩放图片
    m_novelCoverPixmap = croppedPixmap.scaled(MODIFYNOVELDIALOG_COVER_WIDTH*2,
                                              MODIFYNOVELDIALOG_COVER_HEIGHT*2);

    // 将图片 设置为 添加封面按钮的图标
    m_btnAddCover->setIcon(QIcon(m_novelCoverPixmap));

    m_isCoverExisted = true; // 封面已存在
    m_btnCancelCover->setVisible(true); // 显示 取消封面按钮
}

void ModifyNovelDialog::do_btnSure_clicked()
{
    // 不管是 新建 还是 修改，都要检查 书名是否存在
    if(m_ledNoverTitle->text().simplified() == "") // 获取书名输入框的字符串，去掉前后空格，判断是否为空串
    {
        QMessageBox::warning(this,"警告","书名不能为空");
        return;
    }

    QDir dir;
    if(!dir.exists(QDir::currentPath() + "/Config/NovelCover/default book cover.png")) // 默认 封面不存在
    {
        // 直接复制一份小说默认封面
        QFile::copy(":/image/default book cover.png", QDir::currentPath() + "/Config/NovelCover/default book cover.png");
    }

    if(m_isEdited) // 修改 小说
        editNovel();
    else // 新建 小说
        newNovel();

    // 发送 更新 小说信号
    emit updateNovels();

    this->close();
}
