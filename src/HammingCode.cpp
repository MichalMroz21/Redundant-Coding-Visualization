#include "HammingCode.hpp"

#include <climits>
#include <QtConcurrent>
#include <QThread>
#include <QtMath>
#include <thread>

HammingCode::HammingCode(QObject *parent) : QObject{parent}{}

void HammingCode::setInitialData(QBitArray data, bool extend, int animationDelay){
    this->data = data;
    this->m = data.size();
    this->p = calculateP();
    this->animationDelayMs = animationDelay;
    this->encodingExtended = extend;
    this->receivedCode.clear();
}

void HammingCode::setInitialData(QString data, bool extend, int animationDelay)
{
    int n = data.size();

    QBitArray bits(n);

    for(int i = 0; i < n; i++){
        bits[i] = (data[i].toLatin1() - '0');
    }

    setInitialData(bits, extend, animationDelay);
}

//Simulate sending some code, it can have 1 bit error
void HammingCode::sendCode(QBitArray send){
    receivedCode = send;
}

void HammingCode::sendCode(QString send){

    QBitArray code(send.size());

    for(int i = 0; i < send.size(); i++){
        code[i] = send[i].toLatin1() - '0';
    }

    sendCode(code);
}

int HammingCode::correctErrorExtended(bool forQML)
{
    int n = this->m + this->p, C{}, P{}, bit{};

    QString belowText{}, initialText{"<font color=\"orange\">C =</font> %1"}, addText{"<font color=\"orange\">C +=</font> %1"};

    if(forQML){
        emit setBelowText(initialText.arg(C));
        QThread::currentThread()->msleep(this->animationDelayMs);

        emit setBelowText(addText.arg(C));
        QThread::currentThread()->msleep(this->animationDelayMs);
    }

    for(int i = 1; i < n; i *= 2){

        int xorVal = 0;

        xorVal ^= receivedCode[i]; //i because extended parity bit must be not used for this

        if(forQML){
            emit turnBitOn(0, i, "red");
            QThread::currentThread()->msleep(this->animationDelayMs);
            emit turnBitOff(0, i);

            belowText = addText.arg(receivedCode[i]);
        }

        for(int j = i + 1; j < n; j++){  //indexing +1, so <= n -> < n

            if(j & (1 << bit)){

                xorVal ^= receivedCode[j]; //indexing +1

                if(forQML){
                    emit turnBitOn(0, j, "red");
                    belowText.append(QString(" ^ %1").arg(receivedCode[j]));
                    emit setBelowText(belowText);

                    QThread::currentThread()->msleep(this->animationDelayMs);
                    emit turnBitOff(0, j);
                }
            }
        }

        if(forQML){
            emit setBelowText(addText.arg(xorVal));
            QThread::currentThread()->msleep(this->animationDelayMs);

            emit setBelowText(addText.arg(QString("%1 * %2").arg(xorVal).arg(i)));
            QThread::currentThread()->msleep(this->animationDelayMs);
        }

        C += xorVal * i;
        bit++;

        if(forQML){
            emit setBelowText(initialText.arg(C));
            QThread::currentThread()->msleep(this->animationDelayMs);
        }
    }

    initialText = "<font color=\"purple\">P =</font> %1";

    if(forQML){
        belowText = QString(initialText).arg(P);

        emit setBelowText(belowText);

        QThread::currentThread()->msleep(this->animationDelayMs);
    }

    for(int i = 0; i < n; i++){

        if(forQML){
            emit turnBitOn(0, i, "red");
            belowText.append(QString(" ^ %1").arg(receivedCode[i]));
            emit setBelowText(belowText);

            QThread::currentThread()->msleep(this->animationDelayMs);
            emit turnBitOff(0, i);
        }

        P ^= receivedCode[i];
    }

    if(C == 0){ //theory from youtube
        if(P == 1){
            qInfo() << "Error occured in extended parity bit, correcting";
            emit setBelowText("Error is in extended parity bit (0)");
            receivedCode[C] = !receivedCode[C];
            return C;
        }
        else{
            qInfo() << "No error";
            emit setBelowText("There is no error!");
            return -1;
        }
    }
    else{
        if(P == 1){
            qInfo() << "Single error occured at " + QString::number(C) + " correcting...";
            emit setBelowText("Error is at index: " + QString::number(C));
            receivedCode[C] = !receivedCode[C];
            return C;
        }
        else{
            qInfo() << "Double error occured that cannot be corrected";
            emit setBelowText("There is a double error, but it can't be calculated where");
            return -2;
        }
    }
}

int HammingCode::correctErrorStandard(bool forQML)
{
    int n = this->m + this->p, C{}, bit{};

    QString belowText{}, initialText{"<font color=\"orange\">C =</font> %1"}, addText{"<font color=\"orange\">C +=</font> %1"};

    if(forQML){
        emit setBelowText(initialText.arg(C));
        QThread::currentThread()->msleep(this->animationDelayMs);

        emit setBelowText(addText.arg(C));
        QThread::currentThread()->msleep(this->animationDelayMs);
    }

    for(int i = 1; i <= n; i *= 2){ //calculate parity bits and add them up with formula: p1 * 1 + p2 * 2 + p3 * 4 + ... = C

        int xorVal = 0;

        xorVal ^= receivedCode[i - 1]; //xor is same as counting 1's

        if(forQML){
            emit turnBitOn(0, i - 1, "red");
            QThread::currentThread()->msleep(this->animationDelayMs);
            emit turnBitOff(0, i - 1);

            belowText = addText.arg(receivedCode[i - 1]);
        }

        for(int j = i + 1; j <= n; j++){

            if(j & (1 << bit)){

                xorVal ^= receivedCode[j - 1];

                if(forQML){
                    emit turnBitOn(0, j - 1, "red");
                    belowText.append(QString(" ^ %1").arg(receivedCode[j - 1]));
                    emit setBelowText(belowText);

                    QThread::currentThread()->msleep(this->animationDelayMs);
                    emit turnBitOff(0, j - 1);
                }
            }
        }

        if(forQML){
            emit setBelowText(addText.arg(xorVal));
            QThread::currentThread()->msleep(this->animationDelayMs);

            emit setBelowText(addText.arg(QString("%1 * %2").arg(xorVal).arg(i)));
            QThread::currentThread()->msleep(this->animationDelayMs);
        }

        C += xorVal * i;
        bit++;

        if(forQML){
            emit setBelowText(initialText.arg(C));
            QThread::currentThread()->msleep(this->animationDelayMs);
        }
    }

    if(C == 0){ //theory from youtube
        qInfo() << "No error!";
        emit setBelowText("No error found!");
        return -1;
    }
    else{
        qInfo() << "Error at position: " << (C - 1);
        emit setBelowText("Error is at position: " + QString::number(C - 1));

        receivedCode[C - 1] = !receivedCode[C - 1];
        return C - 1;
    }
}

//Correct the 1 bit error in received code
int HammingCode::correctError(bool forQML)
{
    if(forQML){
        if(this->encodingExtended){
            static_cast<void>(QtConcurrent::run([=](){
                correctErrorExtended(forQML);
            }));
        }

        else{
            static_cast<void>(QtConcurrent::run([=](){
                correctErrorStandard(forQML);
            }));
        }
    }

    else{
        if(this->encodingExtended) return correctErrorExtended(forQML);
        else return correctErrorStandard(forQML);
    }

    if(forQML) emit loadMainMenuButton();
    return -1;
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
        QThread::currentThread()->msleep(this->animationDelayMs);
        emit pushArray(this->getDataStr());

        QThread::currentThread()->msleep(this->animationDelayMs);
        emit pushEmptyArray(n);       
    }

    for(int i = 0; i < n; i++){

        bool isParity = isPowerTwo(i + 1),
              dataBit = data[dataPtr];

        if(!isParity) dataEncoded[i] = data[dataPtr]; //copying non-parity bits

        if(forQML){
            emit turnBitOn(0, dataPtr, "blue");

            if(!isParity) {
                emit turnBitOn(1, i, "green");
                emit setBit(1, i, dataBit ? "1" : "0");
            }

            else {
                emit turnBitOn(1, i, "red");
                emit setBit(1, i, "0");
            }

            QThread::currentThread()->msleep(this->animationDelayMs);
            emit turnBitOff(1, i);
            emit turnBitOff(0, dataPtr);
        }

        if(!isParity) dataPtr++;
    }

    if(forQML){
        emit deleteArrayAtIndex(0);
        QThread::currentThread()->msleep(this->animationDelayMs);
    }

    int bit = 0;

    for(int i = 1; i <= n; i *= 2){ //calculating parity bits

        int xorVal = 0; //counting number of 1's for each parity bit, xor just signals even/odd count

        QString belowText{}, initialText{"<font color=\"yellow\">Parity Bit " + QString::number(qLn(i)/qLn(2.0)) + " =</font> %1"};

        if(forQML){
            belowText = QString(initialText).arg(dataEncoded[i - 1]);

            emit turnBitOn(0, i - 1, "yellow");
            emit setBelowText(belowText);

            QThread::currentThread()->msleep(this->animationDelayMs);
        }

        for(int j = i + 1; j <= n; j++){

            if(j & (1 << bit)){ //bit manipulation trick

                xorVal ^= dataEncoded[j - 1];

                if(forQML){
                    emit turnBitOn(0, j - 1, "red");
                    belowText.append(QString(" ^ %1").arg(dataEncoded[j - 1]));
                    emit setBelowText(belowText);

                    QThread::currentThread()->msleep(this->animationDelayMs);
                    emit turnBitOff(0, j - 1);
                }
            }
        }

        dataEncoded[i - 1] = xorVal; //keeping even number of 1's in the code
        bit++;

        if(forQML){

            emit setBelowText(QString(initialText).arg(xorVal));
            QThread::currentThread()->msleep(this->animationDelayMs);

            emit turnBitOn(0, i - 1, "green");
            emit setBit(0, i - 1, xorVal ? "1" : "0");

            QThread::currentThread()->msleep(this->animationDelayMs);

            emit turnBitOff(0, i - 1);
            emit setBelowText(QString(""));
        }
    }

    //putting dataEncoded into data
    if(this->encodingExtended){

        this->p++; //increase parity count if extended

        data = QBitArray(n + 1);

        QString belowText{}, initialText{"<font color=\"purple\">Additional Parity Bit =</font> %1"};

        int xorVal{};

        if(forQML){
            belowText = QString(initialText).arg(xorVal);

            emit setBelowText(belowText);

            QThread::currentThread()->msleep(this->animationDelayMs);
        }

        for(int i = 0; i < n; i++){

            if(forQML){
                emit turnBitOn(0, i, "red");
                belowText.append(QString(" ^ %1").arg(dataEncoded[i]));
                emit setBelowText(belowText);

                QThread::currentThread()->msleep(this->animationDelayMs);
                emit turnBitOff(0, i);
            }

            xorVal ^= dataEncoded[i];
        }

        data[0] = xorVal; //extended parity bit at the beginning

        if(forQML){
            emit setBelowText(QString(initialText).arg(xorVal));
            QThread::currentThread()->msleep(this->animationDelayMs);

            emit insertBit(0, 0, xorVal == 1 ? "1" : "0");
            emit turnBitOn(0, 0, "purple");

            QThread::currentThread()->msleep(this->animationDelayMs);

            emit turnBitOff(0, 0);
            emit setBelowText(QString(""));
        }

        for (int i = 0; i < n; i++) {
            data[i + 1] = dataEncoded[i]; //copy the rest into data, doesn't need visualization
        }
    }

    else data = dataEncoded; //just copy the rest without extending the bit

    if(forQML) emit encodingEnd();
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

int HammingCode::getAnimationDelayMs() const
{
    return animationDelayMs;
}

void HammingCode::setAnimationDelayMs(int delay)
{
    this->animationDelayMs = delay;
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


