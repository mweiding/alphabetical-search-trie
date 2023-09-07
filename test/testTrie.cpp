#include <sstream>
#include "testTrie.h"
#include "gtest/gtest.h"

TEST(BasicTest, ConstructDestructEmpty) {
Trie<int> trie;
EXPECT_TRUE(trie.empty());
Trie<int>* t = new Trie<int>;
EXPECT_TRUE(t->empty());
delete t;
}

TEST(BasicTest, InsertSimple) {
Trie<string> trie;
Trie<string>::value_type t("wer", "who");
trie.insert(t);
EXPECT_FALSE(trie.empty());
}

TEST(BasicTest, InsertPrint) {
Trie<string> trie;
Trie<string>::value_type t("wer", "who");
trie.insert(t);
stringstream strstr;
strstr << trie;
string res("w\n"
           "  e\n"
           "    r\n"
           "      :who\n");
EXPECT_STREQ(res.c_str(), strstr.str().c_str());
}

TEST(BasicTest, InsertComplex) {
Trie<string> trie;
Trie<string>::value_type t1("wer", "who");
Trie<string>::value_type t2("wir", "we");
Trie<string>::value_type t3("wird", "will");
trie.insert(t1);
trie.insert(t2);
trie.insert(t3);
stringstream strstr;
strstr << trie;
string res("w\n"
           "  e\n"
           "    r\n"
           "      :who\n"
           "  i\n"
           "    r\n"
           "      :we\n"
           "      d\n"
           "        :will\n");
EXPECT_STREQ(res.c_str(), strstr.str().c_str());
}

TEST(BasicTest, InsertComplexDifferent) {
Trie<string> trie;
Trie<string>::value_type t1("bart", "beard");
Trie<string>::value_type t2("stift", "pen");
Trie<string>::value_type t3("sonne", "sun");
Trie<string>::value_type t4("wir", "we");
Trie<string>::value_type t5("wird", "will");
trie.insert(t1);
trie.insert(t2);
trie.insert(t3);
trie.insert(t4);
trie.insert(t5);
stringstream strstr;
strstr << trie;
string res(
        "b\n"
        "  a\n"
        "    r\n"
        "      t\n"
        "        :beard\n"
        "s\n"
        "  o\n"
        "    n\n"
        "      n\n"
        "        e\n"
        "          :sun\n"
        "  t\n"
        "    i\n"
        "      f\n"
        "        t\n"
        "          :pen\n"
        "w\n"
        "  i\n"
        "    r\n"
        "      :we\n"
        "      d\n"
        "        :will\n");
EXPECT_STREQ(res.c_str(), strstr.str().c_str());
}


TEST(BasicTest, InsertDouble) {
Trie<string> trie;
Trie<string>::value_type t1("wer", "woh");
Trie<string>::value_type t2("wer", "who");
trie.insert(t1);
trie.insert(t2);
stringstream strstr;
strstr << trie;
string res("w\n"
           "  e\n"
           "    r\n"
           "      :who\n");
EXPECT_STREQ(res.c_str(), strstr.str().c_str());
}

TEST(BasicTest, EraseSimple) {
Trie<string> trie;
Trie<string>::value_type t1("wer", "who");
trie.insert(t1);
trie.erase("wer");
EXPECT_TRUE(trie.empty());
}

TEST(BasicTest, EraseFail) {
Trie<string> trie;
Trie<string>::value_type t1("wer", "who");
trie.insert(t1);
trie.erase("was");
EXPECT_FALSE(trie.empty());
}

TEST(BasicTest, EraseComplex) {
Trie<string> trie;
Trie<string>::value_type t1("wer", "who");
Trie<string>::value_type t2("wir", "we");
Trie<string>::value_type t3("wird", "will");
trie.insert(t1);
trie.insert(t2);
trie.insert(t3);
trie.erase("wir");
stringstream strstr;
strstr << trie;
string res("w\n"
           "  e\n"
           "    r\n"
           "      :who\n"
           "  i\n"
           "    r\n"
           "      d\n"
           "        :will\n");
EXPECT_STREQ(res.c_str(), strstr.str().c_str());
}

TEST(BasicTest, Clear) {
Trie<string> trie;
Trie<string>::value_type t1("wer", "who");
Trie<string>::value_type t2("wir", "we");
Trie<string>::value_type t3("wird", "will");
trie.insert(t1);
trie.insert(t2);
trie.insert(t3);
trie.clear();
EXPECT_TRUE(trie.empty());
}

TEST(IteratorTest, Begin) {
Trie<string> trie;
Trie<string>::value_type t1("wer", "who");
trie.insert(t1);
EXPECT_TRUE(trie.begin() != trie.end());
}

TEST(IteratorTest, Star) {
Trie<string> trie;
Trie<string>::value_type t1("wer", "who");
trie.insert(t1);
Trie<string>::iterator it = trie.begin();
EXPECT_STREQ((*it).first.c_str(), "wer");
EXPECT_STREQ((*it).second.c_str(), "who");
}

TEST(IteratorTest, PlusPlus) {
Trie<string> trie;
Trie<string>::value_type t1("wer", "who");
trie.insert(t1);
Trie<string>::iterator it = trie.begin();
Trie<string>::iterator it2 = it;
EXPECT_TRUE(it == it2);
Trie<string>::iterator it3 = it++;
EXPECT_TRUE(it3 == trie.begin());
EXPECT_TRUE(it == trie.end());
Trie<string>::iterator it4 = ++it3;
EXPECT_TRUE(it4 == trie.end());
}

TEST(IteratorTest, IncrementEnd) {
Trie<string> trie;
Trie<string>::value_type t1("wer", "who");
trie.insert(t1);
Trie<string>::iterator it = trie.end();
it++;
EXPECT_TRUE(it == trie.end());
}

TEST(IteratorTest, DoubleIncrement) {
Trie<string> trie;
Trie<string>::value_type t1("wer", "who");
Trie<string>::value_type t2("wir", "we");
Trie<string>::value_type t3("wird", "will");
trie.insert(t1);
trie.insert(t2);
trie.insert(t3);
Trie<string>::iterator it = trie.begin();
it++;
it++;
Trie<string>::value_type expected("wird", "will");
EXPECT_EQ(*it, expected);
}

TEST(TrieIteratorTest, DoublePostIncrement) {
Trie<string> trie;
Trie<string>::value_type t1("wer", "who");
Trie<string>::value_type t2("wir", "we");
Trie<string>::value_type t3("wird", "will");
trie.insert(t1);
trie.insert(t2);
trie.insert(t3);
Trie<string>::TrieIterator it = trie.begin();
Trie<string>::value_type t4 = *(it++);
Trie<string>::value_type t5 = *(it++);
Trie<string>::value_type expected1("wer", "who");
Trie<string>::value_type expected2("wir", "we");
EXPECT_EQ(t1, expected1);
EXPECT_EQ(t2, expected2);
}

TEST(TrieIteratorTest, EmptyTrie) {
Trie<string> trie;
Trie<string>::TrieIterator it = trie.begin();
Trie<string>::TrieIterator end = trie.end();
EXPECT_EQ(it, end);
EXPECT_FALSE(it != end);
EXPECT_TRUE(it == end);
}

TEST(IteratorTest, ForLoop) {
Trie<string> trie;
Trie<string>::value_type t1("wer", "who");
Trie<string>::value_type t2("wir", "we");
Trie<string>::value_type t3("wird", "will");
trie.insert(t1);
trie.insert(t2);
trie.insert(t3);
size_t count = 0;
for ( Trie<string>::iterator it = trie.begin(); it != trie.end(); ++it ){
switch( count ) {
case 0:
EXPECT_STREQ((*it).first.c_str(), "wer");
EXPECT_STREQ((*it).second.c_str(), "who");
break;
case 1:
EXPECT_STREQ((*it).first.c_str(), "wir");
EXPECT_STREQ((*it).second.c_str(), "we");
break;
case 2:
EXPECT_STREQ((*it).first.c_str(), "wird");
EXPECT_STREQ((*it).second.c_str(), "will");
break;
default:
ASSERT_TRUE(true);
break;
}
count++;
}
EXPECT_TRUE(count==3);
}

TEST(IteratorTest, ForLoopMultiple) {
Trie<string> trie;
Trie<string>::value_type t1("adler", "eagle");
Trie<string>::value_type t2("ader", "vein");
Trie<string>::value_type t3("sonne", "sun");
Trie<string>::value_type t4("arm", "arm");
Trie<string>::value_type t5("zeit", "time");
Trie<string>::value_type t6("nase", "nose");
Trie<string>::value_type t7("nasenbaer", "coati");
trie.insert(t1);
trie.insert(t2);
trie.insert(t3);
trie.insert(t4);
trie.insert(t5);
trie.insert(t6);
trie.insert(t7);
size_t count = 0;
for ( Trie<string>::iterator it = trie.begin(); it != trie.end(); ++it ){
switch( count ) {
case 0:
EXPECT_STREQ((*it).first.c_str(), "ader");
EXPECT_STREQ((*it).second.c_str(), "vein");
break;
case 1:
EXPECT_STREQ((*it).first.c_str(), "adler");
EXPECT_STREQ((*it).second.c_str(), "eagle");
break;
case 2:
EXPECT_STREQ((*it).first.c_str(), "arm");
EXPECT_STREQ((*it).second.c_str(), "arm");
break;
case 3:
EXPECT_STREQ((*it).first.c_str(), "nase");
EXPECT_STREQ((*it).second.c_str(), "nose");
break;
case 4:
EXPECT_STREQ((*it).first.c_str(), "nasenbaer");
EXPECT_STREQ((*it).second.c_str(), "coati");
break;
case 5:
EXPECT_STREQ((*it).first.c_str(), "sonne");
EXPECT_STREQ((*it).second.c_str(), "sun");
break;
case 6:
EXPECT_STREQ((*it).first.c_str(), "zeit");
EXPECT_STREQ((*it).second.c_str(), "time");
break;
default:
ASSERT_TRUE(true);
break;
}
count++;
}
EXPECT_TRUE(count==7);
}


TEST(FindTest, Found) {
Trie<string> trie;
Trie<string>::value_type t1("wer", "who");
Trie<string>::value_type t2("wir", "we");
Trie<string>::value_type t3("wird", "will");
trie.insert(t1);
trie.insert(t2);
trie.insert(t3);

Trie<string>::iterator it = trie.find("wird");
ASSERT_NE(it, trie.end());
EXPECT_STREQ((*it).second.c_str(), "will");
}

TEST(FindTest, FoundFirst) {
Trie<string> trie;
Trie<string>::value_type t1("wer", "who");
Trie<string>::value_type t2("wir", "we");
Trie<string>::value_type t3("wird", "will");
trie.insert(t1);
trie.insert(t2);
trie.insert(t3);

Trie<string>::iterator it = trie.find("wer");
ASSERT_NE(it, trie.end());
EXPECT_STREQ((*it).second.c_str(), "who");
}

TEST(FindTest, FoundMiddle) {
Trie<string> trie;
Trie<string>::value_type t1("wer", "who");
Trie<string>::value_type t2("wir", "we");
Trie<string>::value_type t3("wird", "will");
trie.insert(t1);
trie.insert(t2);
trie.insert(t3);

Trie<string>::iterator it = trie.find("wir");
ASSERT_NE(it, trie.end());
EXPECT_STREQ((*it).second.c_str(), "we");
}

TEST(FindTest, NotFound) {
Trie<string> trie;
Trie<string>::value_type t1("wer", "who");
Trie<string>::value_type t2("wir", "we");
Trie<string>::value_type t3("wird", "will");
trie.insert(t1);
trie.insert(t2);
trie.insert(t3);

Trie<string>::iterator it = trie.find("unknown_key");
EXPECT_EQ(it, trie.end());
}


