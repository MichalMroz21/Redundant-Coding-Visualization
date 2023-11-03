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

    int n = this->m + this->p,
        dataPtr = this->m - 1;

    QBitArray dataEncoded(n);

    //set the parity bits p1, p2, p4...
    for(int i = 1; i <= n; i++){

        if(isPowerTwo(i)){

            int oneCnt{};

            //sum ones in certain positions
            for(int j = i; j <= n; j += i){

                int cnt{};

                while(cnt < i){

                    oneCnt += dataEncoded[n - j];

                    cnt++;
                    j++;
                }
            }

            dataEncoded[n - i] = (oneCnt % 2);
        }

        else{
            dataEncoded[n - i] = data[dataPtr--];
        }
    }

    data.clear();
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
