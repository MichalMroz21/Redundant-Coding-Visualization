#include <gtest/gtest.h>

#include <QSharedPointer>

#include "hammingcode.hpp"


//testing the number of parity bits calculations and constructor
TEST(Hamming, hammingInitParity){

    QList<std::tuple<QString, int, int>> dataAndResult{{"1101", 3, 4}, {"10110100110", 4, 11}};

    foreach(auto tuple, dataAndResult){

        QString data = std::get<0>(tuple);
        int expectedP = std::get<1>(tuple), expectedM = std::get<2>(tuple);

        QBitArray bits(data.size());

        for(int i = 0; i < data.size(); i++) bits[i] = (data[i] == '1');

        auto hammingCode = QSharedPointer<HammingCode>(new HammingCode(bits));

        ASSERT_EQ(expectedP, hammingCode.data()->getP());
        ASSERT_EQ(expectedM, hammingCode.data()->getM());
    }
}

////hamming's encoding testing without additional parity bit
TEST(Hamming, hammingEncoding){

    QList<QPair<QString, QString>> dataAndResult{{"1110", "0010110"}, {"11010101100111011111010110", "1111101001011001111011111010110"}};

    foreach(auto pair, dataAndResult){

        QString data = pair.first, expectedResult = pair.second, result{};

        QBitArray bits(data.size());

        for(int i = 0; i < data.size(); i++) bits[i] = (data[i] == '1');

        auto hammingCode = QSharedPointer<HammingCode>(new HammingCode(bits));

        hammingCode.data()->encodeData();

        QBitArray encoded = hammingCode.data()->getData();

        for(int i = 0; i < encoded.size(); i++){
            result.append(encoded.testBit(i) ? '1' : '0');
        }

        ASSERT_STREQ(result.toStdString().c_str(), expectedResult.toStdString().c_str()); //yup
    }
}

//hamming's encoding testing without additional parity bit
TEST(Hamming, hammingEncodingExtended){

    QList<QPair<QString, QString>> dataAndResult{{"1110", "10010110"}, {"11010101100111011111010110", "11111101001011001111011111010110"}};

    foreach(auto pair, dataAndResult){

        QString data = pair.first, expectedResult = pair.second, result{};

        QBitArray bits(data.size());

        for(int i = 0; i < data.size(); i++) bits[i] = (data[i] == '1');

        auto hammingCode = QSharedPointer<HammingCode>(new HammingCode(bits));

        hammingCode.data()->encodeData(true);

        QBitArray encoded = hammingCode.data()->getData();

        for(int i = 0; i < encoded.size(); i++){
            result.append(encoded.testBit(i) ? '1' : '0');
        }

        ASSERT_STREQ(result.toStdString().c_str(), expectedResult.toStdString().c_str()); //yup
    }
}
