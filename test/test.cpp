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
	return stream << a.value;
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



int main() {
	TestSizes();
}
