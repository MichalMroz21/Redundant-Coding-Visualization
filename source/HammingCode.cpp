#include "HammingCode.hpp"

#include <QtConcurrent>
#include <QThread>
#include <QtMath>

HammingCode::HammingCode(QObject *parent) : QObject{parent}{}

void HammingCode::setInitialData(QBitArray data, bool extend, int animationDelay, bool infiniteWait){
    this->data = data;
    this->m = data.size();
    this->p = calculateP();
    this->animationDelayMs = animationDelay;
    this->encodingExtended = extend;
    this->infiniteWait = infiniteWait;
    this->receivedCode.clear();
}

void HammingCode::setInitialData(QString data, bool extend, int animationDelay, bool infiniteWait)
{
    int n = data.size();

    QBitArray bits(n);

    for(int i = 0; i < n; i++){
        bits[i] = (data[i].toLatin1() - '0');
    }

    setInitialData(bits, extend, animationDelay, infiniteWait);
}

//Simulate sending some code, it can have 1 bit error
void HammingCode::sendCode(QBitArray send){
    this->receivedCode = send;
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

    QString belowText{}, initialText{"<font color=\"orange\">C =</font> %1"}, addText{"<font color=\"orange\">C +=</font> %1"}, belowTextExt{};

    if(forQML){
        emit setBelowText(initialText.arg(C));
        this->waitForQml();

        emit setBelowText(addText.arg(C));
        this->waitForQml();
    }

    for(int i = 1; i < n; i *= 2){

        int xorVal = 0;

        xorVal ^= receivedCode[i]; //i because extended parity bit must be not used for this

        if(forQML){
            emit turnBitOn(0, i, "red");
            this->waitForQml();
            emit turnBitOff(0, i);

            belowText = addText.arg(receivedCode[i]);
            belowTextExt = addText.arg(this->getSymbol(i));
        }

        for(int j = i + 1; j < n; j++){  //indexing +1, so <= n -> < n

            if(j & (1 << bit)){

                xorVal ^= receivedCode[j]; //indexing +1

                if(forQML){
                    emit turnBitOn(0, j, "red");
                    belowText.append(QString(" ^ %1").arg(receivedCode[j]));
                    emit setBelowText(belowText);
                    belowTextExt.append(QString(" ^ %1").arg(this->getSymbol(j)));
                    emit setBelowTextExtended(belowTextExt);

                    this->waitForQml();
                    emit turnBitOff(0, j);
                }
            }
        }

        if(forQML){
            emit setBelowText(addText.arg(xorVal));
            this->waitForQml();

            emit setBelowText(addText.arg(QString("%1 * %2").arg(xorVal).arg(i)));
            int powerOfTwo = qFloor(qLn(i) / qLn(2.0));
            belowTextExt.append(QString(" * 2^%1").arg(powerOfTwo));
            emit setBelowTextExtended(belowTextExt);
            this->waitForQml();
        }

        C += xorVal * i;
        bit++;

        if(forQML){
            emit setBelowText(initialText.arg(C));
            emit setBelowTextExtended(QString(""));
            this->waitForQml();
        }
    }

    initialText = "<font color=\"purple\">P =</font> ";

    if(forQML){
        belowText = QString(initialText);
        belowTextExt = QString(initialText);

        emit setBelowText(belowText);
        emit setBelowTextExtended(belowTextExt);

        this->waitForQml();
    }

    for(int i = 0; i < n; i++){

        if(forQML){
            emit turnBitOn(0, i, "red");
            belowText.append(QString(i == 0 ? "%1" : " ^ %1").arg(receivedCode[i]));
            emit setBelowText(belowText);
            belowTextExt.append(QString(i == 0 ? "%1" : " ^ %1").arg(this->getSymbol(i)));
            emit setBelowTextExtended(belowTextExt);

            this->waitForQml();
            emit turnBitOff(0, i);
        }

        P ^= receivedCode[i];
    }

    emit setBelowTextExtended(QString(""));
    emit setBelowText(QString("C = %1, P = %2").arg(C).arg(P));

    int ret;
    if(C == 0){ //theory from youtube
        if(P == 1){
            //qInfo() << "Error occured in extended parity bit, correcting";
            //emit setBelowTextExtended("Error is in extended parity bit (0)");

            qInfo() << "Błąd wystąpił w rozszerzonym bicie parzystości, poprawianie";
            emit setBelowTextExtended("Błąd występuje w rozszerzonym bicie parzystości (0)");
//            receivedCode[C] = !receivedCode[C];
            ret = C;
            this->setError(1);
        }
        else{
            //qInfo() << "No error";
            //emit setBelowTextExtended("There is no error!");

            qInfo() << "Brak błędów";
            emit setBelowTextExtended("Nie ma błędów!");
            ret = -1;
            this->setError(0);
        }
    }
    else{
        if(P == 1){
            //qInfo() << "Single error occured at " + QString::number(C) + " correcting...";
            //emit setBelowTextExtended("Error is at index: " + QString::number(C));

            qInfo() << "Pojedynczy błąd wystąpił w " + QString::number(C) + ", poprawianie...";
            emit setBelowTextExtended("Błąd znajduje się na pozycji: " + QString::number(C + 1));
//            receivedCode[C] = !receivedCode[C];
            ret = C;
            this->setError(C + 1);
        }
        else{
            //qInfo() << "Double error occured that cannot be corrected";
            //emit setBelowTextExtended("There is a double error, but it can't be calculated where");

            qInfo() << "Wystąpił podwójny błąd, który nie może być poprawiony";
            emit setBelowTextExtended("Występuje podwójny błąd, który nie może zostać wyliczony, gdzie występuje");
            ret = -2;
        }
    }

    if(forQML) emit endErrorCorrection(C, P);
    return ret;
}

int HammingCode::correctErrorStandard(bool forQML)
{
    int n = this->m + this->p, C{}, bit{};

    QString belowText{}, initialText{"<font color=\"orange\">C =</font> %1"}, addText{"<font color=\"orange\">C +=</font> %1"}, belowTextExt{};

    if(forQML){
        emit setBelowText(initialText.arg(C));
        this->waitForQml();

        emit setBelowText(addText.arg(C));
        this->waitForQml();
    }

    for(int i = 1; i <= n; i *= 2){ //calculate parity bits and add them up with formula: p1 * 1 + p2 * 2 + p3 * 4 + ... = C

        int xorVal = 0;

        xorVal ^= receivedCode[i - 1]; //xor is same as counting 1's

        if(forQML){
            emit turnBitOn(0, i - 1, "red");
            this->waitForQml();
            emit turnBitOff(0, i - 1);

            belowText = addText.arg(receivedCode[i - 1]);
            belowTextExt = addText.arg(this->getSymbol(i - 1));
        }

        for(int j = i + 1; j <= n; j++){

            if(j & (1 << bit)){

                xorVal ^= receivedCode[j - 1];

                if(forQML){
                    emit turnBitOn(0, j - 1, "red");
                    belowText.append(QString(" ^ %1").arg(receivedCode[j - 1]));
                    emit setBelowText(belowText);

                    this->waitForQml();
                    emit turnBitOff(0, j - 1);
                    belowTextExt.append(QString(" ^ %1").arg(this->getSymbol(j - 1)));
                    emit setBelowTextExtended(belowTextExt);
                }
            }
        }

        if(forQML){
            emit setBelowText(addText.arg(xorVal));
            this->waitForQml();

            emit setBelowText(addText.arg(QString("%1 * %2").arg(xorVal).arg(i)));
            int powerOfTwo = qFloor(qLn(i) / qLn(2.0));
            belowTextExt.append(QString(" * 2^%1").arg(powerOfTwo));
            emit setBelowTextExtended(belowTextExt);
            this->waitForQml();
        }

        C += xorVal * i;
        bit++;

        if(forQML){
            emit setBelowText(initialText.arg(C));
            emit setBelowTextExtended(QString(""));
            this->waitForQml();
        }
    }

    this->setError(C);
    int ret;
    emit setBelowText("C = " + QString::number(C));
    if(C == 0){ //theory from youtube
        //qInfo() << "No error!";
        //emit setBelowTextExtended("No error found!");

        qInfo() << "Brak błędów!";
        emit setBelowTextExtended("Nie znaleziono błędóww!");
        ret = -1;
    }
    else{
        //qInfo() << "Error at position: " << (C - 1);
        //emit setBelowTextExtended("Error is at position: " + QString::number(C));

        qInfo() << "Błąd na pozycji: " << (C - 1);
        emit setBelowTextExtended("Błąd znajduje się na pozycji: " + QString::number(C));
//        receivedCode[C - 1] = !receivedCode[C - 1];
        ret = C - 1;
    }

    if(forQML) emit endErrorCorrection(C, -1);
    return ret;
}

//Correct the 1 bit error in received code
int HammingCode::correctError(bool forQML)
{
    if(forQML){
        if(this->encodingExtended){
            static_cast<void>(QtConcurrent::run([=, this](){
                correctErrorExtended(forQML);
            }));
        }

        else{
            static_cast<void>(QtConcurrent::run([=, this](){
                correctErrorStandard(forQML);
            }));
        }
    }

    else{
        if(this->encodingExtended) return correctErrorExtended(forQML);
        else return correctErrorStandard(forQML);
    }

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
        static_cast<void>(QtConcurrent::run([=, this](){
            encodeDataAsync(forQML);
        }));
    }

    else{
        encodeDataAsync(forQML);
    }
}

void HammingCode::encodeDataAsync(bool forQML){
    this->setSymbols();

    int n = this->m + this->p, dataPtr{};

    QBitArray dataEncoded(n);

    if(forQML){
        this->waitForQml();
        emit pushArray(this->getDataStr(), false);

        this->waitForQml();
        emit pushEmptyArray(n);       
    }

    for(int i = 0; i < n; i++){

        bool isParity = isPowerTwo(i + 1),
              dataBit = data[dataPtr];

        if(!isParity) dataEncoded[i] = data[dataPtr]; //copying non-parity bits

        if(forQML){
            emit turnBitOn(0, dataPtr, "light blue");

            if(!isParity) {
                emit turnBitOn(1, i, "green");
                emit setBit(1, i, dataBit ? "1" : "0");
            }

            else {
                emit turnBitOn(1, i, "red");
                emit setBit(1, i, "0");
            }

            this->waitForQml();
            emit turnBitOff(1, i);
            emit turnBitOff(0, dataPtr);
        }

        if(!isParity) dataPtr++;
    }

    if(forQML){
        emit deleteArrayAtIndex(0);
        this->waitForQml();
    }

    int bit = 0;

    for(int i = 1; i <= n; i *= 2){ //calculating parity bits

        int xorVal = 0; //counting number of 1's for each parity bit, xor just signals even/odd count
        //QString belowText{}, initialText{"<font color=\"Blue\">Parity Bit " + QString::number(qLn(i)/qLn(2.0)) + " =</font> %1"}, belowTextExt{};

        QString belowText{}, initialText{"<font color=\"Blue\">Bit parzystości " + QString::number(qLn(i)/qLn(2.0)) + " =</font> %1"}, belowTextExt{};
        if(forQML){
            belowText = QString(initialText).arg(dataEncoded[i - 1]);
            belowTextExt = QString("%1 = %1").arg(this->getSymbol(i - 1));

            emit turnBitOn(0, i - 1, "yellow");
            emit setBelowText(belowText);
            emit setBelowTextExtended(belowTextExt);

            this->waitForQml();
        }

        for(int j = i + 1; j <= n; j++){

            if(j & (1 << bit)){ //bit manipulation trick

                xorVal ^= dataEncoded[j - 1];

                if(forQML){
                    emit turnBitOn(0, j - 1, "red");
                    belowText.append(QString(" ^ %1").arg(dataEncoded[j - 1]));
                    emit setBelowText(belowText);
                    belowTextExt.append(QString(" ^ %1").arg(this->getSymbol(j - 1)));
                    emit setBelowTextExtended(belowTextExt);

                    this->waitForQml();
                    emit turnBitOff(0, j - 1);
                }
            }
        }

        dataEncoded[i - 1] = xorVal; //keeping even number of 1's in the code
        bit++;

        if(forQML){

            emit setBelowText(QString(initialText).arg(xorVal));
            this->waitForQml();

            emit turnBitOn(0, i - 1, "green");
            emit setBit(0, i - 1, xorVal ? "1" : "0");

            this->waitForQml();

            emit turnBitOff(0, i - 1);
            emit setBelowText(QString(""));
            emit setBelowTextExtended(QString(""));
        }
    }

    //putting dataEncoded into data
    if(this->encodingExtended){

        this->p++; //increase parity count if extended

        data = QBitArray(n + 1);
        //QString belowText{}, initialText{"<font color=\"purple\">Additional Parity Bit =</font> %1"}, belowTextExt{"p = p"};

        QString belowText{}, initialText{"<font color=\"purple\">Dodatkowy bit parzystości =</font> %1"}, belowTextExt{"p = p"};
        this->symbols.prepend(QString("p"));

        int xorVal{};

        if(forQML){
            belowText = QString(initialText).arg(xorVal);

            emit setBelowText(belowText);
            emit setBelowTextExtended(belowTextExt);

            this->waitForQml();
        }

        for(int i = 0; i < n; i++){

            if(forQML){
                emit turnBitOn(0, i, "red");
                belowText.append(QString(" ^ %1").arg(dataEncoded[i]));
                emit setBelowText(belowText);
                belowTextExt.append(QString(" ^ %1").arg(this->getSymbol(i + 1)));
                emit setBelowTextExtended(belowTextExt);

                this->waitForQml();
                emit turnBitOff(0, i);
            }

            xorVal ^= dataEncoded[i];
        }

        data[0] = xorVal; //extended parity bit at the beginning

        if(forQML){
            emit setBelowText(QString(initialText).arg(xorVal));
            this->waitForQml();

            emit insertBit(0, 0, xorVal == 1 ? "1" : "0", true);
            emit turnBitOn(0, 0, "purple");

            this->waitForQml();

            emit turnBitOff(0, 0);
            emit setBelowText(QString(""));
            emit setBelowTextExtended(QString(""));
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

QString HammingCode::getReceivedCode() {
    QString ret{};

    for(int i = 0; i < this->receivedCode.size(); i++){
        ret.append(QChar(this->receivedCode[i] + '0'));
    }

    return ret;
}

bool HammingCode::getEncodingExtended() const{
    return encodingExtended;
}

void HammingCode::setInfiniteWait(bool value)
{
    this->infiniteWait = value;
}

void HammingCode::waitForQml()
{
    if (this->infiniteWait) {
        while (!this->buttonPressed && this->infiniteWait);
        this->buttonPressed = false;
    }
    else
        QThread::currentThread()->msleep(this->animationDelayMs);
}

void HammingCode::pressButton()
{
    this->buttonPressed = true;
}

// generation matrix created based on the error matrix
// each row is one bit of input, each column means whether the bit is used
// on this position in the encoded data (either included in parity bit or this position simply is this bit)
QString HammingCode::getGenerationMatrixStr()
{
    QString ret{};

    int numOfCols = this->m + this->p;
    int numOfRows = this->m;

    int position = 3; // position in the encoded data of non-parity bit, 1 and 2 are parity
    for(int i = 0; i < numOfRows; i++) {
        QString parityRow{};
        while (isPowerTwo(position)) position++; // has to be non-parity
        int n = i;
        int parityBitCounter = 0;
        for (int j = 0; j < numOfCols; j++) {
            bool bitUsedInCurrentPosition = false;
            bool isParity = isPowerTwo(j + 1);
            if(!isParity) {
                bitUsedInCurrentPosition = ((n--) == 0); // bit used at n-th non-parity position
                ret.append(QChar(bitUsedInCurrentPosition ? '1' : '0'));
            } else {
                bitUsedInCurrentPosition = (1 << parityBitCounter) & position;
                parityBitCounter++;
                parityRow.append(QChar(bitUsedInCurrentPosition ? '1' : '0'));
            }
        }
        ret.append(parityRow); // non-parity columns first - makes the matrix canonical
        if (i != numOfRows - 1) ret.append(QChar('\n'));
        position++;
    }

    return ret;
}

// columns are binary numbers in order beginning at 1
// 0 0 0 1 1 1 1
// 0 1 1 0 0 1 1
// 1 0 1 0 1 0 1
// basically, the last row is 0 then 1 then 0
// previous is two 0s, two 1s, etc, except one fewer zero at the beginning (because start at 1 not 0)
QString HammingCode::getErrorMatrixStr() {
    QString ret{};

    int numOfCols = this->m + this->p;
    int numOfRows = this->p;

    for(int i = 0; i < numOfRows; i++) {
        QString row{};

        int zeroCount = (1 << i) - 1;
        int colCounter = 0;
        for (int j = 0; j < zeroCount; j++) {
            row.append(QChar('0'));
            colCounter++;
        }
        bool insertZero = false;
        int howManyInARow = (1 << i);
        while (colCounter < numOfCols) {
            for (int j = 0; j < howManyInARow && colCounter < numOfCols; j++) {
                row.append(QChar(insertZero ? '0' : '1'));
                colCounter++;
            }
            insertZero = !insertZero;
        }
        if (i != 0) row.append(QChar('\n'));
        ret.prepend(row);
    }
    return ret;
}


QString HammingCode::getEncodedStr() {
    QString ret{};
    QString error = getError();
    QString received = getReceivedCode();

    // Perform XOR operation character by character
    for (int i = 0; i < error.length(); ++i) {
        // Convert characters to integers and perform XOR
        int errorBit = error.at(i).digitValue();
        int receivedBit = received.at(i).digitValue();
        int xorResult = errorBit ^ receivedBit;

        ret.append(QString::number(xorResult));
    }

    return ret;

}

QString HammingCode::getError() {
    return this->error;
}

QString HammingCode::getSyndrome() {
    return this->syndrome;
}

void HammingCode::setError(int C)
{
    this->error = {};
    int syndromeLength = qCeil(qLn(this->m + this->p) / qLn(2));
    this->syndrome = QString::number(C, 2).rightJustified(syndromeLength, '0'); // converts to binary, left padded with 0s
    for (int i = 0; i < this->m + this->p; i++) {
        this->error.append(QChar(i == C - 1 ? '1' : '0'));
    }

}

QString HammingCode::getSymbol(int index) {
    return this->symbols[index];
}

void HammingCode::setSymbols() {
    this->symbols = {};
    int parityIndex = 0;
    int dataIndex = 0;
    for (int i = 0; i < this->m + this->p; i++) {
        QString symbol;
        if (isPowerTwo(i + 1)) symbol = QString("p%1").arg(parityIndex++);
        else symbol = QString("d%1").arg(dataIndex++);
        this->symbols << symbol;
    }
}

QString HammingCode::getDecodedStr() {
    QString ret {};
    QString encoded = getEncodedStr();
    for (int i = this->encodingExtended ? 1 : 0; i < this->m + this->p; i++) {
        if (isPowerTwo(this->encodingExtended ? i : i + 1)) continue;
        ret.append(encoded.at(i));
    }
    return ret;

}
