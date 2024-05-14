#include "BasedDirectoryWidget.h"

BasedDirectoryWidget::BasedDirectoryWidget(QWidget *parent)
    : QTreeWidget{parent}
{
    // 设置 目录树控件 选中 拖拽属性
    this->setSelectionMode(QAbstractItemView::SingleSelection); // 单选模式
    this->setSelectionBehavior(QAbstractItemView::SelectRows); // 选择行为，选中行
    this->setDefaultDropAction(Qt::MoveAction); // 设置默认拖动行为为移动拖动
    this->setDragDropMode(QAbstractItemView::InternalMove); // 设置拖动释放模式为内部移动

    // 设置 自定义弹出菜单
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    // 设置无边框
    this->setFrameShape(QFrame::NoFrame);

    // 设置列数
    this->setColumnCount(3);

    // 设置表头
    QStringList titleHeader;
    titleHeader<< "标题" << "数量" << "索引";
    this->setHeaderLabels(titleHeader);

    // 隐藏表头
    this->setHeaderHidden(true);
    this->setColumnHidden(int(DirectoryDateColume::IndexColume), true);

    // 隐藏 水平滚动条
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 设置 水平方向固定，垂直方向延展
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);

    // 获取表头
    QHeaderView *head = this->header();
    // 设置列为固定
    head->setSectionResizeMode(int(DirectoryTextColume::TitleColume), QHeaderView::Fixed);
    head->setSectionResizeMode(int(DirectoryTextColume::CountColume),QHeaderView::Fixed);

}

// 获取 节点新标题
QString BasedDirectoryWidget::getNewTitleFromDialog(QString dialogTitle, QString orignalTitle,
                    QString placeholderText, bool &isCancelRename)
{
    // 用于接收 重命名对话框 发送的标题
    QString newItemTitle = "";

    // 新建 重命名 对话框
    RenameDirectoryItemDialog *renameDialog = new RenameDirectoryItemDialog;

    // 设置 对话框标题 和 文本输入框的初始内容
    renameDialog->setDialogTitle(dialogTitle); // 设置 对话框标题
    renameDialog->setPlaceholderText(placeholderText); // 设置 文本输入框 提示内容
    renameDialog->setLineEditTitle(orignalTitle); // 设置 文本输入框 初始内容

    // 绑定 信号和槽，点击确定，newTitleText自动更改为新值，点击取消，则还是原来的值
    connect(renameDialog,&RenameDirectoryItemDialog::newTitle_sent,this,[&](QString newTitle){
        newItemTitle = newTitle;
    });
    connect(renameDialog,&RenameDirectoryItemDialog::isCancel,this,[&](bool isCancel){
        isCancelRename = isCancel;
    });

    // 遮罩层
    MaskFrame *maskFrm = new MaskFrame(this);
    // this 是 当前目录控件
    // this->parentWidget() 是 StackedWidget栈容器
    // this->parentWidget()->parentWidget() 是 EditCatalogBar目录栏
    // this->parentWidget()->parentWidget()->parentWidget() 是 splitter分裂器
    // this->parentWidget()->parentWidget()->parentWidget()->parentWidget() 是 最顶层的容器
    QWidget *maskedWidget = this->parentWidget()->parentWidget()->parentWidget()->parentWidget();
    maskFrm->setMaskedWidget(maskedWidget); //设置 被遮罩的控件
    maskFrm->setUpperWidget(renameDialog); // 设置 在遮罩层上的控件

    // 设置 对话框 位置
    QPoint maskedWidgetLeftTop = maskedWidget->mapToGlobal(QPoint(0, 0));
    int x = maskedWidgetLeftTop.x() + (maskedWidget->width() - renameDialog->width())/2;
    int y = maskedWidgetLeftTop.y() + (maskedWidget->height() - renameDialog->height())/2;

    renameDialog->move(QPoint(x, y));
    // 显示不能用show，因为用show后，对话框和主程序是分开 同时执行，即未接收到新标题，主程序先执行，新旧标题会一样
    // 使用exec 主程序要 等待 对话框关闭才继续执行
    renameDialog->exec();


    // 返回 新标题
    return newItemTitle;
}

void BasedDirectoryWidget::renameItem(QTreeWidgetItem *item)
{
    // 获取节点类型
    int itemType = item->type();

    // 根据 节点类型 设置 对话框的标题 和 文本输入框提示内容
    QString dialogTitle = (itemType == int(DirectoryItemType::VolumeItem)? "重命名分卷":"重命名章节");
    QString placeholderText = (itemType == int(DirectoryItemType::VolumeItem)? "请输入分卷名":"请输入章节名");

    // 获取 初始 标题
    QString orignalTitelText = item->text(int(DirectoryTextColume::TitleColume));
    QString newTitleText = orignalTitelText; // 用于接收 新标题，防止点击取消

    // 是否取消新建 重命名
    bool isCancelRename = false;

    // 获取 新分卷的标题
    newTitleText = getNewTitleFromDialog(dialogTitle,orignalTitelText,placeholderText,isCancelRename);

    if(isCancelRename || (newTitleText == orignalTitelText)) // 点击了取消键，或者是 未修改标题，直接确定
        return; // 直接退出

    // 更新 节点 显示的标题
    item->setText(int(DirectoryTextColume::TitleColume),newTitleText);

    // 把 重命名的节点 的ID，新标题 发送出去
    emit chapterItem_renamed(item->data(int(DirectoryDateColume::IDColume),Qt::UserRole).toString(),newTitleText);
}

// 将 章节导出成 TXT格式
void BasedDirectoryWidget::exportChapterItemAsTXT(QTreeWidgetItem *chapterItem, QString savedFilePath)
{
    // 获取 章节ID、标题
    QString exportChapterID = chapterItem->data(int(DirectoryDateColume::IDColume),Qt::UserRole).toString();
    QString exportChapterTitle = chapterItem->text(int(DirectoryTextColume::TitleColume));

    // 要导出的章节 对应 文件路径
    QString chapterFilePath = QDir::currentPath() + "/Config/LocalChapters/" + exportChapterID + ".txt";

    // 要导出的 章节 是文本框正在编辑的章节，以 当前文本框的内容 为准
    if(exportChapterID == m_chapterIDSent)
    {
        // 发送信号 保存 当前打开着 章节
        emit saveBeingOpenedChapter();

        // 等待 文本框 响应信号 完成保存文本框内容
        // ???????????????????????????????????

        // 打开已经保存的文件，获取文件内容
        QFile file(chapterFilePath); // 通过文件路径创建文件对象
        file.open(QFile::ReadOnly|QIODevice::Text); // 文件打开方式
        QString chapterContent = file.readAll(); // 获取 章节文件中的 全部字符
        file.close(); // 关闭文件

        // 新建 文件
        QFile chapterFile(savedFilePath); // 文件操作对象
        chapterFile.open(QIODevice::WriteOnly|QIODevice::Text); // 创建文件
        // 使用文本流 写入文件
        QTextStream writeStream(&chapterFile);
        writeStream << exportChapterTitle << "\n" << chapterContent << "\n\n";

        // 关闭 文件
        chapterFile.close();
    }
    else // 要导出的 章节不是正在编辑的章节
        QFile::copy(chapterFilePath, savedFilePath); // 直接复制一份 章节对应主文件
}

void BasedDirectoryWidget::updateDirectoryColumnWidth()
{
    // 设置 列宽
    if(this->width() < 210) // 控件宽度 过小，使用比例
    {
        this->setColumnWidth(int(DirectoryTextColume::TitleColume),int(0.66*this->width())); // this->width()-70
        this->setColumnWidth(int(DirectoryTextColume::CountColume),int(0.24*this->width())); // 50
    }
    else // 控件宽度 过大，使用固定
    {
        this->setColumnWidth(int(DirectoryTextColume::TitleColume),this->width()-70);
        this->setColumnWidth(int(DirectoryTextColume::CountColume),50);
    }
}
// 清除 已发送的章节ID
void BasedDirectoryWidget::clearChapterIDSent()
{
    m_chapterIDSent = "";
}
