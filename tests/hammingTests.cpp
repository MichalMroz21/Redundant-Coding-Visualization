#include <gtest/gtest.h>

#include <QSharedPointer>

#include "HammingCode.hpp"

//test data generated with: https://www.dcode.fr/hamming-error-correction and https://www.browserling.com/tools/random-bin

//testing the number of parity bits calculations and constructor
TEST(Hamming, hammingInitParity){

    //data, expected parity number, expected data number
    QList<std::tuple<QString, int, int>> dataAndResult{{"1101", 3, 4}, {"10110100110", 4, 11}, {"11010010010", 4, 11}, {"11011111011111101001010111", 5, 26},
                                                       {"000010110010110001110101011001001110111001001110000100111", 6, 57},
                                                       {"001100001000101010001001010000101111110001111100010000100101100010101011001000100110010011100000111110011010000101011010", 7, 120},
                                                       {"0100101110111011100110101011001111110011001110101110011110111101110100111111101010000001010111000010011010010000101001010000100011111110110001101011101100001111000000101110111111000010011010111011010100110000101111001111001010111110111011000100011", 8, 247}};

    foreach(auto tuple, dataAndResult){

        QString data = std::get<0>(tuple);

        int expectedP = std::get<1>(tuple), expectedM = std::get<2>(tuple);

        QBitArray bits(data.size());

        for(int i = 0; i < data.size(); i++) bits[i] = (data[i] == '1');

        auto hammingCode = QSharedPointer<HammingCode>(new HammingCode());
        hammingCode.data()->setInitialData(bits, false);

        ASSERT_EQ(expectedP, hammingCode.data()->getP());
        ASSERT_EQ(expectedM, hammingCode.data()->getM());
    }
}

////hamming's encoding testing without additional parity bit
TEST(Hamming, hammingEncoding){

    //data, expected encoded data
    QList<QPair<QString, QString>> dataAndResult{{"1110", "0010110"}, {"11010101100111011111010110", "1111101001011001111011111010110"}, {"11100001000", "001111010001000"},
                                                 {"00101010110", "110101001010110"}, {"01111000100000001001010001", "1100111110001000000001001010001"}, {"11100001011010010001110100", "1111110100010110010010001110100"},
                                                 {"011000010010001110110100100001101001001110111001000111101", "010011010001001000011101101001000001101001001110111001000111101"}, {"111011011100001010101101000000101101100001000100101101100", "001011001101110100010101011010000000101101100001000100101101100"},
                                                 {"100001111100101100000010010011000110011100010101111110001101110111110101001001000000100001101011110110101000000100010010", "1011000101111101010110000001001000110001100111000101011111100010101110111110101001001000000100001101011110110101000000100010010"},
                                                 {"1101000110001100111111010000010111010000010010000010100010010011100110111101100010111001001000101000000101101011100111000111000000010110110111101100001000001001011110110110111011111101111100101010101000111010011000010100001001101100111000101110100", "011110100001100101100111111010010001011101000001001000001010001100100111001101111011000101110010010001010000001011010111001110010111000000010110110111101100001000001001011110110110111011111101111100101010101000111010011000010100001001101100111000101110100"}};

    foreach(auto pair, dataAndResult){

        QString data = pair.first, expectedResult = pair.second, result{};

        QBitArray bits(data.size());

        for(int i = 0; i < data.size(); i++) bits[i] = (data[i] == '1');

        auto hammingCode = QSharedPointer<HammingCode>(new HammingCode());
        hammingCode.data()->setInitialData(bits, false);

        hammingCode.data()->encodeData(false);

        QBitArray encoded = hammingCode.data()->getData();

        for(int i = 0; i < encoded.size(); i++){
            result.append(encoded.testBit(i) ? '1' : '0');
        }

        ASSERT_STREQ(result.toStdString().c_str(), expectedResult.toStdString().c_str()); //yup
    }
}

//hamming's encoding testing with additional parity bit
TEST(Hamming, hammingEncodingExtended){

    //data, expected encoded data with additional parity bit
    QList<QPair<QString, QString>> dataAndResult{{"1110", "10010110"}, {"11001110001", "1101010001110001"}, {"00011001000", "0010000101001000"},
                                                 {"10111001100", "0011001111001100"}, {"01100011010010001110100011", "10100110100110101010001110100011"},
                                                 {"00101110011001011111011111", "10100010011100111001011111011111"}, {"11111100001100100001000001", "00010111111000010100100001000001"},
                                                 {"011011111110011011000101011001111010111111011100100000101", "1110111001111111000110110001010101001111010111111011100100000101"},
                                                 {"101000111000101111001101010101000010101101100100100101110", "0011101010011100101011110011010100101000010101101100100100101110"},
                                                 {"010011010101101101110000010011001001011010110011111101110110010100000111001011001100100001011101100111101110011000000010", "11001100111010101110110111000001000110010010110101100111111011101110010100000111001011001100100001011101100111101110011000000010"},
                                                 {"011101111111000111111010000011000001011010110011011110000110011111100110001100000111110110011101101001110100111000010000", "10100111101111111100011111101000100110000010110101100110111100001110011111100110001100000111110110011101101001110100111000010000"},
                                                 {"1111101100100000110001111001011110100000100000001101001111110100000000001010100100110011111110001111011100101110010101101000010010011100101001001010110100011000011011111001011000100010110111110111011100000110011001111110101010100000101001100100101", "0111011101011001000000110001111010101111010000010000000110100111011101000000000010101001001100111111100011110111001011100101011001000010010011100101001001010110100011000011011111001011000100010110111110111011100000110011001111110101010100000101001100100101"}};

    foreach(auto pair, dataAndResult){

        QString data = pair.first, expectedResult = pair.second, result{};

        QBitArray bits(data.size());

        for(int i = 0; i < data.size(); i++) bits[i] = (data[i] == '1');

        auto hammingCode = QSharedPointer<HammingCode>(new HammingCode());
        hammingCode.data()->setInitialData(bits, true);

        hammingCode.data()->encodeData(false);

        QBitArray encoded = hammingCode.data()->getData();

        for(int i = 0; i < encoded.size(); i++){
            result.append(encoded.testBit(i) ? '1' : '0');
        }

        ASSERT_STREQ(result.toStdString().c_str(), expectedResult.toStdString().c_str());
    }
}

//Test correcting errors after sending and receiving some message
TEST(Hamming, hammingErrorCorrection){

    //data, encoded data in Hamming with some or none error (flip selected bit), expectedErrorIndex (index (counted from 0) of that flipped bit)
    QList<std::tuple<QString, QString, int>> dataAndResult{{"1110", "0010111", 6}, {"11010101100111011111010110", "1111101001011001111011110010110", 24},
                                                           {"0101", "0100101", -1}, {"0101", "0110101", 2}, {"0101", "0100100", 6},
                                                           {"11100010000", "111011010010000", -1}, {"11100010000", "111011010011000", 11},
                                                           {"10010101111000011011000010", "0110001001011111000011011000010", -1},
                                                           {"10010101111000011011000010", "0110001001011111000011011010010", 26}};

    foreach(auto tuple, dataAndResult){

        QString data = std::get<0>(tuple), sendStr = std::get<1>(tuple);

        int expectedErrorPos = std::get<2>(tuple);

        QBitArray bits(data.size()), toBeSent(sendStr.size());

        for(int i = 0; i < data.size(); i++) bits[i] = (data[i] == '1');
        for(int i = 0; i < sendStr.size(); i++) toBeSent[i] = (sendStr[i] == '1');

        auto hammingCode = QSharedPointer<HammingCode>(new HammingCode());
        hammingCode.data()->setInitialData(bits, false);

        hammingCode.data()->encodeData(false);
        hammingCode->sendCode(toBeSent);

        int errorPos = hammingCode.data()->correctError();

        ASSERT_EQ(errorPos, expectedErrorPos);
    }
}


//Test correcting errors in extended encodings after sending and receiving some message
TEST(Hamming, hammingErrorCorrectionExtended){

    //data, encoded data in Hamming with some or none error (flip selected bit), expectedErrorIndex (index (counted from 0) of that flipped bit)
    QList<std::tuple<QString, QString, int>> dataAndResult{{"1110", "10110110", 2}, {"1110", "10010111", 7}, {"0111", "00101111", 2}, {"0111", "11001111", -2},
                                                           {"0111", "00001111", -1}, {"11101011010011011110011010", "00011110010111101011011110011010", 13},
                                                           {"11101011010011011110011010", "10011110010110101011011110011010", 0},
                                                           {"11101011010011011110011010", "00011110010110101111011110011011", -2},
                                                           {"100101011011101000100011100100110110101000010100111100100", "0101000100101101011010001000111000100110110101000010100111100101", 63},
                                                           {"1110001111101101100001110001100011000111001111011111010100110101001111011010010010000010110001001101011111101000100101100110100001010101100000001101011011100100001010111001101010100110001001110101011001001110001111000100010011011011110011111100000", "1001111000011111001101100001110110110001100011100111101111101010101101010011110110100100100000101100010011010111111010001001011010110100001010101100000001101011011100100001010111001101010100110001001110101011001001110001111000100010011011011110011111100000", 31}};

    foreach(auto tuple, dataAndResult){

        QString data = std::get<0>(tuple), sendStr = std::get<1>(tuple);

        int expectedErrorPos = std::get<2>(tuple);

        QBitArray bits(data.size()), toBeSent(sendStr.size());

        for(int i = 0; i < data.size(); i++) bits[i] = (data[i] == '1');
        for(int i = 0; i < sendStr.size(); i++) toBeSent[i] = (sendStr[i] == '1');

        auto hammingCode = QSharedPointer<HammingCode>(new HammingCode());
        hammingCode.data()->setInitialData(bits, true);

        hammingCode.data()->encodeData(false);
        hammingCode->sendCode(toBeSent);

        int errorPos = hammingCode.data()->correctError();

        ASSERT_EQ(errorPos, expectedErrorPos);
    }
}
