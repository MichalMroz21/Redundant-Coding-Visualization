#ifndef HAMMINGCODE_HPP
#define HAMMINGCODE_HPP

#include <QObject>
#include <QBitArray>
#include <QDebug>

class HammingCode : public QObject
{
    Q_OBJECT

public:

    explicit HammingCode(QBitArray data, QObject *parent = nullptr);

    void setP(int newP), setM(int newM), setReceivedCode(const QBitArray& newReceivedCode);

    int getP() const, getM() const;

    QBitArray getReceivedCode() const;

    bool getEncodingExtended() const, isPowerTwo(int n);

public slots:

    int calculateP();
    int correctError();
    int correctErrorExtended();
    int correctErrorStandard();

    void encodeData(bool extend = false);
    void setInitialData(QBitArray data);
    void sendCode(QBitArray send);

    QBitArray getData();

signals:


private:

    QBitArray data{}, receivedCode;
    int p{}, m{};
    bool encodingExtended = false;

};

#endif // HAMMINGCODE_HPP
