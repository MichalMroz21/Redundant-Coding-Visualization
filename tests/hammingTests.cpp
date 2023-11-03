#include <gtest/gtest.h>

#include <QSharedPointer>

#include "hammingcode.hpp"

TEST(Hamming, hammingInit){

    QBitArray data = QBitArray::fromBits("1001", 4);

    auto hammingCode = QSharedPointer<HammingCode>(new HammingCode(data));

    ASSERT_EQ(data, hammingCode.data()->getData());
    ASSERT_EQ(4, hammingCode.data()->getM());
    ASSERT_EQ(3, hammingCode.data()->getP());
}

TEST(Hamming, hammingInitParity){

    QBitArray data(1900), data2(19229319);

    auto hammingCode = QSharedPointer<HammingCode>(new HammingCode(data)),
         hammingCode2 = QSharedPointer<HammingCode>(new HammingCode(data2));

    ASSERT_EQ(11, hammingCode.data()->getP());
    ASSERT_EQ(25, hammingCode2.data()->getP());
}

TEST(Hamming, hammingEncoding){

    QBitArray data = QBitArray::fromBits("1110", 4);

    auto hammingCode = QSharedPointer<HammingCode>(new HammingCode(data));

    hammingCode.data()->encodeData();

    ASSERT_STREQ("0010110", hammingCode.data()->getData().bits());

}
