#pragma once
#include <iterator>
#include <type_traits>


namespace intrusive {

template<typename Type, typename Tag> class Node;
template<typename Type, typename Tag> class List;
template<typename Type, typename Tag> class Iterator;

struct tag_default;


template<typename Type, typename Tag=tag_default>
class Node {
public:
	Node();
	~Node();
	Node(const Node&) = delete;
	Node(Node&&);
	Node& operator=(const Node&) = delete;
	Node& operator=(Node&&);

	void Unlink();
	bool IsLinked() const;
	Type* Object();
	const Type* Object() const;

private:
	friend class List<Type, Tag>;
	friend class Iterator<Type, Tag>;
	friend class Iterator<const Type, Tag>;

	static void Link(Node* u, Node* v);

	Node* next_;
	Node* prev_;
};


template<typename Type, typename Tag=tag_default>
class Iterator : public std::iterator<std::bidirectional_iterator_tag, Type> {
public:
	friend class List<typename std::remove_const<Type>::type, Tag>;
	using Node = typename std::conditional<
		std::is_const<Type>::value,
		const Node<typename std::remove_const<Type>::type, Tag>,
		Node<Type, Tag>>::type;

	Iterator() = default;
	explicit Iterator(Node* node);
	Iterator& operator++();
	Iterator& operator--();

	Iterator operator++(int);
	Iterator operator--(int);

	bool operator==(const Iterator& other) const;
	bool operator!=(const Iterator& other) const;

	Type* operator->() const;
	Type& operator*() const;

private:
	Node* node_ = nullptr;
};


template<typename Type, typename Tag=tag_default>
class List {
public:
	using Node = Node<Type, Tag>;
	using iterator = Iterator<Type, Tag>;
	using const_iterator = Iterator<const Type, Tag>;

	List() = default;
	List(List&&) = default;
	List(const List&) = delete;
	~List();

	List& operator=(const List&) = delete;
	List& operator=(List&&) = default;


	// O(1)
	void UnlinkFront();
	void UnlinkBack();
	void LinkFront(Node& u);
	void LinkBack(Node& u);
	bool IsEmpty() const;
	void Splice(iterator pos, iterator first, iterator last);
	void Insert(iterator pos, Node& u);

	Type& Front();
	Type& Back();
	const Type& Front() const;
	const Type& Back() const;

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;

	iterator Remove(iterator it);

	// O(n)
	void Clear();
	std::size_t Size() const;
	std::size_t Count(const Node& u) const;
	iterator Find(const Node& u);
	const_iterator Find(const Node& u) const;


private:
	Node head_;
};


} // namespace

#include "intrusive/List-inl.h"
