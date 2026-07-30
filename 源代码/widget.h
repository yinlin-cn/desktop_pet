#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include"musicgui.h"
#include"zidingyi.h"
#include"form.h"
#include"taici.h"
#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QPoint>
#include <QWidget>
#include<filesystem>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void loadMusicFromFolder(const std::filesystem::path& folderPath);
protected:
    void mousePressEvent(QMouseEvent *event) override;    // 鼠标按下（拖拽/点击响应）
    void mouseMoveEvent(QMouseEvent *event) override;     // 鼠标移动（窗口拖拽）
    void mouseReleaseEvent(QMouseEvent *event) override;  // 鼠标释放
    void moveEvent(QMoveEvent *event)override;
private:
    int target;
    int h;
    void onRestartClicked();
    bool xianshibofangqi1;
    bool useownwenjianjia1;
    std::filesystem::path zidingwenjianjiadizhi1;
    Ui::Widget *ui;
    void initWindow();    // 初始化窗口特性（无边框、置顶等）
    void initshezhi();
    void initAnimation(); // 初始化动画（GIF/序列帧）
    void initTimer();     // 初始化定时器（自主移动、动画切换）
    void initTrayIcon();  // 初始化系统托盘（隐藏/退出菜单）
    void onSingleClick(const QPoint &clickPos);
    void ailiyidong();
    void ailiplay();
    void showText(const QString &text, int durationMs = 3000);
    void updateTextPosition();
    QString extractMp3FromQrc(const QString &qrcPath);
    std::filesystem::path shezhiwenjian;
    void play0(int wei);
    void play1(int wei);
    void pause();
    void stopAudio();
    void initbool();
    void ailisay();
    void shezhichuangkou();
    void shezhibaocun(bool ailimove1, bool playwav1, bool ifshow1,bool xianshibofangqi,bool useownwenjianjia,int bofangmoshi,std::filesystem::path zidingwenjianjiadizhi);
    void changeplayingcurrent(int a);
    void changebofangmoshi(int a);
    void changeifstop(bool a);
    void chushimoshi();
    void bendimoshi();
    void ailiheti(QPoint a,int wid,int hig);
    void ailiheti1();
    void wangyiyun();
    void hidebofangqi();
    void xianshibofangqi();
    void showbofangqiactionhanshu();
    void hetijiancha(QPoint a,int wid,int hig);
    void jianchaxinghao();
    /*void playAudio();
    void pauseAudio();
    void resumeAudio();
    void stopAudio();*/


    // 界面元素
    QString tempMp3Path;
    QStringList musicList;
    std::filesystem::path bendimusic;
    int playingcurrent;
    int playmodel;
    bool isstop;
    bool qiehuan;
    bool hetizhongjiantai;
    int bofangmoshi;
    int changdu;
    QString chushimusic[2];
    int chushichangdu;
    QLabel *petlable;    // 显示宠物动画的标签
    QWidget *text;
    Form *form;
    musicGUI *musicgui;
    QMovie *xianzhi;   // 闲置状态动画（GIF）
    QMovie *yidong;
    QMovie *xianzhicun;   // 闲置状态动画（GIF）
    QMovie *danjicun;
    QMovie *danji;
    taici *tc;
    void taicixianshi();
    QString xianzhiyv[5]={"说起粉色头发的可爱女孩，你第一个会想到谁？321回答！","今天的任务都完成啦？真棒，夸夸你哦。","嗨，早上好！一天的好心情，从见到你开始。"," 高洁而纯真的灵魂，踏上旅途。","我名为爱莉希雅……最初的律者，人之律者。"};
    QString yidongyv[5]={"嗨，想我了吗？","嗯～和女孩子独处时，可要好好看向对方的眼晴噢～","这一次有你想要的东西吗?没有的话，我就可以再见你一面了。","不要被三次元的女人骗了呀。","哇，你看那朵白白软软的云，是不是有点像我呢？"};
    QString danjiyv[5]={" 你好像有不少问题想问我呢，别心急，我们还有很多很多时间。","这么晚了还不睡吗？是在想我，对不对？","天色暗了，接下来就是调皮捣蛋的时间了呢。","白天的我也很可爱，晚上的我也很可爱，你更喜欢哪个我呢？","真是的......我会哭的哦......"};
    QAction *showAction;
    QAction *hideAction;
    QAction *unmoveAction;
    QAction *moveAction;
    QAction *sayAction;
    QAction *unsayAction;
    QAction *playAction;
    QAction *pauseAction;
    QAction *chushiAction;
    QAction *bendiAction;
    QAction *shezhiAction;
    QAction *exitAction;
    QAction *hetiAction;
    QAction *jietiAction;
    QAction *wangyiyunAction;
    QAction *showbofangqiAction;



    // 交互相关
    QPoint dian;      // 拖拽起点坐标（用于窗口移动）
    bool ismove;    // 是否正在拖拽
    bool ispress;
    QPoint danjidian;
    bool isdanji;
    bool ailimove;
    bool isplaying;
    bool ifshow;
    bool playwav;
    bool chushihua;
    bool ifailiheti;
    bool zhongjiantai;
    bool zidongyidong;

    // 定时器（控制自主行为）
    QTimer *timer;   // 自主移动定时器
    QTimer *textHideTimer;
    QTimer *playStatusTimer;
    int kuan;     // 屏幕宽度（用于限制移动范围）
    int gao;    // 屏幕高度

    // 系统托盘
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayMenu;

    zidingyi* zi;
    std::filesystem::path shezhibeijing;
    std::filesystem::path bofangqibeijing;
    std::filesystem::path yvshemusic1;
    std::filesystem::path yvshemusic2;
    std::filesystem::path jingzhidonghua;
    std::filesystem::path danjidonghua;
    std::filesystem::path tuodongdonghua;
    QColor color;
    std::filesystem::path zidingyiwenjian;
    void initzidingyi();
    void zidingyibaocun(std::filesystem::path shezhibeijing1,
                        std::filesystem::path bofangqibeijing1,
                        std::filesystem::path yvshemusic11,
                        std::filesystem::path yvshemusic21,
                        std::filesystem::path jingzhidonghua1,
                        std::filesystem::path danjidonghua1,
                        std::filesystem::path tuodongdonghua1,
                        QColor color,int target,int h);
    void showzidingyi();
    std::filesystem::path taiciwenjian;
    void taicibaocun(QString* a,QString* b,QString* c);
    void taiciinit();
private slots:
    // 定时器槽函数（自主移动、动画切换等）
    void onAutoMove();
    // 系统托盘菜单槽函数
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void onShowPet();    // 显示宠物
    void onExit();       // 退出程序
    void checkPlayStatus();
    void onAnimationFinished(); // 动画结束后执行的逻辑
};
#endif // WIDGET_H
