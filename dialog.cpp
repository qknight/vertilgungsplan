#include <QtGui>
#include "dialog.h"
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <QDate>
#include <math.h>

Dialog::Dialog(QWidget *parent) : QWidget(parent) {
    ui.setupUi(this);
    ui.pushButton->setText("&Exit");

    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(myexit()));
    connect(ui.r1, SIGNAL(clicked()), this, SLOT(r1()));
    connect(ui.r2, SIGNAL(clicked()), this, SLOT(r2()));
    connect(ui.r3, SIGNAL(clicked()), this, SLOT(r3()));
    connect(ui.r4, SIGNAL(clicked()), this, SLOT(r4()));
    connect(ui.betrag, SIGNAL(valueChanged(int)), this, SLOT(summechanged(int)));
    connect(ui.tilgung, SIGNAL(valueChanged(int)), this, SLOT(tilgungchanged(int)));
    connect(ui.zinsen, SIGNAL(valueChanged(double)), this, SLOT(zinssatzchanged(double)));
    connect(ui.dateEdit, SIGNAL(dateChanged(QDate)), this, SLOT(berechne()));
    connect(ui.restlaufzeit, SIGNAL(valueChanged(int)), this, SLOT(berechne_restlaufzeit(int)));
    ui.r2->setChecked(true);
    ui.radioTilgung->setChecked(true);

    // restore settings from last session
    ui.dateEdit->setDate(QDate::currentDate());
    summechanged(55555); // in cents [euro]
    tilgungchanged(5555); // in cents [euro]
    zinssatzchanged(6.75); // prozent/100
    r2(); // 4 == all 3 months
}

void Dialog::summechanged(int value) {
    if (ui.betrag->value() != value)
        ui.betrag->setValue(value);
    summe=value*100;
    if (ui.radioLaufzeit->isChecked() == true) {
        berechne_restlaufzeit(ui.restlaufzeit->value());
    } else {
        berechne();
    }
}

void Dialog::tilgungchanged(int value) {
    if (ui.tilgung->value() != value)
        ui.tilgung->setValue(value);
    abzahlung=value*100;
    berechne();
}

void Dialog::zinssatzchanged(double value) {
    if (ui.zinsen->value() != value)
        ui.zinsen->setValue(value);
    zinssatz=(float)value/100;
    if (ui.radioLaufzeit->isChecked() == true) {
        berechne_restlaufzeit(ui.restlaufzeit->value());
    } else {
        berechne();
    }
}

void Dialog::r1() {
    zahlungenprojahr=12;
    berechne();
}

void Dialog::r2() {
    zahlungenprojahr=4;
    berechne();
}

void Dialog::r3() {
    zahlungenprojahr=2;
    berechne();
}

void Dialog::r4() {
    zahlungenprojahr=1;
    berechne();
}

void Dialog::berechne_restlaufzeit(int value) {
    if (ui.radioLaufzeit->isChecked() == true) {
        double mzf = pow((double)(zinssatz+1),((double)1/(double)zahlungenprojahr));
        double r = (double)summe/100 * (pow((double)mzf,(double)value)/(pow((double)mzf,(double)value)-1))*(mzf-(double)1);
        ui.tilgung->setValue((int)r);
    }
}

void Dialog::berechne() {

    long lsumme=summe;
    int  count=0;
    long tilgung=0;
    long zins=0;
    long sum_zins=0;
    long sum_tilgung=0;

    QDate startDate = ui.dateEdit->date();
    QString text,tail;
    ui.status->setText("");

    count = 0;

    text.append(QString("AnfangsTermin: ").rightJustified(30, ' '));

    text.append(startDate.toString("dd MMM yyyy") + "\n");

    QString tmp;
    tmp.setNum(lsumme);
    text.append(QString("KreditSumme: ").rightJustified(30, ' ') + tmp.insert((tmp.size())-2,'.') + " Euro\n");
    tmp.clear();
    tmp.setNum(zinssatz*100);
    text.append(QString("Zinssatz: ").rightJustified(30, ' ') + tmp + " %\n");
    tmp.setNum(zahlungenprojahr);
    text.append(QString("Zahlungen/Jahr: ").rightJustified(30, ' ') + tmp + "\n");
    tmp.setNum(abzahlung);
    text.append(QString("Zahlung/Rate: ").rightJustified(30, ' ') + tmp.insert((tmp.size())-2,'.') + " Euro\n");

    zins=(int)((float)(lsumme*zinssatz)/zahlungenprojahr+0.5);
    tmp.setNum(zins);
    text.append(QString("Zinsen bei erster Rate: ").rightJustified(30, ' ') + tmp.insert((tmp.size())-2,'.') + " Euro\n");

    if (zins >= abzahlung) {
        text.append("\n  ACHTUNG: DIE ZINSEN PRO RATE SIND GROESSER ALS DIE VORGEGEBENEN ZAHLUNGEN\n");
    } else {
        tail.append(" " +
                    QString("Rate").rightJustified(5, ' ') + " " +
                    QString("Datum").rightJustified(14, ' ') + " " +
                    QString("Tilgung").rightJustified(12, ' ') + " " +
                    QString("Zins").rightJustified(12, ' ') + " " +
                    QString("Restschuld").rightJustified(12, ' ') + "\n");
        while (lsumme > 0) {
            if (count != 0) {
                tmp.setNum(count);
                tmp=tmp.rightJustified(5, ' ', true);

                tail.append(" " + tmp + "    " + startDate.toString("dd-MMM-yyyy") + " ");

                tmp.setNum(tilgung);
                tmp.insert((tmp.size())-2,'.');
                tail.append(tmp.rightJustified(12, ' ') + " ");

                tmp.setNum(zins);
                tmp.insert((tmp.size())-2,'.');
                tail.append(tmp.rightJustified(12, ' ') + " ");

                tmp.setNum(lsumme);
                tmp.insert((tmp.size())-2,'.');
                tail.append(tmp.rightJustified(12, ' ') + " ");

                tail.append("\n");
                switch (zahlungenprojahr) {
                    case 12:
                        startDate=startDate.addMonths(1);
                        break;
                    case 4:
                        startDate=startDate.addMonths(3);
                        break;
                    case 2:
                        startDate=startDate.addMonths(6);
                        break;
                    case 1:
                        startDate=startDate.addMonths(12);
                        break;
                }
            }

            count++;
            zins=(int)((float)(lsumme*zinssatz)/zahlungenprojahr+0.5);

            sum_zins+=zins;

            tilgung=(abzahlung-zins);
            if (tilgung > lsumme)
                tilgung=lsumme;

            sum_tilgung+=tilgung;
            lsumme-=tilgung;

        }
        ui.restlaufzeit->setValue(count-1);
        ui.restlaufzeit->setSuffix(" raten");

        tmp.setNum(sum_zins);
        tmp.insert((tmp.size())-2,'.');
        ui.status->setText("Summe an Zinsen: " + tmp + " Euro");
        text.append(QString("Summe an Zinsen: ").rightJustified(30, ' ') + tmp + " Euro\n\n");
        text.append(tail);
        text.append("\n\n  Thanks for using http://sourceforge.net/projects/vertilgungsplan\n");
        text.append("  This software is distributed under the terms of the GPL V2\n");

        if (summe != sum_tilgung) {
            text.append("Achtung! Schwerer Ausnahmefehler 0E\n");
        }
    }
    ui.textBrowser->setPlainText(text);
}

void Dialog::myexit() {
    exit(0);
}
