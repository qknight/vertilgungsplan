#ifndef CALCULATORFORM_H
#define CALCULATORFORM_H

#include "ui_dialog.h"
class Dialog : public QWidget {
Q_OBJECT
private slots:
    void myexit();
    void r1();
    void r2();
    void r3();
    void r4();
    void berechne();
    void summechanged(int value);
    void tilgungchanged(int value);
    void zinssatzchanged(double value);
    void berechne_restlaufzeit(int value);
public:
    Dialog(QWidget *parent = 0);
private:
    Ui::Dialog ui;
    long summe;
    long abzahlung;
    float zinssatz;
    int zahlungenprojahr;

};

#endif
