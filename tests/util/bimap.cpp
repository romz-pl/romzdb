#include "gtest/gtest.h"
#include "util/bimap.h"

class correctness : public testing::Test
{
protected:
    bimap<int, std::string> map;
    
    virtual void SetUp() {
        EXPECT_TRUE(map.insert(1, "aba"));
        EXPECT_TRUE(map.insert(2, "caba"));
        EXPECT_TRUE(map.insert(3, "abacaba")); 
    }
};

TEST_F(correctness, empty_map) {
    bimap<int, std::string> map;
    EXPECT_EQ(0, map.size());
    EXPECT_TRUE(map.empty());
}

TEST_F(correctness, one_element) {
    bimap<int, int> map;
    map.insert(1, 2);
    EXPECT_EQ(2, map.get_value_by_key(1));
    EXPECT_EQ(1, map.get_key_by_value(2));
    EXPECT_EQ(1, map.size());
}

TEST_F(correctness, multi_items) {
    EXPECT_EQ(1, map.get_key_by_value("aba"));
    EXPECT_EQ(2, map.get_key_by_value("caba"));
    EXPECT_EQ(3, map.get_key_by_value("abacaba"));
    EXPECT_EQ("aba", map.get_value_by_key(1));
    EXPECT_EQ("caba", map.get_value_by_key(2));
    EXPECT_EQ("abacaba", map.get_value_by_key(3));

    EXPECT_FALSE(map.empty());
    EXPECT_EQ(3, map.size());
}

TEST_F(correctness, insert_element_with_existing_value_or_key) {
    EXPECT_FALSE(map.insert(4, "aba"));
    EXPECT_FALSE(map.insert(3, "QWE"));
    EXPECT_FALSE(map.insert(2, "caba"));

    EXPECT_EQ("aba", map.get_value_by_key(1));
    EXPECT_EQ("caba", map.get_value_by_key(2));
    EXPECT_EQ("abacaba", map.get_value_by_key(3));
    EXPECT_EQ(1, map.get_key_by_value("aba"));
    EXPECT_EQ(2, map.get_key_by_value("caba"));
    EXPECT_EQ(3, map.get_key_by_value("abacaba"));

    EXPECT_EQ(3, map.size());
}

TEST_F(correctness, geting_absent_value) {
    bimap<int, std::string> map;
    EXPECT_ANY_THROW(map.get_value_by_key(1));
    EXPECT_ANY_THROW(map.get_key_by_value("asdasd"));
}

TEST_F(correctness, delete_existing_element) {
    EXPECT_TRUE(map.erase_by_key(2));
    EXPECT_ANY_THROW(map.get_value_by_key(2));
    EXPECT_ANY_THROW(map.get_key_by_value("caba"));
    
    EXPECT_TRUE(map.erase_by_value("aba"));
    EXPECT_ANY_THROW(map.get_value_by_key(1));
    EXPECT_ANY_THROW(map.get_key_by_value("aba"));
    
    EXPECT_EQ(3, map.get_key_by_value("abacaba"));
    EXPECT_EQ(1, map.size());
    EXPECT_FALSE(map.empty());
}

TEST_F(correctness, delete_absent_element) {
    bimap<int, std::string> map;
    map.insert(1, "aba");
    EXPECT_FALSE(map.erase_by_key(213123));
    EXPECT_FALSE(map.erase_by_key(131451));
    EXPECT_FALSE(map.erase_by_value("asdq"));
    EXPECT_FALSE(map.erase_by_value("asdh"));
    
    EXPECT_EQ(1, map.size());
    EXPECT_FALSE(map.empty());
}

TEST_F(correctness, random_insert_erase) {
    /*
    bimap<int, int> map;
    std::set<int> keys_set;
    std::set<int> values_set;

    for (size_t i = 0; i < 100000; ++i) {
        int key = std::rand(); 
        int value = std::rand();
        auto in = map.insert(key, value);
        EXPECT_TRUE(in == (keys_set.find(key) == keys_set.end()));
        EXPECT_TRUE(in == (values_set.find(value) == values_set.end()));
        keys_set.insert(key);
        values_set.insert(value);
    }
    for (size_t i = 0; i < 100000; ++i) {
        int key = std::rand(); 
        auto del = map.erase_by_key(key);
        EXPECT_TRUE(del == (keys_set.find(key) != keys_set.end()));
        keys_set.erase(key);
    }

    EXPECT_EQ(keys_set.size(), map.size());
    */
}

TEST(copy, bimap_assiginment_copys_independence) {
    bimap<int, std::string> a; 
    EXPECT_TRUE(a.insert(1, "abacaba"));
    EXPECT_TRUE(a.insert(2, "abaa"));
    EXPECT_TRUE(a.insert(3, "abac")); 
    bimap<int, std::string> b = a;

    EXPECT_TRUE(a.erase_by_value("abacaba"));
    EXPECT_TRUE(a.erase_by_key(3));

    EXPECT_TRUE(b.erase_by_key(1));
    EXPECT_TRUE(b.erase_by_value("abaa"));

    EXPECT_ANY_THROW(b.get_value_by_key(1));
    EXPECT_ANY_THROW(b.get_value_by_key(2)); 
    EXPECT_ANY_THROW(b.get_key_by_value("abacaba"));
    EXPECT_ANY_THROW(b.get_key_by_value("abaa"));
    
    EXPECT_EQ("abac", b.get_value_by_key(3));
    EXPECT_EQ(3, b.get_key_by_value("abac"));
    EXPECT_EQ(1, b.size());

    EXPECT_EQ("abaa", a.get_value_by_key(2));
    EXPECT_EQ(1, a.size());
}

TEST(copy, bimap_copys_independence) {
    bimap<int, std::string> a; 
    EXPECT_TRUE(a.insert(1, "abacaba"));
    EXPECT_TRUE(a.insert(2, "abaa"));
    EXPECT_TRUE(a.insert(3, "abac")); 
    bimap<int, std::string> b = a;

    EXPECT_TRUE(a.erase_by_value("abacaba"));
    EXPECT_TRUE(a.erase_by_key(3));

    EXPECT_TRUE(b.erase_by_key(1));
    EXPECT_TRUE(b.erase_by_value("abaa"));

    EXPECT_ANY_THROW(b.get_value_by_key(1));
    EXPECT_ANY_THROW(b.get_value_by_key(2)); 
    EXPECT_ANY_THROW(b.get_key_by_value("abacaba"));
    EXPECT_ANY_THROW(b.get_key_by_value("abaa"));
    
    EXPECT_EQ("abac", b.get_value_by_key(3));
    EXPECT_EQ(3, b.get_key_by_value("abac"));
    EXPECT_EQ(1, b.size());

    EXPECT_EQ("abaa", a.get_value_by_key(2));
    EXPECT_EQ(1, a.size());
}

TEST(iterator_correctness, iterate_over_collection) {
    /*
    bimap<int, int> map;
    std::set<int> keys_set;
    std::set<int> values_set;
    std::set<std::pair<int, int>> key_value_pairs;
    for (size_t i = 0; i < 100000; ++i) {
        int key = std::rand(); 
        int value = std::rand();
        auto in = map.insert(key, value);
        EXPECT_TRUE(in == (keys_set.find(key) == keys_set.end()));
        EXPECT_TRUE(in == (values_set.find(value) == values_set.end()));
        keys_set.insert(key);
        values_set.insert(value);
        key_value_pairs.emplace(key, value);
    }

    for (auto pair : map) {
        EXPECT_TRUE(key_value_pairs.find(pair) != key_value_pairs.end());
        key_value_pairs.erase(pair);
    }

    EXPECT_EQ(0, key_value_pairs.size());
    */
}

