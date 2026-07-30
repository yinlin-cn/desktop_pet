#include "musicgui.h"
#include "ui_musicgui.h"
#include<QLabel>
#include<QMovie>
#include<QScreen>
#include<QMouseEvent>
#include<windows.h>
#include<QTimer>
musicGUI::musicGUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::musicGUI)
{
    ui->setupUi(this);
    chushihua=true;
    // ===================== 2. 获取屏幕可用区域（自动扣除任务栏等系统区域） =====================
    // 获取主屏幕（多屏时可改为获取当前窗口所在屏幕，见下方说明）
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) {
        qDebug() << "无法获取屏幕信息";
        return;
    }

    // 获取屏幕可用区域（不含任务栏的区域，右下角坐标是关键）
    QRect screenGeometry = screen->availableGeometry();
    int screenWidth = screenGeometry.width();   // 屏幕可用宽度
    int screenHeight = screenGeometry.height(); // 屏幕可用高度

    // ===================== 3. 计算窗口左上角坐标（让窗口右下角对齐屏幕右下角） =====================
    int windowWidth = this->width();   // 窗口宽度（已通过setFixedSize设置）
    int windowHeight = this->height(); // 窗口高度

    // 窗口左上角x = 屏幕宽度 - 窗口宽度
    // 窗口左上角y = 屏幕高度 - 窗口高度
    int x = screenWidth - windowWidth;
    int y = screenHeight - windowHeight;

    // ===================== 4. 移动窗口到计算好的位置 =====================
    this->move(x, y);
    setAttribute(Qt::WA_TranslucentBackground, true);  // 核心修复！
    // 禁用窗口的默认背景绘制（避免干扰透明效果）
    setAttribute(Qt::WA_NoSystemBackground, false);
    // 在 musicGUI 构造函数中，设置主窗口背景透明
    setStyleSheet(R"(
    musicGUI {
        background-color: transparent;  /* 主窗口背景透明 */
        border: none;  /* 避免主窗口默认边框 */
    }
)");
    qDebug() << "窗口尺寸：" << width() << "x" << height();

    // 2. 创建背景Label
    bgLabel = new QLabel(this);
    bgLabel->setScaledContents(true);  // 启用内容自适应 Label 大小
    bgLabel->setObjectName("backgroundLabel");
    bgLabel->setStyleSheet("background-color: transparent;");
    // 关键：明确设置Label背景为透明，且不继承其他样式
    bgLabel->setStyleSheet(R"(
    QLabel#backgroundLabel {  /* 用对象名限定，避免被全局样式覆盖 */
        background-color: transparent;  /* Label自身背景透明 */
        border: none;  /* 确保没有默认边框（部分主题可能有） */
    }
)");

    // 3. 强制Label尺寸与窗口一致（关键！）
    bgLabel->setGeometry(0, 0, width(), height());  // 从(0,0)开始，宽高=窗口宽高
    qDebug() << "Label初始尺寸：" << bgLabel->width() << "x" << bgLabel->height();




    // 窗口属性：无边框、置顶、无任务栏图标（保持你的需求）
    setWindowFlags(
        Qt::FramelessWindowHint        // 无边框
        | Qt::WindowStaysOnTopHint     // 置顶
        | Qt::Tool                     // 工具窗口（无任务栏图标）
        | Qt::X11BypassWindowManagerHint // Linux兼容（Windows可保留）
        );


    // ===================== 2. 按钮配置（图标+样式+文字，不覆盖） =====================
    // 2.1 统一按钮样式（合并：无边框+透明+悬停+点击+文字样式，避免重复覆盖）
    QString btnStyle = R"(
        QPushButton {
            /* 基础样式：无边框、透明背景 */
            border: none;
            background-color: transparent;

            /* 图标大小（与按钮大小匹配） */
            icon-size: 35px 35px;  /* 等价于 setIconSize(QSize(40,40)) */

            /* 按钮固定大小（避免变形） */
            min-width: 50px;
            min-height: 50px;
            max-width: 50px;
            max-height: 50px;

        }

        /* 鼠标悬停效果 */
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 50);  /* 轻微透明白色 */
            border-radius: 4px;  /* 可选：圆角更美观 */
        }

        /* 点击效果 */
        QPushButton:pressed {
            background-color: rgba(0, 0, 0, 30);  /* 轻微透明黑色 */
        }
    )";

    // 2.2 给每个按钮设置图标 + 应用统一样式（关键：一次设置，不重复覆盖）
    // 上一首按钮
    ui->btn1->setIcon(QIcon(":/shangyishou.png"));  // 图标路径正确
    ui->btn1->setStyleSheet(btnStyle);              // 应用样式（不覆盖图标）
    // 播放按钮
    ui->btn2->setIcon(QIcon(":/bofang.png"));
    ui->btn2->setStyleSheet(btnStyle);
    // 下一首按钮
    ui->btn3->setIcon(QIcon(":/xiayishou.png"));
    ui->btn3->setStyleSheet(btnStyle);
    // 随机播放按钮
    ui->btn4->setIcon(QIcon(":/suijibofangqi.png"));  // 注意：原代码是 suijibofang.png，确认资源名一致！
    ui->btn4->setStyleSheet(btnStyle);
}

musicGUI::~musicGUI()
{
    delete ui;
}

void musicGUI::init(int a,int b,bool c,int d,int e,std::filesystem::path f)
{
    playingcurrent=a;
    playmodel=b;
    isstop=c;
    bofangmoshi=d;
    changdu=e;
    switch (bofangmoshi) {
    case 0:{ui->btn4->setIcon(QIcon(":/suijibofang.png"));break;}
    case 1:{ui->btn4->setIcon(QIcon(":/xunhuanbofang.png"));break;}
    case 2:{ui->btn4->setIcon(QIcon(":/danqvxunhuan.png"));break;}
    }
    /*if(isNeteaseMusicPlaying()&&playmodel==3)
    {
        ui->btn2->setIcon(QIcon(":/zanting.png"));
        return;
    }
    else if(!isNeteaseMusicPlaying()&&playmodel==3)
    {
        ui->btn2->setIcon(QIcon(":/bofang.png"));
        return;
    };*/
    if(isstop)ui->btn2->setIcon(QIcon(":/bofang.png"));
    else ui->btn2->setIcon(QIcon(":/zanting.png"));

    if(chushihua)
    {
    beijing=f;
    // 4. 加载图片并缩放
    // 加载原图（建议用 QImage 先处理，比 QPixmap 缩放质量更高）
    QImage originalImage(QString::fromStdString(beijing.string()));  // 优先使用高分辨率原图
    if (originalImage.isNull()) {
        qDebug() << "图片加载失败";
        return;
    }

    // 目标缩放尺寸（例如缩放到 800x600）
    QSize targetSize(332, 132);

    // 关键：使用平滑缩放算法（SmoothTransformation）
    QImage scaledImage = originalImage.scaled(
        targetSize,
        Qt::KeepAspectRatio,  // 保持原图比例（避免拉伸变形导致的“视觉模糊”）
        Qt::SmoothTransformation  // 平滑缩放，保留细节
        );

    // 显示缩放后的图片（QLabel 或绘制到界面）
    bgLabel->setPixmap(QPixmap::fromImage(scaledImage));
    /*QString imgPath = ":/bofangqi.jpg";
    QPixmap bgPixmap(imgPath);
    if (!bgPixmap.isNull()) {
        // 强制充满Label
        QPixmap scaledPix = bgPixmap.scaled(
            bgLabel->size(),
            Qt::IgnoreAspectRatio,  // 忽略比例，充满Label
            Qt::SmoothTransformation
            );
        bgLabel->setPixmap(scaledPix);
        qDebug() << "图片缩放后尺寸：" << scaledPix.width() << "x" << scaledPix.height();
    } else {
        qDebug() << "图片加载失败！";
    }*/

    bgLabel->lower();
    chushihua=false;
    };
}

void musicGUI::on_btn1_clicked(bool checked)
{
    if(playmodel == 3) {
        // 按下按键
        keybd_event(VK_CONTROL, 0, 0, 0);
        keybd_event(VK_MENU, 0, 0, 0);
        keybd_event(VK_LEFT, 0, 0, 0);

        // 100毫秒后释放（非阻塞）
        QTimer::singleShot(500, this, [](){
            keybd_event(VK_LEFT, 0, 2, 0);
            keybd_event(VK_MENU, 0, 2, 0);
            keybd_event(VK_CONTROL, 0, 2, 0);
        });
        return;
    }
    if(playingcurrent==0)
        playingcurrent=changdu-1;
    else
        playingcurrent--;
    emit geqvbianhua(playingcurrent);
}


void musicGUI::on_btn2_clicked()
{
    if(playmodel == 3) {
        // 按下按键
        keybd_event(VK_CONTROL, 0, 0, 0);
        keybd_event(VK_MENU, 0, 0, 0);
        keybd_event('P', 0, 0, 0);

        // 100毫秒后释放（非阻塞）
        QTimer::singleShot(500, this, [](){
            keybd_event('P', 0, 2, 0);
            keybd_event(VK_MENU, 0, 2, 0);
            keybd_event(VK_CONTROL, 0, 2, 0);
        });
    }
    if(isstop)
    {
        isstop=false;
        ui->btn2->setIcon(QIcon(":/zanting.png"));
    }else{
        isstop=true;
        ui->btn2->setIcon(QIcon(":/bofang.png"));
    }
    if(playmodel==3)return;
    emit ifstop(isstop);
}


void musicGUI::on_btn3_clicked(bool checked)
{
    if(playmodel == 3) {
        // 按下按键
        keybd_event(VK_CONTROL, 0, 0, 0);
        keybd_event(VK_MENU, 0, 0, 0);
        keybd_event(VK_RIGHT, 0, 0, 0);

        // 100毫秒后释放（非阻塞）
        QTimer::singleShot(500, this, [](){
            keybd_event(VK_RIGHT, 0, 2, 0);
            keybd_event(VK_MENU, 0, 2, 0);
            keybd_event(VK_CONTROL, 0, 2, 0);
        });
        return;
    }
    if(playingcurrent==changdu-1)
        playingcurrent=0;
    else
        playingcurrent++;
    emit geqvbianhua(playingcurrent);
}


void musicGUI::on_btn4_clicked(bool checked)
{
    switch (bofangmoshi) {
    case 0:{bofangmoshi++;ui->btn4->setIcon(QIcon(":/xunhuanbofang.png"));break;}
    case 1:{bofangmoshi++;ui->btn4->setIcon(QIcon(":/danqvxunhuan.png"));break;}
    case 2:{bofangmoshi=0;ui->btn4->setIcon(QIcon(":/suijibofang.png"));break;}
    }
    emit moshibianhua(bofangmoshi);
}

void musicGUI::mousePressEvent(QMouseEvent *event)    // 鼠标按下（拖拽/点击响应）
{
    if (event->button() == Qt::LeftButton) {
        danjidian = event->pos();  // 保存点击在窗口内的相对位置
        ispress = true;     // 标记左键已按下
        // 记录拖拽起点（窗口左上角到鼠标的偏移）
        dian = event->globalPos() - frameGeometry().topLeft();
        ismove = true;
    }
    QWidget::mousePressEvent(event);
}
void musicGUI::mouseMoveEvent(QMouseEvent *event)     // 鼠标移动（窗口拖拽）
{
    if (ismove && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - dian);
        QPoint a=this->pos();
        emit ailiheti(a,this->width(),this->height());
    }
    QWidget::mouseMoveEvent(event);
}
void musicGUI::mouseReleaseEvent(QMouseEvent *event)  // 鼠标释放
{
    ispress = false;
    ismove = false;
    QWidget::mouseReleaseEvent(event);
}

void musicGUI::ailihetisignal()
{
    QPoint a=this->pos();
    emit ailiheti(a,this->width(),this->height());
}

//由于网易云句柄不定，因此难以实现该功能
// 获取网易云音乐播放状态
/*bool musicGUI::isNeteaseMusicPlaying()
{
    // 1. 查找网易云音乐窗口句柄（类名和标题关键字需根据实际版本调整）
    // 网易云音乐的窗口类名通常为"OrpheusBrowserHost"，标题包含"网易云音乐"
    HWND hwnd = FindWindowA("OrpheusBrowserHost", nullptr);
    if (hwnd == nullptr) {
        // 若未找到主窗口，尝试通过标题模糊查找
        hwnd = FindWindowA(nullptr, "网易云音乐");
        if (hwnd == nullptr) {
            // 网易云音乐未运行
            return false;
        }
    }

    // 2. 获取窗口标题文本（最多256个字符）
    char title[256] = {0};
    GetWindowTextA(hwnd, title, sizeof(title));
    QString windowTitle = QString::fromLocal8Bit(title);

    // 3. 解析标题判断播放状态
    // 播放时标题格式通常为："歌曲名 - 歌手名 - 网易云音乐"
    // 暂停时部分版本会显示："(暂停) 歌曲名 - 歌手名 - 网易云音乐"
    if (windowTitle.contains("网易云音乐")) {
        // 排除未播放任何歌曲的情况（标题可能仅为"网易云音乐"）
        if (windowTitle == "网易云音乐") {
            return false; // 未播放歌曲
        }
        // 排除暂停状态（根据实际版本调整判断逻辑）
        if (windowTitle.contains("(暂停)")) {
            return false; // 已暂停
        }
        // 其他情况视为正在播放
        return true;
    }

    return false;
}*/
