#include "intrusive/List.h"
#include <iostream>
#include <vector>
#include <initializer_list>


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
	: public intrusive::Node<Element>
	, public intrusive::Node<Element, struct tag1>
{
	int value = 0;
};


using XList = intrusive::List<Element>;
using YList = intrusive::List<Element, struct tag1>;


std::ostream& operator<<(std::ostream& stream, const Element& e) {
	return stream << e.value;
}

template<typename Container>
std::ostream& OutputToStream(std::ostream& stream, const Container& container) {
	stream << "[";
	auto it = container.begin(), it_end = container.end();
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

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vec) {
	return OutputToStream(stream, vec);
}

template<typename Type, typename Tag>
std::ostream& operator<<(std::ostream& stream, const intrusive::List<Type, Tag>& ls) {
	return OutputToStream(stream, ls);
}

template<typename Type, typename Tag>
std::ostream& operator<<(std::ostream& stream, const intrusive::Iterator<Type, Tag>& it) {
	return stream << (&*it);
}

template<typename Type, typename Tag>
std::vector<const Type*> MakeVector(const intrusive::List<Type, Tag>& ls) {
	std::vector<const Type*> vec;
	for (auto& item : ls) {
		vec.push_back(&item);
	}
	return vec;
}

template<typename Type>
std::vector<const Type*> MakeVector(std::initializer_list<const Type*> ls) {
	std::vector<const Type*> vec;
	for (auto* item : ls) {
		vec.push_back(item);
	}
	return vec;
}


void TestSizes() {
	XList list;
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
	XList list;
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
	XList list;
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
	XList list;
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

	list.Remove(XList::iterator(&e1));
	EXPECT_EQ(&e3, &list.Front());
	EXPECT_EQ(&e3, &list.Back());

	list.Remove(list.Find(e3));
	EXPECT_TRUE(list.IsEmpty());

	list.Remove(list.Find(e4));
	EXPECT_TRUE(list.IsEmpty());
}

void TestCount() {
	XList list1;
	XList list2;

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
	XList list;
	XList::iterator li;
	XList::iterator lj;

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
	XList list;
	XList::const_iterator li;
	XList::const_iterator lj;

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

void TestContainment() {
	XList list1;
	XList list2;

	Element e1;
	Element e2;

	list1.LinkBack(e1);
	list1.LinkBack(e2);
	EXPECT_EQ(&e1, &list1.Front());
	EXPECT_EQ(&e2, &list1.Back());

	list2.LinkBack(e1);
	EXPECT_EQ(&e2, &list1.Front());
	EXPECT_EQ(&e2, &list1.Back());
	EXPECT_EQ(&e1, &list2.Front());
	EXPECT_EQ(&e1, &list2.Back());

	list2.LinkBack(e2);
	EXPECT_TRUE(list1.IsEmpty());
	EXPECT_TRUE(list1.IsEmpty());
	EXPECT_EQ(&e1, &list2.Front());
	EXPECT_EQ(&e2, &list2.Back());
}

void TestMultipleContainment() {
	XList list1;
	YList list2;

	Element e1;
	Element e2;

	list1.LinkBack(e1);
	list1.LinkBack(e2);
	EXPECT_EQ(&e1, &list1.Front());
	EXPECT_EQ(&e2, &list1.Back());

	list2.LinkBack(e1);
	EXPECT_EQ(&e1, &list1.Front());
	EXPECT_EQ(&e2, &list1.Back());
	EXPECT_EQ(&e1, &list2.Front());
	EXPECT_EQ(&e1, &list2.Back());

	list2.LinkBack(e2);
	EXPECT_EQ(&e1, &list1.Front());
	EXPECT_EQ(&e2, &list1.Back());
	EXPECT_EQ(&e1, &list2.Front());
	EXPECT_EQ(&e2, &list2.Back());
}

void TestRangeIterator() {
	using Vector = std::vector<Element*>;
	using ConstVector = std::vector<const Element*>;

	XList list;

	Element e1;
	Element e2;
	Element e3;
	Element e4;

	list.LinkBack(e1);
	list.LinkFront(e2);
	list.LinkBack(e3);
	list.LinkFront(e4);

	Vector vec0;
	ConstVector vec1;

	for (auto& e : list) {
		vec0.push_back(&e);
		vec1.push_back(&e);
	}

	EXPECT_EQ(Vector({&e4, &e2, &e1, &e3}), vec0);
	EXPECT_EQ(ConstVector({&e4, &e2, &e1, &e3}), vec1);
}

void TestInsertion() {
	XList list;
	XList::iterator it;

	Element e1;
	Element e2;
	Element e3;
	Element e4;

	list.LinkBack(e1);
	list.LinkBack(e2);
	list.LinkBack(e3);
	EXPECT_EQ(MakeVector({ &e1, &e2, &e3 }), MakeVector(list));

	it = list.begin();
	list.Insert(it, e4);
	EXPECT_EQ(MakeVector({ &e4, &e1, &e2, &e3 }), MakeVector(list));

	it = list.begin();
	EXPECT_EQ(&e4, &*it);

	list.Insert(it, e4);
	EXPECT_EQ(MakeVector({ &e4, &e1, &e2, &e3 }), MakeVector(list));

	++it;
	EXPECT_EQ(&e1, &*it);

	list.Insert(it, e4);
	EXPECT_EQ(MakeVector({ &e4, &e1, &e2, &e3 }), MakeVector(list));

	++it;
	EXPECT_EQ(&e2, &*it);

	list.Insert(it, e4);
	EXPECT_EQ(MakeVector({ &e1, &e4, &e2, &e3 }), MakeVector(list));

	++it;
	EXPECT_EQ(&e3, &*it);

	list.Insert(it, e4);
	EXPECT_EQ(MakeVector({ &e1, &e2, &e4, &e3 }), MakeVector(list));

	++it;
	EXPECT_EQ(list.end(), it);

	list.Insert(it, e4);
	EXPECT_EQ(MakeVector({ &e1, &e2, &e3, &e4 }), MakeVector(list));
}


void TestFind() {
	XList list;
	XList list2;
	const XList& const_list = list;
	XList::iterator it;
	XList::const_iterator const_it;

	Element e1;
	Element e2;
	Element e3;
	Element e4;

	list.LinkBack(e1);
	list.LinkBack(e2);
	list.LinkBack(e3);
	list2.LinkBack(e4);

	it = list.begin();
	++it;

	const_it = const_list.begin();
	++const_it;

	EXPECT_EQ(it, XList::iterator(&e2));
	EXPECT_EQ(const_it, XList::const_iterator(&e2));
	EXPECT_EQ(const_list.cbegin(), const_list.begin());
	EXPECT_EQ(const_list.cend(), const_list.end());

	EXPECT_EQ(list.end(), list.Find(e4));
	EXPECT_EQ(const_list.end(), const_list.Find(e4));
	EXPECT_EQ(list2.begin(), list2.Find(e4));

	list.Remove(list.Find(e3));
	EXPECT_EQ(list.end(), list.Find(e3));
}

void TestSplice() {
	XList list1, list2;
	Element e1;
	Element e2;
	Element e3;
	Element e4;

	list1.LinkBack(e1);
	list1.LinkBack(e2);
	list1.LinkBack(e3);
	list1.LinkBack(e4);

	// splice to different list
	auto i3 = XList::iterator(&e3);
	list2.Splice(list2.begin(), list1.begin(), i3);
	EXPECT_EQ(MakeVector({&e3, &e4}), MakeVector(list1));
	EXPECT_EQ(MakeVector({&e1, &e2}), MakeVector(list2));

	list1.Splice(list1.begin(), list2.begin(), list2.end());
	EXPECT_EQ(MakeVector({&e1, &e2, &e3, &e4}), MakeVector(list1));
	EXPECT_EQ(MakeVector<Element>({}), MakeVector(list2));

	// splice to same list
	auto i4 = XList::iterator(&e4);
	i3 = XList::iterator(&e3);
	list1.Splice(i4, list1.begin(), i3);
	EXPECT_EQ(MakeVector({&e3, &e1, &e2, &e4}), MakeVector(list1));

	// last iterator equals pos in the same list
	auto it = list1.begin();
	++it;
	list1.Splice(it, list1.begin(), it);
	EXPECT_EQ(MakeVector({&e3, &e1, &e2, &e4}), MakeVector(list1));

	// first / last are the same
	++(it = list1.begin());
	list1.Splice(list1.begin(), it, it);
	EXPECT_EQ(MakeVector({&e3, &e1, &e2, &e4}), MakeVector(list1));
}

void TestMoveConstructAndAssign() {
	XList list1;
	Element e1;
	Element e2;
	Element e3;
	Element e4;
	auto vec1 = MakeVector({&e1, &e2, &e3});

	list1.LinkBack(e1);
	list1.LinkBack(e2);
	list1.LinkBack(e3);
	EXPECT_EQ(MakeVector({&e1, &e2, &e3}), MakeVector(list1));

	XList list2(std::move(list1));
	XList list3;
	EXPECT_TRUE(list1.IsEmpty());

	list1.LinkBack(e4);
	EXPECT_EQ(1, list1.Size());
	EXPECT_EQ(MakeVector({&e1, &e2, &e3}), MakeVector(list2));
	EXPECT_TRUE(list3.IsEmpty());

	list3 = std::move(list2);
	EXPECT_EQ(MakeVector({&e1, &e2, &e3}), MakeVector(list3));

	e4 = std::move(e3);
	EXPECT_EQ(MakeVector({&e1, &e2, &e4}), MakeVector(list3));

	Element e5(std::move(e2));
	EXPECT_EQ(MakeVector({&e1, &e5, &e4}), MakeVector(list3));

	e4 = std::move(e1);
	EXPECT_EQ(MakeVector({&e4, &e5}), MakeVector(list3));
}

void TestDestruct() {
	XList list1;
	Element e1;
	{
		Element e2;

		list1.LinkBack(e1);
		list1.LinkBack(e2);
		EXPECT_EQ(MakeVector({&e1, &e2}), MakeVector(list1));
	}
	EXPECT_EQ(MakeVector({&e1}), MakeVector(list1));
}

int main() {
	TestSizes();
	TestLink();
	TestLinkUnlink();
	TestRemove();
	TestCount();
	TestIterators();
	TestConstIterators();
	TestContainment();
	TestMultipleContainment();
	TestRangeIterator();
	TestInsertion();
	TestFind();
	TestSplice();
	TestMoveConstructAndAssign();
	TestDestruct();
}
