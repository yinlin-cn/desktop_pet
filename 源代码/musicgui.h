#ifndef MUSICGUI_H
#define MUSICGUI_H

#include <QWidget>
#include<QMovie>
#include<filesystem>
#include<QLabel>

namespace Ui {
class musicGUI;
}

class musicGUI : public QWidget
{
    Q_OBJECT

public:
    explicit musicGUI(QWidget *parent = nullptr);
    ~musicGUI();
    void init(int a,int b,bool c,int d,int e,std::filesystem::path f);
    void ailihetisignal();

private slots:
    void on_btn1_clicked(bool checked);

    void on_btn2_clicked();

    void on_btn3_clicked(bool checked);

    void on_btn4_clicked(bool checked);

    //bool isNeteaseMusicPlaying();



private:
    Ui::musicGUI *ui;
    int playingcurrent;
    int playmodel;
    bool isstop;
    int bofangmoshi;
    int changdu;
    QMovie *bofanggif;
    bool ispress;
    bool ismove;
    QPoint dian;
    QPoint danjidian;
    std::filesystem::path beijing;
    QLabel *bgLabel;
    bool chushihua;
signals:
    void geqvbianhua(int a);
    void ifstop(bool q);
    void moshibianhua(int a);
    void ailiheti(QPoint a,int wid,int hig);

protected:
    void mousePressEvent(QMouseEvent *event) override;    // 鼠标按下（拖拽/点击响应）
    void mouseMoveEvent(QMouseEvent *event) override;     // 鼠标移动（窗口拖拽）
    void mouseReleaseEvent(QMouseEvent *event) override;  // 鼠标释放
};

#endif // MUSICGUI_H
