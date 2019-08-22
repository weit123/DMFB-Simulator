#ifndef CHIP_H
#define CHIP_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QTimer>
#include <set>
#include <stack>
#include <cstdlib>
#include <QSoundEffect>
#include "command.h"

using std::multiset;
using std::stack;

const QColor colorPalette[7] = {
    Qt::red, Qt::darkRed, Qt::green, Qt::darkGreen, Qt::blue, Qt::magenta, Qt::yellow
};

class waterDrop {
public:
    static int idMax;
    //waterDrop(int row, int col, int id, QColor color);
    waterDrop(int row, int col, int id, QColor color, int dir = 0, double size = 1.0);
    int row, col, dir;
    double size;
    bool operator< (const waterDrop &d) const {
        if(id < d.id)
            return true;
        return false;
    }
    int getId() {return id;}

    friend class chip;

private:
    int id;
    QColor color;
};

class chip : public QWidget
{
    Q_OBJECT
public:
    explicit chip(QWidget *parent = nullptr);
    bool ready = false;
    void clearAllInput();

    void initCommandList(multiset<command>& commandList);
    void toPrev();
    void playAll();
    void reset();

protected:
    void paintEvent(QPaintEvent *);

signals:
    void timeChanged(int);

public slots:
    void setReady();
    void setSize(int, int);
    void setInput(QString);
    void setOutput(int, int);

    void toNext();

private:
    QString point2string(int row, int col);

    int currentTime = 0;
    QTimer *timer = new QTimer();
    int _rowNum = 3, _colNum = 3;

    const int startRow = 90, startCol = 130;
    int _length = 50;
    multiset<QString> inputPos;
    int _outputRowPos, _outputColPos;

    void drawChip();
    void drawInput(int, int);
    void drawOutput();
    void drawDrop(int, int, QColor, int, double);
    void drawWater();

    int calRowPos(int rowPos);
    int calColPos(int colPos);

    multiset<waterDrop> water;
    multiset<waterDrop>::iterator getDrop(int row, int col);

    multiset<command>::iterator curCommand;
    multiset<command> commands;
    multiset<command>::iterator getPrev();
    void operate(command op, bool mute = false);
    void operateReverse(command op);
    stack<QColor> usedColor;

    QSoundEffect *moveSound, *splitSound, *mergeSound, *stretchSound;

    bool conatraint = true;
    bool checkConstraint();
};

#endif // CHIP_H
