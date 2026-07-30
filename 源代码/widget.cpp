#ifdef _MSC_VER
#pragma comment(lib, "winmm.lib")  // 链接系统音频库
#else
#endif
#include "widget.h"
#include"form.h"
#include "ui_widget.h"
#include <QMouseEvent>
#include <QScreen>
#include <QApplication>
#include <QAction>
#include <QMessageBox>
#include<QPropertyAnimation>
#include<QVBoxLayout>
#include<QFile>
#include<QByteArray>
#include<QTemporaryFile>
#include<QDir>
#include<filesystem>
#include <fstream>
#include<QResource>
#include <windows.h>  // Windows 系统头文件
#include <QApplication>
#include <QProcess>
#include <QMessageBox>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , petlable(nullptr)
    , xianzhi(nullptr)
    , timer(nullptr)
    ,m_trayIcon(nullptr)
    ,m_trayMenu(nullptr)
{
    ui->setupUi(this);
    tc=new taici();
    zi=new zidingyi();
    std::filesystem::path a=std::filesystem::path(std::filesystem::current_path()/"zidingyiwenjian.txt");
    std::filesystem::path b=std::filesystem::path(std::filesystem::current_path()/"taiciwenjian.txt");
    taiciwenjian=b;
    zidingyiwenjian=a;
    initzidingyi();
    extractMp3FromQrc(QString::fromStdString(yvshemusic1.string()));
    chushimusic[0]=QString::fromStdString(yvshemusic1.string());
    xianzhicun=new QMovie(QString::fromStdString(danjidonghua.string()));//注意此处闲置动画为单机动画
    danjicun=new QMovie(QString::fromStdString(jingzhidonghua.string()));
    taiciinit();
    chushichangdu=2;
    form=new Form();
    musicgui=new musicGUI();
    std::filesystem::path current=std::filesystem::path(std::filesystem::current_path()/"shezhiwenjian.txt");
    shezhiwenjian=current;
    initbool();
    std::filesystem::path current1=std::filesystem::path(std::filesystem::current_path()/"music");
    bendimusic=current1;
    if(!std::filesystem::exists(bendimusic))
        std::filesystem::create_directory(bendimusic);
    connect(form,&Form::shezhisignal,this,&Widget::shezhibaocun);
    connect(musicgui,&musicGUI::geqvbianhua,this,&Widget::changeplayingcurrent);
    connect(musicgui,&musicGUI::ifstop,this,&Widget::changeifstop);
    connect(musicgui,&musicGUI::moshibianhua,this,&Widget::changebofangmoshi);
    connect(musicgui,&musicGUI::ailiheti,this,&Widget::ailiheti);
    connect(zi,&zidingyi::shezhixinghao,this,&Widget::zidingyibaocun);
    connect(form,&Form::xianshi,this,&Widget::showzidingyi);
    connect(zi,&zidingyi::taicishezhi,this,&Widget::taicixianshi);
    connect(tc,&taici::taicichuandi,this,&Widget::taicibaocun);
    qiehuan=false;
    ismove=false;    // 是否正在拖拽
    ispress=false;
    isdanji=false;
    isplaying=false;
    isstop=false;
    chushihua=true;
    ifailiheti=true;
    zidongyidong=false;
    bofangmoshi=0;
    playingcurrent=0;
    // 初始化各模块（顺序可调整，窗口需先初始化）
    initWindow();
    initAnimation();
    initTimer();
    initTrayIcon();
    musicgui->ailihetisignal();
    chushihua=false;
    // 添加播放状态检查定时器（用于手动循环）
    playStatusTimer = new QTimer(this);
    playStatusTimer->setInterval(200); // 每500ms检查一次
    connect(playStatusTimer, &QTimer::timeout, this, &Widget::checkPlayStatus);
    playStatusTimer->start();
    if(playmodel==0)
    {
        if(playwav)
            play0(playingcurrent);
        musicgui->init(playingcurrent,playmodel,isstop,bofangmoshi,2,bofangqibeijing);
    }
    if(!useownwenjianjia1)
        loadMusicFromFolder(bendimusic);
    else
        loadMusicFromFolder(zidingwenjianjiadizhi1);
    if(playmodel==1)
    {
        play1(playingcurrent);
        musicgui->init(playingcurrent,playmodel,isstop,bofangmoshi,changdu,bofangqibeijing);
    }
    if(playmodel==3)
        musicgui->init(playingcurrent,playmodel,isstop,bofangmoshi,changdu,bofangqibeijing);
    chushimusic[1]=QString::fromStdString(yvshemusic2.string());
    extractMp3FromQrc(QString::fromStdString(yvshemusic2.string()));
    if(xianshibofangqi1)
        musicgui->show();
    else
    {
        showbofangqiAction->setChecked(false);
        showbofangqiactionhanshu();
    }
    ailiheti1();
}

Widget::~Widget()
{
    if (xianzhi) {
        xianzhi->stop();
        delete xianzhi;
    }
    if (yidong) {
        yidong->stop();
        delete yidong;
    }
    if (danji) {
        danji->stop();
        delete danji;
    }

    // 释放标签和文本框
    delete petlable;
    if (text) {
        QLabel *textContent = text->property("textContent").value<QLabel*>();
        delete textContent;
        delete text;
    }


    // 释放托盘相关对象
    delete m_trayMenu;
    delete m_trayIcon;

    // 释放定时器
    delete timer;
    delete ui;
    stopAudio();
    delete tc;
    delete zi;
    delete form;
    delete musicgui;
}

void Widget::initbool()
{
    std::ifstream ss(shezhiwenjian);
    if(!ss.is_open())
    {
        ss.close();
        ailimove=true;
        playwav=true;
        ifshow=true;
        xianshibofangqi1=true;
        useownwenjianjia1=false;
        playmodel=0;
        zidingwenjianjiadizhi1=std::filesystem::path(std::filesystem::current_path()/"music");
        bendimusic=zidingwenjianjiadizhi1;
        std::ofstream gg(shezhiwenjian);
        gg<<1<<" "<<1<<" "<<1<<" "<<1<<" "<<0<<" "<<0<<" "<<bendimusic;
        gg.close();
        form->shezhislot(ailimove,ifshow,playwav,xianshibofangqi1,useownwenjianjia1,playmodel,zidingwenjianjiadizhi1,shezhibeijing);
        return;
    }else{
        ss>>ailimove>>playwav>>ifshow>>xianshibofangqi1>>useownwenjianjia1>>playmodel>>zidingwenjianjiadizhi1;//用0表示false用1表示true
        form->shezhislot(ailimove,ifshow,playwav,xianshibofangqi1,useownwenjianjia1,playmodel,zidingwenjianjiadizhi1,shezhibeijing);
        if(ifailiheti)
        {zhongjiantai=ailimove;
            ailimove=false;}
    }
    ss.close();
}




void Widget::initWindow()   // 初始化窗口特性（无边框、置顶等）
{
    // 获取屏幕尺寸（用于限制宠物移动范围）
    QScreen *screen = QApplication::primaryScreen();
    kuan = screen->geometry().width();
    gao = screen->geometry().height();

    // 窗口特性设置
    setWindowFlags(
        Qt::FramelessWindowHint        // 无边框
        | Qt::WindowStaysOnTopHint     // 置顶显示
        | Qt::Tool                     // 工具窗口（无任务栏图标）
        | Qt::X11BypassWindowManagerHint // 兼容 Linux 窗口管理器
        );
    setAttribute(Qt::WA_TranslucentBackground); // 背景透明（显示 GIF 透明区域）
    setFixedSize(target, target); // 固定宠物大小（根据图片尺寸调整）
    text = new QWidget(this);  // 用 QWidget 做文本容器（替代 QLabel）
    text->setVisible(false);
    text->setWindowFlags(
        Qt::FramelessWindowHint    // 无边框
        | Qt::WindowStaysOnTopHint // 置顶（和宠物窗口同层级）
        | Qt::Tool                 // 工具窗口（无任务栏图标）
        | Qt::WindowDoesNotAcceptFocus // 不接受焦点（避免遮挡操作）
        );
    text->setAttribute(Qt::WA_TranslucentBackground); // 背景透明

    // 在文本窗口内添加 QLabel 显示文本（负责文字渲染）
    QLabel *textContent = new QLabel(text);
    QString style=R"(
        background-color:#ffffff;
        color: %1;
        padding: 5px 10px;
        border-radius: 4px;
        font-size: 12px;
        border: 1px solid %2;
    )";
    style = style.arg(color.name()).arg(color.name());
    textContent->setStyleSheet(style);
    textContent->setWordWrap(true);
    // 用布局让 QLabel 适配 QWidget 大小
    QVBoxLayout *layout = new QVBoxLayout(text);
    layout->setContentsMargins(0, 0, 0, 0); // 去除布局边距
    layout->addWidget(textContent);
    // 存储文本内容标签（供后续修改文字）
    text->setProperty("textContent", QVariant::fromValue(textContent));
    // 初始化文本隐藏定时器（全局唯一）
    textHideTimer = new QTimer(this);
    textHideTimer->setSingleShot(true); // 单次触发
    // 绑定隐藏文本窗口的槽函数
    connect(textHideTimer, &QTimer::timeout, text, &QWidget::hide);
}
void Widget::initAnimation() // 初始化动画（GIF/序列帧）
{
    petlable = new QLabel(this);
    petlable->setFixedSize(size()); // 标签大小与窗口一致
    petlable->setScaledContents(true);
    // 加载闲置状态 GIF（资源路径对应 resources.qrc 中的 prefix）
    xianzhi =danjicun;
    yidong=new QMovie(QString::fromStdString(tuodongdonghua.string()));
    danji=xianzhicun;
    if (text) {
        text->raise();
    }
    if (xianzhi->isValid()) {
        petlable->setMovie(xianzhi);
        xianzhi->start(); // 启动动画
    } else {
        // 若 GIF 加载失败，显示占位文本
        petlable->setText("宠物加载失败");
        petlable->setAlignment(Qt::AlignCenter);
    }
}
void Widget::initTimer()   // 初始化定时器（自主移动、动画切换）
{
    timer = new QTimer(this);
    timer->setInterval(5000); // 3000ms = 3秒
    connect(timer, &QTimer::timeout, this, &Widget::onAutoMove);
    timer->start(); // 启动定时器
}
void Widget::initTrayIcon()  // 初始化系统托盘（隐藏/退出菜单）
{
    // 创建托盘菜单
    m_trayMenu = new QMenu(this);
    showAction = new QAction("显示爱莉", this);
    showAction->setCheckable(true);
    showAction->setChecked(true);
    showbofangqiAction = new QAction("显示播放器", this);
    showbofangqiAction->setCheckable(true);
    if(xianshibofangqi1)showbofangqiAction->setChecked(true);
    else showbofangqiAction->setChecked(false);
    moveAction = new QAction("爱莉移动", this);
    moveAction->setCheckable(true);
    if(ailimove&&!ifailiheti)moveAction->setChecked(true);
    else moveAction->setChecked(false);
    sayAction = new QAction("爱莉说话", this);
    sayAction->setCheckable(true);
    if(ifshow)sayAction->setChecked(true);
    else sayAction->setChecked(false);
    playAction = new QAction("播放音乐", this);
    playAction->setCheckable(true);
    if(playwav)playAction->setChecked(true);
    else playAction->setChecked(false);
    chushiAction = new QAction("初始音乐", this);
    chushiAction->setCheckable(true);
    bendiAction = new QAction("本地音乐", this);
    bendiAction->setCheckable(true);
    wangyiyunAction = new QAction("软件音乐", this);
    wangyiyunAction->setCheckable(true);
    if(playmodel==0)chushiAction->setChecked(true);
    if(playmodel==1)bendiAction->setChecked(true);
    if(playmodel==3)wangyiyunAction->setChecked(true);
    shezhiAction = new QAction("设置", this);
    exitAction = new QAction("退出", this);
    hetiAction = new QAction("爱莉合体", this);
    hetiAction->setCheckable(true);
    if(ifailiheti)hetiAction->setChecked(true);
    else hetiAction->setChecked(false);
    m_trayMenu->addAction(showAction);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(showbofangqiAction);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(moveAction);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(hetiAction);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(sayAction);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(playAction);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(chushiAction);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(bendiAction);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(wangyiyunAction);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(shezhiAction);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(exitAction);



    // 创建托盘图标
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(QIcon(":/tubiao.png")); // 加载托盘图标
    m_trayIcon->setContextMenu(m_trayMenu);            // 绑定右键菜单
    m_trayIcon->setToolTip("爱莉爱莉爱");                 // 鼠标悬停提示
    m_trayIcon->show();                                // 显示托盘图标

    // 绑定菜单槽函数
    connect(exitAction, &QAction::triggered, this, &Widget::onExit);
    connect(showAction, &QAction::triggered, this, &Widget::onShowPet);
    connect(moveAction, &QAction::triggered, this, &Widget::ailiyidong);
    connect(sayAction, &QAction::triggered, this, &Widget::ailisay);
    connect(playAction, &QAction::triggered, this, &Widget::ailiplay);
    // 双击托盘图标显示宠物
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &Widget::onTrayIconActivated);
    connect(shezhiAction, &QAction::triggered, this, &Widget::shezhichuangkou);
    connect(chushiAction, &QAction::triggered, this, &Widget::chushimoshi);
    connect(bendiAction, &QAction::triggered, this, &Widget::bendimoshi);
    connect(hetiAction, &QAction::triggered, this, &Widget::ailiheti1);
    connect(wangyiyunAction, &QAction::triggered, this, &Widget::wangyiyun);
    connect(showbofangqiAction, &QAction::triggered, this, &Widget::showbofangqiactionhanshu);
}

void Widget::mousePressEvent(QMouseEvent *event)    // 鼠标按下（拖拽/点击响应）
{
    if(qiehuan)return;
    if (event->button() == Qt::LeftButton) {
        danjidian = event->pos();  // 保存点击在窗口内的相对位置
        ispress = true;     // 标记左键已按下
    }
    if (event->button() == Qt::LeftButton) {
        // 记录拖拽起点（窗口左上角到鼠标的偏移）
        dian = event->globalPos() - frameGeometry().topLeft();
        ismove = true;
    } else if (event->button() == Qt::RightButton) {
        // 右键显示托盘菜单（可选：也可自定义右键菜单）
        m_trayMenu->exec(event->globalPos());
    }
    QWidget::mousePressEvent(event);
}
void Widget::mouseMoveEvent(QMouseEvent *event)     // 鼠标移动（窗口拖拽）
{
    if(qiehuan)return;
    if(ifailiheti)return;
    if (ismove && (event->buttons() & Qt::LeftButton)) {
        // 计算新位置（鼠标全局坐标 - 偏移量）
        petlable->setMovie(yidong);
        yidong->start(); // 启动动画
        int suiji=rand()%(6);
        if (!yidongyv[suiji].isEmpty() && suiji <=5) {
            showText(yidongyv[suiji], 3000);
        }
        move(event->globalPos() - dian);
    }
    QWidget::mouseMoveEvent(event);
}
void Widget::mouseReleaseEvent(QMouseEvent *event)  // 鼠标释放
{
    if(qiehuan)return;
    static qint64 lastShowTime = 0;
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (currentTime - lastShowTime < 1000) {
        return; // 1秒内忽略新请求
    }
    lastShowTime = currentTime;
    if (event->button() == Qt::LeftButton && ispress) {
        // 计算按下到释放的位置差（避免误判拖拽为单击）
        int distance = (event->pos() -danjidian).manhattanLength();
        if (distance < 5) {  // 距离小于5像素视为单击（可调整阈值）
            onSingleClick(event->pos());  // 触发单击处理函数
        }
        if(distance>=5&&ifailiheti)
            onSingleClick(event->pos());
        ispress = false;  // 重置标记
        petlable->setMovie(xianzhi);
        xianzhi->start(); // 启动动画
        return;
    }
    ismove = false;
    petlable->setMovie(xianzhi);
    xianzhi->start(); // 启动动画
    QWidget::mouseReleaseEvent(event);
}

// 3. 单击处理函数（实现具体交互逻辑）
void Widget::onSingleClick(const QPoint &clickPos)
{
    if(qiehuan)return;
    danji=danjicun;
    xianzhi=xianzhicun;
    petlable->setMovie(xianzhi);
    xianzhi->start();
    // 示例1：切换到点击动画（1秒后恢复闲置）
    if (danji && danji->isValid()&&ispress) {
        QTimer::singleShot(1000, this, [=]() {

            petlable->setMovie(danji);
            danji->start();
        });
        int suiji=rand()%(6);
        if (!danjiyv[suiji].isEmpty() && suiji <=5) {
            showText(danjiyv[suiji], 3000);
        }
        ispress = false;
        ismove=false;
    }
}
// 定时器槽函数（自主移动、动画切换等）
void Widget::onAutoMove()
{

    if (!isVisible()) return;
    int kai=rand()%8;
    if(kai<=2)
    {
        int suiji=rand()%(6);
        if (!xianzhiyv[suiji].isEmpty() && suiji <=5) {
            showText(xianzhiyv[suiji], 3000);
        }
    };
    if(!ailimove) return;
    if(ismove) return;
    if(ifailiheti)return;

    // 1. 随机生成目标位置（与之前相同）
    int newX = rand() % (kuan - width() - 40) + 20;
    int newY = rand() % (gao - height() - 100) + 20;

    // 2. 创建属性动画（目标对象：this 窗口，动画属性：pos 位置）
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");

    connect(animation, &QPropertyAnimation::finished, this, &Widget::onAnimationFinished);
    // 3. 设置动画参数
    animation->setDuration(1000);  // 动画持续时间：1000ms = 1秒
    animation->setStartValue(pos());  // 起始位置：当前位置
    animation->setEndValue(QPoint(newX, newY));  // 目标位置：随机生成的坐标

    // 4. 设置动画曲线（可选，控制速度变化）
    animation->setEasingCurve(QEasingCurve::OutCubic);  // 先快后慢，更自然

    // 5. 启动动画（动画结束后自动释放内存）
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    zidongyidong=true;
}
// 系统托盘菜单槽函数
void Widget::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick) {
        onShowPet();
        showAction->setChecked(true);
    }
}
void Widget::onShowPet()   // 显示宠物
{
    if(showAction->isChecked())
    {
        showNormal(); // 恢复显示
        raise(); // 置顶显示
    }else
        hide();
}
void Widget::onExit()      // 退出程序
{
    m_trayIcon->hide(); // 隐藏托盘图标
    tc->hide();
    zi->hide();
    form->hide();
    qApp->quit();       // 退出应用
}

void Widget::ailiyidong()// 1秒内不重复触发（可选）
{
    if(ifailiheti){
        moveAction->setChecked(false);
        return;
    }
    if(moveAction->isChecked())
        ailimove=true;
    else
        ailimove=false;
}

// 添加显示文本的函数（外部可调用）
void Widget::showText(const QString &text1, int jiange)
{
    if(!ifshow)
        return;
    // 1秒内不重复触发（可选）
    if(text1.isEmpty())return;
    static qint64 lastShowTime = 0;
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (currentTime - lastShowTime < 3000) {
        return; // 1秒内忽略新请求
    }
    lastShowTime = currentTime;
    if (text1.isEmpty() || !text) {
        text->hide();
        return;
    }
    textHideTimer->stop();
    // 从文本窗口中取出 QLabel 并设置文字
    QLabel *textContent = text->property("textContent").value<QLabel*>();
    textContent->setText(text1);
    textContent->adjustSize(); // 让文字标签适配内容
    text->resize(textContent->size()); // 让文本窗口和标签大小一致

    // -------------------------- 计算文本窗口位置（宠物上方居中） --------------------------
    updateTextPosition();
    text->show();
    text->raise(); // 确保在最顶层

    // 定时隐藏
    QTimer::singleShot(jiange, text, &QWidget::hide);
    textHideTimer->start(jiange);
}

// 监听宠物窗口移动，同步更新文本框位置
void Widget::moveEvent(QMoveEvent *event)
{
    // 调用父类方法，确保正常事件处理
    QWidget::moveEvent(event);

    // 如果文本框正在显示，更新其位置
    if (text && text->isVisible()) {
        updateTextPosition(); // 自定义函数：重新计算文本框位置
    }
}

// 重新计算文本框位置（复用showText中的位置逻辑）
void Widget::updateTextPosition()
{
    if(!ifshow)return;
    if (!text) return;

    // 获取文本内容标签（用于获取尺寸）
    QLabel *textContent = text->property("textContent").value<QLabel*>();
    if (!textContent) return;

    // 计算新位置（与宠物保持相对位置）
    QPoint petGlobalPos = this->mapToGlobal(QPoint(0, 0));
    int textGlobalX = petGlobalPos.x() + (this->width() - text->width()) / 2;
    int textGlobalY = petGlobalPos.y() - text->height() - 10;

    // 屏幕边界检测（避免超出屏幕）
    QRect screenRect = QGuiApplication::primaryScreen()->availableGeometry();
    if (textGlobalX < screenRect.left()) textGlobalX = screenRect.left();
    if (textGlobalX + text->width() > screenRect.right()) {
        textGlobalX = screenRect.right() - text->width();
    }
    if (textGlobalY < screenRect.top()) {
        textGlobalY = petGlobalPos.y() + this->height() + 10;
    }

    // 更新文本框位置
    text->move(textGlobalX, textGlobalY);
}

void Widget::shezhichuangkou()
{
    form->init();
    form->show();
}
void Widget::shezhibaocun(bool ailimove1, bool playwav1, bool ifshow1,bool xianshibofangqi,bool useownwenjianjia,int bofangmoshi,std::filesystem::path zidingwenjianjiadizhi)
{
    if(ifailiheti)
        zhongjiantai=ailimove1;
    else
    {
        ailimove=ailimove1;
        if(ailimove&&!ifailiheti)moveAction->setChecked(true);
        else moveAction->setChecked(false);
        ailiyidong();
    };
    playwav=playwav1;
    if(playwav)playAction->setChecked(true);
    else playAction->setChecked(false);
    ailiplay();
    ifshow=ifshow1;
    if(ifshow)sayAction->setChecked(true);
    else sayAction->setChecked(false);
    ailisay();
    playmodel=bofangmoshi;
    if(playmodel==0)
        chushimoshi();
    if(playmodel==1)
        bendimoshi();
    if(playmodel==3)
        wangyiyun();
    xianshibofangqi1=xianshibofangqi;
    if(xianshibofangqi1)
        musicgui->show();
    else
        musicgui->hide();
    useownwenjianjia1=useownwenjianjia;
    zidingwenjianjiadizhi1=zidingwenjianjiadizhi;
    std::ofstream ss(shezhiwenjian);
    ss<<ailimove<<" "<<playwav<<" "<<ifshow<<" "<<xianshibofangqi1<<" "<<useownwenjianjia1<<" "<<bofangmoshi<<" "<<zidingwenjianjiadizhi1;
    ss.close();
    onRestartClicked();
}

void Widget::loadMusicFromFolder(const std::filesystem::path& folderPath)
{
    musicList.clear();  // 清空列表
    playingcurrent = -1;

    QDir dir(folderPath);
    if (!dir.exists()) {  // 检查文件夹是否存在
        QMessageBox::warning(this, "错误", "文件夹不存在");
        return;
    }

    // 筛选音频文件（支持的格式可扩展）
    QStringList filters;
    filters <<"*.wav";
    dir.setNameFilters(filters);

    // 获取所有音频文件路径
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    foreach (QFileInfo fileInfo, fileInfoList) {
        musicList.append(fileInfo.absoluteFilePath());  // 存储绝对路径
    }

    // 显示加载结果
    if (musicList.isEmpty()) {
        qDebug() << "不存在音频文件";
    } else {
        playingcurrent  = 0;
    }
    changdu=musicList.size();
}

void Widget::changeplayingcurrent(int a)
{

    playingcurrent=a;
    stopAudio();
    if(playmodel==0)
        play0(playingcurrent);
    else
        play1(playingcurrent);
}
void Widget::changebofangmoshi(int a)
{
    bofangmoshi=a;
}
void Widget::changeifstop(bool a)
{
    isstop=a;
    if(a)
        pause();
    else
        if(playmodel==1)
            play1(playingcurrent);
        else
            play0(playingcurrent);
}

// 从资源文件提取MP3到临时文件
QString Widget::extractMp3FromQrc(const QString &qrcPath)
{
    qDebug() << "开始提取WAV资源：" << qrcPath;

    // 检查资源是否存在
    if (!QFile::exists(qrcPath)) {
        qDebug() << "错误：WAV资源不存在！";
        return "";
    }

    // 创建临时WAV文件（系统临时目录）
    QTemporaryFile tempFile(QDir::tempPath() + "/audio_XXXXXX.wav");
    tempFile.setAutoRemove(false);  // 手动控制删除

    // 打开临时文件并写入资源数据
    if (tempFile.open()) {
        QFile resFile(qrcPath);
        if (resFile.open(QIODevice::ReadOnly)) {
            QByteArray wavData = resFile.readAll();
            qint64 written = tempFile.write(wavData);
            resFile.close();
            tempFile.close();

            if (written == wavData.size()) {
                qDebug() << "WAV提取成功，临时路径：" << tempFile.fileName();
                return tempFile.fileName();
            } else {
                qDebug() << "错误：写入临时文件不完整！";
                return "";
            }
        } else {
            qDebug() << "错误：无法打开WAV资源文件！";
            return "";
        }
    } else {
        qDebug() << "错误：创建临时文件失败！";
        return "";
    }
}

// 播放按钮
void Widget::play0(int wei)
{
    if(!playwav)return;
    if(!isplaying)
    {
        mciSendStringW(L"close mywav", NULL, 0, NULL);
        QString qrcPath = chushimusic[wei];
        tempMp3Path = extractMp3FromQrc(qrcPath);

        if (tempMp3Path.isEmpty()) {
            return;
        }

        std::wstring wFilePath = tempMp3Path.toStdWString();
        // 关键：使用waveaudio类型（WAV原生支持）
        std::wstring openCommand = L"open \"" + wFilePath + L"\" type waveaudio alias mywav";

        MCIERROR error = mciSendStringW(openCommand.c_str(), NULL, 0, NULL);
        if (error != 0) {
            qDebug() << "播放失败！路径：" << tempMp3Path << "错误码：" << error;
            QFile::remove(tempMp3Path);
            tempMp3Path.clear();
            return;
        }
        mciSendStringW(L"seek mywav to 0", NULL, 0, NULL);

        mciSendStringW(L"play mywav", NULL, 0, NULL);
        isplaying = true;
    }else{
        mciSendStringW(L"resume mywav", NULL, 0, NULL);
        isstop=false;
        isplaying=true;
    }
}

void Widget::play1(int wei)
{
    if(!playwav)return;
    if(playmodel==0)return;
    if(!isplaying)
    {
        mciSendStringW(L"close mywav", NULL, 0, NULL);
        std::wstring wFilePath =musicList[wei].toStdWString();
        // 关键：使用waveaudio类型（WAV原生支持）
        std::wstring openCommand = L"open \"" + wFilePath + L"\" type waveaudio alias mywav";

        MCIERROR error = mciSendStringW(openCommand.c_str(), NULL, 0, NULL);
        if (error != 0) {
            qDebug() << "播放失败！路径：" << wFilePath << "错误码：" << error;
            return;
        }
        mciSendStringW(L"seek mywav to 0", NULL, 0, NULL);
        mciSendStringW(L"play mywav", NULL, 0, NULL);
        isplaying = true;
    }else{
        mciSendStringW(L"resume mywav", NULL, 0, NULL);
        isstop=false;
        isplaying=true;
    }
}
// 暂停按钮
void Widget::pause()
{
    if (isplaying) {
        mciSendStringW(L"pause mywav", NULL, 0, NULL);
        isstop=true;
    }
}

// 停止按钮
void Widget::stopAudio()
{
    mciSendStringW(L"stop mywav", NULL, 0, NULL);
    mciSendStringW(L"close mywav", NULL, 0, NULL);
    isplaying = false;

    // 清理临时文件
    if (!tempMp3Path.isEmpty()&&playmodel==0) {
        QFile::remove(tempMp3Path);
        tempMp3Path.clear();
    }
}

void Widget::ailisay()
{
    if(sayAction->isChecked())
        ifshow=true;
    else
        ifshow=false;
}
void  Widget::ailiplay()
{
    if(playAction->isChecked())
    {
        playwav=true;
        isstop=false;
        if(playmodel==0)
        {
            play0(playingcurrent);
            musicgui->init(playingcurrent,playmodel,isstop,bofangmoshi,2,bofangqibeijing);
        }
        else
        {
            play1(playingcurrent);
            musicgui->init(playingcurrent,playmodel,isstop,bofangmoshi,changdu,bofangqibeijing);
        };

    }else{
        stopAudio();
        playwav=false;
        isstop=true;
        if(playmodel==0)
            musicgui->init(playingcurrent,playmodel,isstop,bofangmoshi,2,bofangqibeijing);
        else
            musicgui->init(playingcurrent,playmodel,isstop,bofangmoshi,changdu,bofangqibeijing);
    }

}

// 3. 实现播放状态检查，完成后自动重启（核心手动循环逻辑）
void Widget::checkPlayStatus()
{
    if(!isplaying)return;

    // 查询当前播放状态
    wchar_t statusBuf[256] = {0};
    MCIERROR statusErr = mciSendStringW(
        L"status mywav mode",  // 查询模式：stopped/playing/paused
        statusBuf,
        sizeof(statusBuf)/sizeof(wchar_t),
        NULL
        );

    if (statusErr != 0) {
        qDebug() << "查询状态失败，错误码：" << statusErr;
        return;
    }

    // 如果状态为"stopped"（播放完成），重新启动播放
    if (wcscmp(statusBuf, L"stopped") == 0) {
        qDebug() << "播放完成，重新开始...";
        stopAudio();
        if(bofangmoshi==1)
        {
            if(playmodel==1)
            {
                if(playingcurrent==musicList.size()-1)
                    playingcurrent=0;
                else
                    playingcurrent++;
            }else{
                if(playingcurrent==1)
                    playingcurrent=0;
                else
                    playingcurrent++;
            }
            if(playmodel==0)
                play0(playingcurrent);
            else
                play1(playingcurrent);
        }else if(bofangmoshi==2)
        {
            if(playmodel==0)
                play0(playingcurrent);
            else
                play1(playingcurrent);
        }
        else if(bofangmoshi==0)
        {
            if(playmodel==0)
            {
                int suiji=rand()%(2);
                playingcurrent=suiji;
                play0(playingcurrent);
            }else{
                int suiji=rand()%(changdu);
                playingcurrent=suiji;
                play1(playingcurrent);
            };
        }

    }
}

void Widget::chushimoshi()
{
    isstop=false;
    if(!chushiAction->isChecked())
    {
        bendiAction->setChecked(false);
        chushiAction->setChecked(true);
        wangyiyunAction->setChecked(false);
        return;
    }
    playmodel=0;
    bendiAction->setChecked(false);
    wangyiyunAction->setChecked(false);
    playingcurrent=0;
    musicgui->init(playingcurrent,playmodel,isstop,bofangmoshi,2,bofangqibeijing);
    stopAudio();
    play0(playingcurrent);
    qiehuan=true;
    QTimer::singleShot(500, [this]() {
        qiehuan=false;
    });
}


void Widget::bendimoshi()
{
    isstop=false;
    if(!bendiAction->isChecked())
    {
        bendiAction->setChecked(true);
        chushiAction->setChecked(false);
        wangyiyunAction->setChecked(false);
        return;
    }
    if(changdu==0)
    {
        showText("未找到音频文件",300);
        chushiAction->setChecked(false);
        return;
    }
    playmodel=1;
    chushiAction->setChecked(false);
    wangyiyunAction->setChecked(false);
    stopAudio();
    musicgui->init(playingcurrent,playmodel,isstop,bofangmoshi,changdu,bofangqibeijing);
    play1(playingcurrent);
    qiehuan=true;
    QTimer::singleShot(500, [this]() {
        qiehuan=false;
    });
}

void Widget::ailiheti(QPoint a,int wid,int hig)
{
    if(!chushihua&&ailimove)return;
    if(!chushihua&&!ifailiheti)return;
    int x=a.x()-this->width()/2+wid/2;
    int y=a.y()-hig-20-(h-50);
    move(x,y);
}

void Widget::ailiheti1()
{
    if(!xianshibofangqi1)
    {
        hetiAction->setChecked(false);
        return;
    }
    if(hetiAction->isChecked())
    {
        zhongjiantai=ailimove;
        moveAction->setChecked(false);
        ailimove=false;
        ifailiheti=true;
        if(!zidongyidong)
            musicgui->ailihetisignal();
    }else{
        ailimove=zhongjiantai;
        if(ailimove)moveAction->setChecked(true);
        else moveAction->setChecked(false);
        ifailiheti=false;
    }
}

void Widget::wangyiyun()
{
    isstop=false;
    if(!wangyiyunAction->isChecked())
    {
        bendiAction->setChecked(false);
        chushiAction->setChecked(false);
        wangyiyunAction->setChecked(true);
        return;
    }
    stopAudio();
    playmodel=3;
    chushiAction->setChecked(false);
    bendiAction->setChecked(false);
    musicgui->init(playingcurrent,playmodel,isstop,bofangmoshi,changdu,bofangqibeijing);
    qiehuan=true;
    QTimer::singleShot(1000, [this]() {
        qiehuan=false;
    });
}

void Widget::hidebofangqi()
{
    musicgui->hide();
    ifailiheti=false;
}

void Widget::xianshibofangqi()
{
    musicgui->show();
}

void Widget::showbofangqiactionhanshu()//存在逻辑问题
{
    if(showbofangqiAction->isChecked())
    {
        xianshibofangqi1=true;
        ifailiheti=hetizhongjiantai;
        if(!ifailiheti)
            ailimove=zhongjiantai;
        if(ifailiheti)
        {
            hetiAction->setChecked(true);
            if(!zidongyidong)
                ailiheti1();
        };
        musicgui->show();
    }else{
        xianshibofangqi1=false;
        hetizhongjiantai=ifailiheti;
        ifailiheti=false;
        hetiAction->setChecked(false);
        ailimove=zhongjiantai;
        if(zhongjiantai)
            moveAction->setChecked(true);
        musicgui->hide();
    }
}


void Widget::onAnimationFinished() // 动画结束后执行的逻辑ads
{
    zidongyidong=false;
    if(ifailiheti)
        ailiheti1();
}

void Widget::zidingyibaocun(std::filesystem::path shezhibeijing1,
                            std::filesystem::path bofangqibeijing1,
                            std::filesystem::path yvshemusic11,
                            std::filesystem::path yvshemusic21,
                            std::filesystem::path jingzhidonghua1,
                            std::filesystem::path danjidonghua1,
                            std::filesystem::path tuodongdonghua1,
                            QColor color1,int target1,int h1)
{
    shezhibeijing=shezhibeijing1;
    bofangqibeijing=bofangqibeijing1;
    yvshemusic1=yvshemusic11;
    yvshemusic2=yvshemusic21;
    jingzhidonghua=jingzhidonghua1;
    danjidonghua=danjidonghua1;
    tuodongdonghua=tuodongdonghua1;
    color=color1;
    target=target1;
    h=h1;
    std::ofstream ss(zidingyiwenjian);
    ss<<shezhibeijing<<" "<<bofangqibeijing<<" "<<yvshemusic1<<" "<<yvshemusic2<<" "<<jingzhidonghua<<" "<<danjidonghua<<" "<<" "<<tuodongdonghua<<" "<<" "<< color.name().toStdString()<<" "<<target<<" "<<h;
    ss.close();
    onRestartClicked();
}


void Widget::initzidingyi()
{
    std::ifstream ss(zidingyiwenjian);
    if(ss.is_open())
    {
        std::string a;
        ss>>shezhibeijing>>bofangqibeijing>>yvshemusic1>>yvshemusic2>>jingzhidonghua>>danjidonghua>>tuodongdonghua>>a>>target>>h;
        color=QColor(QString::fromStdString(a));
        ss.close();
    }else{
        ss.close();
        shezhibeijing=":/tubiao.png";
        bofangqibeijing=":/bofangqi.png";
        yvshemusic1=":/music.wav";
        yvshemusic2=":/elysiam realm.wav";
        jingzhidonghua=":/1758263119038(1).gif";
        danjidonghua=":/0334aad0-61f2-4854-8d35-0cf70a8b1472.gif";
        tuodongdonghua=":/1680313875248.gif";
        QColor pink2("pink");
        color=pink2;
        target=150;
        h=50;
        std::ofstream ss(zidingyiwenjian);
        ss<<shezhibeijing<<" "<<bofangqibeijing<<" "<<yvshemusic1<<" "<<yvshemusic2<<" "<<jingzhidonghua<<" "<<danjidonghua<<" "<<tuodongdonghua<<" "<< color.name().toStdString()<<" "<<target<<" "<<h;
        ss.close();
    }
    zi->init(shezhibeijing,bofangqibeijing,yvshemusic1,yvshemusic2,jingzhidonghua,danjidonghua,tuodongdonghua,color,target,h);
}

void Widget::showzidingyi()
{
    zi->show();
}

void Widget::taicixianshi()
{
    tc->show();
}

void Widget::taicibaocun(QString* a,QString* b,QString* c)
{
    std::ofstream ss(taiciwenjian);
    for(int i=0;i<5;i++)
    {
        xianzhiyv[i]=a[i];
        ss<<xianzhiyv[i].toStdString()<<" ";
    }
    for(int i=0;i<5;i++)
    {
        danjiyv[i]=b[i];
        ss<<danjiyv[i].toStdString()<<" ";
    }
    for(int i=0;i<5;i++)
    {
        yidongyv[i]=c[i];
        ss<<yidongyv[i].toStdString()<<" ";
    }
    ss.close();
    tc->hide();
}


void Widget::taiciinit()
{
    std::ifstream ss(taiciwenjian);
    if(ss.is_open())
    {
        for(int i=0;i<5;i++)
        {
            std::string a;
            ss>>a;
            xianzhiyv[i]=QString::fromStdString(a);
        }
        for(int i=0;i<5;i++)
        {
            std::string a;
            ss>>a;
            danjiyv[i]=QString::fromStdString(a);
        }
        for(int i=0;i<5;i++)
        {
            std::string a;
            ss>>a;
            yidongyv[i]=QString::fromStdString(a);
        }
        ss.close();
        tc->init(xianzhiyv,danjiyv,yidongyv,shezhibeijing);
    }else{
        ss.close();
        std::ofstream ss(taiciwenjian);
        for(int i=0;i<5;i++)
        {
            ss<<xianzhiyv[i].toStdString()<<" ";
        }
        for(int i=0;i<5;i++)
        {
            ss<<danjiyv[i].toStdString()<<" ";
        }
        for(int i=0;i<5;i++)
        {
            ss<<yidongyv[i].toStdString()<<" ";
        }
        ss.close();
        tc->init(xianzhiyv,danjiyv,yidongyv,shezhibeijing);
    }
}


void Widget::onRestartClicked()
{
    // 1. 获取当前程序路径
    QString appPath = QApplication::applicationFilePath();

    // 2. 获取命令行参数（移除第一个参数，因为第一个是程序路径）
    QStringList args = QApplication::arguments();
    args.removeFirst();  // 移除程序名，保留其他参数

    // 3. 启动新的程序实例
    bool isStarted = QProcess::startDetached(appPath, args);
    if (!isStarted) {
        QMessageBox::critical(this, "错误", "无法启动新的程序实例！");
        return;
    }

    // 4. 退出当前程序（可在此处添加数据保存等清理逻辑）
    musicgui->hide();
    this->hide();
    this->~Widget();
    qApp->quit();
}
