#ifndef TAICI_H
#define TAICI_H

#include <QWidget>
#include<filesystem>

namespace Ui {
class taici;
}

class taici : public QWidget
{
    Q_OBJECT

public:
    explicit taici(QWidget *parent = nullptr);
    ~taici();
    void init(QString* a,QString* b,QString* c,std::filesystem::path e);

private slots:
    void on_pushButton_clicked(bool checked);

private:
    Ui::taici *ui;
    QString xianzhi[5];
    QString danji[5];
    QString tuodong[5];
    std::filesystem::path taicibeijing;
    QImage* beijing;

signals:
    void taicichuandi(QString* a,QString* b,QString* c);

protected:
    void paintEvent(QPaintEvent *event)override;
    void closeEvent(QCloseEvent *event)override;
};

#endif // TAICI_H
