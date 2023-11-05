#include "HammingCode.hpp"

#include <climits>
#include <QtConcurrent>

HammingCode::HammingCode(QObject *parent) : QObject{parent}{}

void HammingCode::setInitialData(QBitArray data, bool extend){
    this->data = data;
    this->m = data.size();
    this->p = calculateP();
    this->animationDelayMs = 1000;
    this->encodingExtended = extend;
    this->receivedCode.clear();
}

void HammingCode::setInitialData(QString data, bool extend)
{
    int n = data.size();

    QBitArray bits(n);

    for(int i = 0; i < n; i++){
        bits[i] = (data[i].toLatin1() - '0');
    }

    setInitialData(bits, extend);
}

//Simulate sending some code, it can have 1 bit error
void HammingCode::sendCode(QBitArray send){
    receivedCode = send;
}

int HammingCode::correctErrorExtended()
{
    int n = this->m + this->p, C{}, P{}, bit{};

    for(int i = 1; i < n; i *= 2){

        int xorVal = 0;

        xorVal ^= receivedCode[i]; //i because extended parity bit must be not used for this

        for(int j = i + 1; j < n; j++){  //indexing +1, so <= n -> < n
            if(j & (1 << bit)){
                xorVal ^= receivedCode[j]; //indexing +1
            }
        }

        C += xorVal * i;
        bit++;
    }

    P = receivedCode.count(true) & 1;

    if(C == 0){ //theory from youtube
        if(P == 1){
            qInfo() << "Error occured in extended parity bit, correcting";
            receivedCode[C] = !receivedCode[C];
            return C;
        }
        else{
            qInfo() << "No error";
            return -1;
        }
    }
    else{
        if(P == 1){
            qInfo() << "Single error occured correcting...";
            receivedCode[C] = !receivedCode[C];
            return C;
        }
        else{
            qInfo() << "Double error occured that cannot be corrected";
            return -2;
        }
    }
}

int HammingCode::correctErrorStandard()
{
    int n = this->m + this->p, C{}, bit{};

    for(int i = 1; i <= n; i *= 2){ //calculate parity bits and add them up with formula: p1 * 1 + p2 * 2 + p3 * 4 + ... = C

        int xorVal = 0;

        xorVal ^= receivedCode[i - 1]; //xor is same as counting 1's

        for(int j = i + 1; j <= n; j++){
            if(j & (1 << bit)){
                xorVal ^= receivedCode[j - 1];
            }
        }

        C += xorVal * i;
        bit++;
    }

    if(C == 0){ //theory from youtube
        qInfo() << "No error!";
        return -1;
    }
    else{
        qInfo() << "Error at position: " << (C - 1);
        receivedCode[C - 1] = !receivedCode[C - 1];
        return C - 1;
    }
}

//Correct the 1 bit error in received code
int HammingCode::correctError()
{
    if(this->encodingExtended) return correctErrorExtended();
    else return correctErrorStandard();
}

int HammingCode::calculateP(){

    int power = 1, newP{};

    while(power < (m + newP + 1)){ //the lowest integer that satisfies the formula 2^p >= m + p + 1
        newP++;
        power <<= 1;
    }

    return newP;
}

bool HammingCode::isPowerTwo(int n){
    return (n > 0) && ((n & (n - 1)) == 0); //O(1) trick
}


void HammingCode::encodeData(bool forQML){

    if(forQML){
        static_cast<void>(QtConcurrent::run([=](){
            encodeDataAsync(forQML);
        }));
    }

    else{
        encodeDataAsync(forQML);
    }
}

void HammingCode::encodeDataAsync(bool forQML){

    int n = this->m + this->p, dataPtr{};

    QBitArray dataEncoded(n);

    if(forQML){
        emit pushArray(this->getDataStr());
        emit pushEmptyArray(n);
    }

    for(int i = 0; i < n; i++){
        if(!isPowerTwo(i + 1)) dataEncoded[i] = data[dataPtr++]; //copying non-parity bits
    }

    int bit = 0;

    for(int i = 1; i <= n; i *= 2){ //calculating parity bits

        int xorVal = 0; //counting number of 1's for each parity bit, xor just signals even/odd count

        for(int j = i + 1; j <= n; j++){
            if(j & (1 << bit)){ //bit manipulation trick
                xorVal ^= dataEncoded[j - 1];
            }
        }

        dataEncoded[i - 1] = xorVal; //keeping even number of 1's in the code
        bit++;
    }

    //putting dataEncoded into data
    if(this->encodingExtended){

        this->p++; //increase parity count if extended

        data = QBitArray(n + 1);

        int oneCnt{};

        data[0] = dataEncoded.count(true) & 1; //extended parity bit at the beginning

        for (int i = 0; i < n; i++) {
            data[i + 1] = dataEncoded[i]; //copy the rest into data
        }
    }

    else data = dataEncoded; //just copy the rest without extending the bit
}

QString HammingCode::getDataStr()
{
    QString ret{};

    int n = this->m;

    for(int i = 0; i < n; i++){
        ret.append(QChar(this->data[i] + '0'));
    }

    return ret;
}

int HammingCode::getP() const{
    return p;
}

int HammingCode::getM() const{
    return m;
}

QBitArray HammingCode::getData(){
    return this->data;
}

QBitArray HammingCode::getReceivedCode() const{
    return receivedCode;
}

bool HammingCode::getEncodingExtended() const{
    return encodingExtended;
}


