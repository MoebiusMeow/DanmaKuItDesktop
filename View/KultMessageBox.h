#ifndef KULTMESSAGEBOX_H
#define KULTMESSAGEBOX_H

#include <QObject>
#include <QtWidgets>

class KultMessageBox : public QDialog
{
    Q_OBJECT

public:
    enum KultMessageBoxButton { Confirm, Cancel, NoButton };

private:
    bool draging;
    QPoint dragPosition;
    KultMessageBox(QWidget *parent);

protected:
    QPushButton *button1;
    QPushButton *button2;
    QLabel *label;
    QFrame *frame;
    void setupUI();
    void closeEvent(QCloseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

    static void setupButtonState(QPushButton *button, KultMessageBoxButton type);

public:
    void setText(QString text);

    static KultMessageBoxButton information(QWidget *parent, QString title, QString text,
                                            KultMessageBoxButton button1,
                                            KultMessageBoxButton button2 = NoButton);

    ~KultMessageBox();
};

#endif // KULTMESSAGEBOX_H
