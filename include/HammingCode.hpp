#ifndef HAMMINGCODE_HPP
#define HAMMINGCODE_HPP

#include <QObject>
#include <QBitArray>
#include <QDebug>

class HammingCode : public QObject
{
    Q_OBJECT

public:

    explicit HammingCode(QObject *parent = nullptr);

    void setP(int newP), setM(int newM), setReceivedCode(const QBitArray& newReceivedCode);

    int getP() const, getM() const;

    QBitArray getReceivedCode() const;

    bool isPowerTwo(int n);

public slots:

    int calculateP();

    int correctErrorExtended(bool forQML);
    int correctErrorStandard(bool forQML);

    int correctError(bool forQML);
    void encodeDataAsync(bool forQML);
    void encodeData(bool forQML);

    void setInitialData(QBitArray data, bool extend, int animationSpeed = 1000, bool infiniteWait = false);
    void setInitialData(QString data, bool extend, int animationSpeed = 1000, bool infiniteWait = false);

    void sendCode(QBitArray send);
    void sendCode(QString send);

    QBitArray getData();
    QString getDataStr();

    int getAnimationDelayMs() const;
    bool getEncodingExtended() const;

    void setAnimationDelayMs(int delay);
    void setInfiniteWait(bool value);

    void pressButton();
//QML Hamming.qml visualization API
signals:

    void turnBitOnAutoOff(int arrIndex, int index, QString color = "");

    void turnBitOn(int arrIndex, int index, QString color = "");
    void turnBitOff(int arrIndex, int index);

    void pushEmptyArray(int size);
    void pushArray(QString str);

    void popArray();
    void deleteArrayAtIndex(int index);

    void setBit(int arrIndex, int index, QString bit);
    void negateBit(int arrIndex, int index);

    void insertEmptyBit(int arrIndex, int index);
    void insertBit(int arrIndex, int index, QString bit);

    void insertArray(int index, QString str);

    void setBelowText(QString str);
    void setClickAllow(int arrIndex, bool isAllowed);

    void encodingEnd();
    void loadMainMenuButton();

private:

    QBitArray data{}, receivedCode{};
    int p{}, m{}, animationDelayMs{};
    bool encodingExtended = false, infiniteWait = false, buttonPressed = false;

    void waitForQml();

};

#endif // HAMMINGCODE_HPP
