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

    bool getEncodingExtended() const, isPowerTwo(int n);

public slots:

    int calculateP();
    int correctError();
    int correctErrorExtended();
    int correctErrorStandard();

    void encodeDataAsync(bool forQML);
    void encodeData(bool forQML);

    void setInitialData(QBitArray data, bool extend);
    void setInitialData(QString data, bool extend);

    void sendCode(QBitArray send);

    QBitArray getData();
    QString getDataStr();

    int getAnimationDelayMs() const;
    void setAnimationDelayMs(int delay);

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

private:

    QBitArray data{}, receivedCode{};
    int p{}, m{}, animationDelayMs{};
    bool encodingExtended = false;

};

#endif // HAMMINGCODE_HPP
