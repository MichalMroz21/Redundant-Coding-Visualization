#ifndef HAMMINGCODE_HPP
#define HAMMINGCODE_HPP

#include <QObject>
#include <QBitArray>
#include <QDebug>

class HammingCode : public QObject
{
    Q_OBJECT

private:
    QBitArray data{};
    int p{}, m{};

public:
    explicit HammingCode(QBitArray data, QObject *parent = nullptr);

    int getP() const;
    void setP(int newP);

    int getM() const;
    void setM(int newM);

public slots:
    int calculateP();
    void encodeData();
    bool isPowerTwo(int n);
    QBitArray getData();
    void setInitialData(QBitArray data);

signals:


};

#endif // HAMMINGCODE_HPP
