#include "hammingcode.hpp"
#include <climits>

HammingCode::HammingCode(QBitArray data, QObject *parent) : QObject{parent}
{
    setInitialData(data);
}

void HammingCode::setInitialData(QBitArray data){
    this->data = data;
    this->m = data.size();
    this->p = calculateP();
}

int HammingCode::calculateP(){

    int power = 1, newP{};

    while(power < (m + newP + 1)){
        newP++;
        power <<= 1;
    }

    return newP;
}

bool HammingCode::isPowerTwo(int n){
    return (n > 0) && ((n & (n - 1)) == 0);
}

void HammingCode::encodeData(){

    int n = this->m + this->p, dataPtr{};

    QBitArray dataEncoded(n);

    for(int i = 0; i < n; i++){
        if(!isPowerTwo(i + 1)) dataEncoded[i] = data[dataPtr++];
    }

    for(int i = 1; i <= n; i *= 2){

        int xorRes = 0;

        for(int j = i - 1; j < n; j += i){
            int cnt = i;

            while(cnt--){
                xorRes ^= dataEncoded[j];
                j++;
            }
        }

        dataEncoded[i - 1] = xorRes;
    }

    data = dataEncoded;
}



int HammingCode::getP() const
{
    return p;
}

void HammingCode::setP(int newP)
{
    p = newP;
}

int HammingCode::getM() const
{
    return m;
}

void HammingCode::setM(int newM)
{
    m = newM;
}

QBitArray HammingCode::getData()
{
    return this->data;
}
