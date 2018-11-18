#include "intrusive.h"
#include <iostream>


struct Tag0 {};
struct Tag1 {};
struct Tag2 {};


struct A
	: public intrusive::Node<A, Tag0>
	, public intrusive::Node<A, Tag1>
	, public intrusive::Node<A, Tag2>
{
	int x = 0;
};

std::ostream& operator<<(std::ostream& stream, const A& a) {
	return stream << a.x;
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

int main() {
	intrusive::List<A, Tag0> l0;
	intrusive::List<A, Tag1> l1;
	intrusive::List<A, Tag2> l2;
	A a0, a1, a2;

	a0.x = 2;
	a1.x = 3;
	a2.x = 5;

	l0.LinkBack(a0);
	l0.LinkBack(a1);
	l0.LinkBack(a2);
	l0.LinkBack(a0);

	l1.LinkBack(a0);
	l1.LinkBack(a2);

	l2.LinkBack(a0);

	std::cout << l0 << std::endl;
	std::cout << l1 << std::endl;
	std::cout << l2 << std::endl;

	l0.UnlinkFront();
	l0.UnlinkBack();
	std::cout << l0 << std::endl;
}
