#ifndef CHIP_H
#define CHIP_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QTimer>
#include <set>
#include <stack>
#include <queue>
#include <cstdlib>
#include <QSoundEffect>
#include <QMouseEvent>
#include "command.h"
#include <string>

using std::multiset;
using std::stack;
using std::queue;
using std::string;

const QColor colorPalette[7] = {
    Qt::red, Qt::darkRed, Qt::green, Qt::darkGreen, Qt::darkBlue, Qt::magenta, Qt::yellow
};

struct stainCommand {
    int time, row, col;
    QColor prevColor;
    int prevId;
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

    multiset<QString> inputPos;
    int _outputRowPos, _outputColPos;
    QString point2string(int row, int col);
    int getRowNum() {return _rowNum;}
    int getColNum() {return _colNum;}

    void initCommandList(multiset<command>& commandList);
    void toPrev();
    void playAll();
    void reset();

    void changePrintFlag();

    void setWashEnable(bool);
    void setWashInput(int, int);
    void setWashOutput(int, int);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent ( QMouseEvent * e );

signals:
    void timeChanged(int);

public slots:
    void setReady();
    void setSize(int, int);
    void setInput(QString);
    void setOutput(int, int);
    void toNext();

private:
    int currentTime = 0;
    QTimer *timer = new QTimer();
    QTimer *washTimer = new QTimer();
    int _rowNum = 3, _colNum = 3;

    const int startRow = 90, startCol = 130;
    int _length = 50;

    void drawChip();
    void drawInput(int, int);
    void drawOutput();
    void drawDrop(int, int, QColor, int, double);
    void drawWater();
    void drawWasher();
    void drawStain();
    void drawPollution();
    void drawWasherDrop();

    int calRowPos(int rowPos);
    int calColPos(int colPos);

    multiset<waterDrop> water;
    multiset<waterDrop>::iterator getDrop(int row, int col);

    multiset<command>::iterator curCommand;
    multiset<command> commands;
    multiset<command>::iterator getPrev();
    void operate(command op, bool mute = false);
    void operateReverse(command op);
    stack<double> usedSize;
    stack<QColor> usedColor;

    QSoundEffect *moveSound, *splitSound, *mergeSound, *stretchSound;

    bool conatraint = true;
    bool checkConstraint();

    QColor stainColor[15][15];
    int stainCnt[15][15];
    int stainId[15][15];
    stack<stainCommand> stainLog;
    bool printPollution = false;

    bool washEnable = false;
    int washInputRow, washInputCol;
    int washOutputRow, washOutputCol;
    int washerRow, washerCol;
    bool isClicked[15][15];
    bool washState;
    bool washStopSignal = false;
    void washStain(int, int, bool flag = true);
    string findRoute1(int, int);
    string findRoute2(int, int);
};

#endif // CHIP_H
