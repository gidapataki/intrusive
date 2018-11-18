#include "intrusive/List.h"
#include <iostream>


template<typename U, typename V>
void expect_eq(int line, const U& expected, const V& actual) {
	if (!(expected == actual)) {
		std::cerr << "Error in line " << line << std::endl;
		std::cerr << "  expected: " << expected << std::endl;
		std::cerr << "    actual: " << actual << std::endl;
	}
}

template<typename U, typename V>
void expect_ne(int line, const U& expected, const V& actual) {
	if (!(expected != actual)) {
		std::cerr << "Error in line " << line << std::endl;
		std::cerr << "  expected: " << expected << std::endl;
		std::cerr << "    actual: " << actual << std::endl;
	}
}

#define EXPECT_EQ(u, v) expect_eq(__LINE__, u, v)
#define EXPECT_NE(u, v) expect_ne(__LINE__, u, v)
#define EXPECT_TRUE(v) expect_eq(__LINE__, true, v)
#define EXPECT_FALSE(v) expect_eq(__LINE__, false, v)


struct Element
	: public intrusive::Node<Element, struct tag0>
	, public intrusive::Node<Element, struct tag1>
{
	int value = 0;
};


using List0 = intrusive::List<Element, struct tag0>;
using List1 = intrusive::List<Element, struct tag1>;


std::ostream& operator<<(std::ostream& stream, const Element& e) {
	return stream << e.value;
}

template<typename Type, typename Tag>
std::ostream& operator<<(std::ostream& stream, const intrusive::List<Type, Tag>& ls) {
	stream << "[";
	auto it = ls.begin(), it_end = ls.end();
	if (it != it_end) {
		stream << *it;
		++it;
	}
	for (; it != it_end; ++it) {
		stream << " " << *it;
	}
	stream << "]";
	return stream;
}

template<typename Type, typename Tag>
std::ostream& operator<<(std::ostream& stream, const intrusive::Iterator<Type, Tag>& it) {
	return stream << (&*it);
}

void TestSizes() {
	List0 list;
	Element e1;
	Element e2;
	Element e3;

	EXPECT_TRUE(list.IsEmpty());
	EXPECT_EQ(0, list.Size());

	list.LinkBack(e1);
	EXPECT_FALSE(list.IsEmpty());
	EXPECT_EQ(1, list.Size());

	list.LinkBack(e2);
	EXPECT_FALSE(list.IsEmpty());
	EXPECT_EQ(2, list.Size());

	list.LinkBack(e3);
	EXPECT_FALSE(list.IsEmpty());
	EXPECT_EQ(3, list.Size());

	list.UnlinkBack();
	EXPECT_FALSE(list.IsEmpty());
	EXPECT_EQ(2, list.Size());

	list.Clear();
	EXPECT_TRUE(list.IsEmpty());
	EXPECT_EQ(0, list.Size());
}


void TestLink() {
	List0 list;
	Element e1;
	Element e2;
	Element e3;

	list.LinkBack(e1);
	list.LinkBack(e2);
	list.LinkFront(e3);

	EXPECT_EQ(&e3, &list.Front());
	EXPECT_EQ(&e2, &list.Back());
}


void TestLinkUnlink() {
	List0 list;
	Element e1;
	Element e2;
	Element e3;

	list.LinkBack(e1);
	list.LinkBack(e2);
	list.LinkBack(e3);
	EXPECT_EQ(&e1, &list.Front());
	EXPECT_EQ(&e3, &list.Back());

	list.UnlinkBack();
	EXPECT_EQ(&e1, &list.Front());
	EXPECT_EQ(&e2, &list.Back());

	list.UnlinkFront();
	EXPECT_EQ(&e2, &list.Front());
	EXPECT_EQ(&e2, &list.Back());
}


void TestRemove() {
	List0 list;
	Element e1;
	Element e2;
	Element e3;
	Element e4;

	list.LinkBack(e1);
	list.LinkBack(e2);
	list.LinkBack(e3);

	list.Remove(list.Find(e2));
	EXPECT_EQ(&e1, &list.Front());
	EXPECT_EQ(&e3, &list.Back());

	list.Remove(list.Find(e1));
	EXPECT_EQ(&e3, &list.Front());
	EXPECT_EQ(&e3, &list.Back());

	list.Remove(List0::iterator(&e1));
	EXPECT_EQ(&e3, &list.Front());
	EXPECT_EQ(&e3, &list.Back());

	list.Remove(list.Find(e3));
	EXPECT_TRUE(list.IsEmpty());

	list.Remove(list.Find(e4));
	EXPECT_TRUE(list.IsEmpty());
}

void TestCount() {
	List0 list1;
	List0 list2;

	Element e1;
	Element e2;
	Element e3;
	Element e4;

	list1.LinkBack(e1);
	list1.LinkBack(e2);
	list2.LinkBack(e3);

	EXPECT_EQ(1, list1.Count(e1));
	EXPECT_EQ(1, list1.Count(e2));
	EXPECT_EQ(0, list1.Count(e3));

	EXPECT_EQ(0, list2.Count(e1));
	EXPECT_EQ(0, list2.Count(e2));
	EXPECT_EQ(1, list2.Count(e3));
}


void TestIterators() {
	List0 list;
	List0::iterator li;
	List0::iterator lj;

	Element e1;
	Element e2;

	list.LinkBack(e1);
	list.LinkBack(e2);

	lj = li = list.begin();
	EXPECT_EQ(&*li, &e1);
	EXPECT_EQ(&*lj, &e1);
	EXPECT_EQ(li, lj);

	lj = ++li;
	EXPECT_EQ(&*li, &e2);
	EXPECT_EQ(&*lj, &e2);

	lj = li++;
	EXPECT_EQ(li, list.end());
	EXPECT_EQ(&*lj, &e2);

	lj = li--;
	EXPECT_EQ(&*li, &e2);
	EXPECT_EQ(lj, list.end());

	lj = --li;
	EXPECT_EQ(&*li, &e1);
	EXPECT_EQ(&*lj, &e1);
}

void TestConstIterators() {
	List0 list;
	List0::const_iterator li;
	List0::const_iterator lj;

	Element e1;
	Element e2;

	list.LinkBack(e1);
	list.LinkBack(e2);

	lj = li = list.cbegin();
	EXPECT_EQ(&*li, &e1);
	EXPECT_EQ(&*lj, &e1);
	EXPECT_EQ(li, lj);

	lj = ++li;
	EXPECT_EQ(&*li, &e2);
	EXPECT_EQ(&*lj, &e2);

	lj = li++;
	EXPECT_EQ(li, list.cend());
	EXPECT_EQ(&*lj, &e2);

	lj = li--;
	EXPECT_EQ(&*li, &e2);
	EXPECT_EQ(lj, list.cend());

	lj = --li;
	EXPECT_EQ(&*li, &e1);
	EXPECT_EQ(&*lj, &e1);
}


int main() {
	TestSizes();
	TestLink();
	TestLinkUnlink();
	TestRemove();
	TestCount();
	TestIterators();
	TestConstIterators();
}
