// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"

using namespace std;

TEST(test_empty_true)
{
    BinarySearchTree<int> b;
    ASSERT_TRUE(b.empty());
}

TEST(test_empty_false)
{
    BinarySearchTree<int> b;
    b.insert(2);
    ASSERT_FALSE(b.empty());
}

TEST(test_size)
{
    BinarySearchTree<int> b;
    b.insert(1);
    b.insert(0);
    b.insert(5);
    b.insert(3);
    ASSERT_EQUAL(b.size(), 4);
}

TEST(test_size_zero)
{
    BinarySearchTree<int> b;
    ASSERT_EQUAL(b.size(), 0);
}

TEST(test_height)
{
    BinarySearchTree<int> b;
    b.insert(1);
    b.insert(0);
    b.insert(5);
    b.insert(3);
    b.insert(2);
    b.insert(4);
    ASSERT_EQUAL(b.height(), 4);
}

TEST(test_height_zero)
{
    BinarySearchTree<int> b;
    ASSERT_EQUAL(b.height(), 0);
}

TEST(test_check_sorting_invariant_false)
{
    BinarySearchTree<int> b;
    b.insert(1);
    b.insert(0);
    *b.begin() = 2;
    ASSERT_FALSE(b.check_sorting_invariant());
}

TEST(test_check_sorting_invariant_true)
{
    BinarySearchTree<int> b;
    b.insert(6);
    b.insert(4);
    b.insert(9);
    b.insert(3);
    b.insert(5);
    b.insert(8);
    b.insert(10);
    b.insert(12);
    ASSERT_TRUE(b.check_sorting_invariant());
}

TEST(test_check_sorting_invariant_true_empty)
{
    BinarySearchTree<int> b;
    ASSERT_TRUE(b.check_sorting_invariant());
}

TEST(test_copy_node)
{
    BinarySearchTree<int> b1;
    b1.insert(1);
    b1.insert(0);
    b1.insert(2);
    b1.insert(3);
    BinarySearchTree<int> b2 = BinarySearchTree<int>(b1);
    ASSERT_TRUE(*b1.begin() == *b2.begin());
    ASSERT_TRUE(b1.height() == b2.height());
    ASSERT_TRUE(b1.size() == b2.size());
}

TEST(test_destroy_node)
{
    BinarySearchTree<int> b1;
    b1.insert(1);
    b1.insert(0);
    b1.insert(2);
    b1.insert(3);
    BinarySearchTree<int> b2;
    b1 = b2;
    ASSERT_TRUE(b1.begin() == b2.begin());
    ASSERT_TRUE(b1.end() == b2.end());
    ASSERT_TRUE(b1.height() == b2.height());
    ASSERT_TRUE(b1.size() == b2.size());
}

TEST(test_find_true)
{
    BinarySearchTree<int> b;
    b.insert(1);
    b.insert(0);
    b.insert(2);
    b.insert(3);
    b.insert(5);
    b.insert(9);
    b.insert(7);
    b.insert(12);
    BinarySearchTree<int>::Iterator iter = b.find(0);

    ASSERT_EQUAL(*iter, 0);
}

TEST(test_find_false)
{
    BinarySearchTree<int> b;
    b.insert(6);
    b.insert(4);
    b.insert(9);
    b.insert(3);
    b.insert(5);
    b.insert(8);
    b.insert(10);
    b.insert(12);
    BinarySearchTree<int>::Iterator iter = b.find(18);
    ASSERT_TRUE(iter == b.end());
}

TEST(test_find_false_empty)
{
    BinarySearchTree<int> b;
    BinarySearchTree<int>::Iterator iter = b.find(18);
    ASSERT_TRUE(iter == b.end());
}

TEST(test_min_element)
{
    BinarySearchTree<int> b;
    b.insert(6);
    b.insert(4);
    b.insert(9);
    b.insert(3);
    b.insert(5);
    b.insert(8);
    b.insert(10);
    b.insert(12);
    BinarySearchTree<int>::Iterator iter = b.min_element();
    ASSERT_EQUAL(*iter, 3);
}

TEST(test_min_element_negative)
{
    BinarySearchTree<int> b;
    b.insert(6);
    b.insert(-4);
    b.insert(9);
    b.insert(3);
    b.insert(5);
    b.insert(-8);
    b.insert(-10);
    b.insert(12);
    BinarySearchTree<int>::Iterator iter = b.min_element();
    ASSERT_EQUAL(*iter, -10);
}

TEST(test_min_element_empty)
{
    BinarySearchTree<int> b;
    BinarySearchTree<int>::Iterator iter = b.min_element();
    ASSERT_TRUE(iter == b.end());
}

TEST(test_max_element)
{
    BinarySearchTree<int> b;
    b.insert(6);
    b.insert(4);
    b.insert(9);
    b.insert(3);
    b.insert(5);
    b.insert(8);
    b.insert(10);
    b.insert(12);
    BinarySearchTree<int>::Iterator iter = b.max_element();
    ASSERT_EQUAL(*iter, 12);
}

TEST(test_max_element_empty)
{
    BinarySearchTree<int> b;
    BinarySearchTree<int>::Iterator iter = b.max_element();
    ASSERT_TRUE(iter == b.end());
}

TEST(test_min_element_greater_than)
{
    BinarySearchTree<int> b;
    b.insert(6);
    b.insert(-4);
    b.insert(-9);
    b.insert(3);
    b.insert(5);
    b.insert(8);
    b.insert(10);
    b.insert(12);
    BinarySearchTree<int>::Iterator iter = b.min_greater_than(8);
    ASSERT_EQUAL(*iter, 10);
}

TEST(test_print)
{
    BinarySearchTree<int> tree;

    tree.insert(5);

    ASSERT_TRUE(tree.size() == 1);
    ASSERT_TRUE(tree.height() == 1);

    ASSERT_TRUE(tree.find(5) != tree.end());

    tree.insert(7);
    tree.insert(3);

    ASSERT_TRUE(tree.check_sorting_invariant());
    ASSERT_TRUE(*tree.max_element() == 7);
    ASSERT_TRUE(*tree.min_element() == 3);
    ASSERT_TRUE(*tree.min_greater_than(5) == 7);

    cout << tree.to_string() << endl
         << endl;

    cout << tree << endl
         << endl;

    ostringstream oss_preorder;
    tree.traverse_preorder(oss_preorder);
    cout << oss_preorder.str() << endl
         << endl;
    ASSERT_TRUE(oss_preorder.str() == "5 3 7 ");

    ostringstream oss_inorder;
    tree.traverse_inorder(oss_inorder);
    cout << oss_inorder.str() << endl
         << endl;
    ASSERT_TRUE(oss_inorder.str() == "3 5 7 ");
}


TEST_MAIN()
