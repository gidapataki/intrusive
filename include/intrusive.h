#pragma once
#include <iterator>
#include <type_traits>


namespace intrusive {

namespace detail {

template<bool cond, typename T, typename U>
struct If;

template<typename T, typename U>
struct If<true, T, U> {
	using type = T;
};

template<typename T, typename U>
struct If<false, T, U> {
	using type = U;
};

} // namespace detail



template<typename Type, typename Tag> class Node;
template<typename Type, typename Tag> class List;
template<typename Type, typename Tag> class Iterator;


template<typename Type, typename Tag>
class Node {
public:
	Node() {
		next_ = prev_ = this;
	}

	Node(const Node&) = delete;
	Node& operator=(const Node&) = delete;

	void Unlink() {
		prev_->next_ = next_;
		next_->prev_ = prev_;
		next_ = prev_ = this;
	}

	bool IsLinked() const {
		return next_ != this;
	}

	Type* Object() {
		return static_cast<Type*>(this);
	}

	const Type* Object() const {
		return static_cast<const Type*>(this);
	}

private:
	friend class List<Type, Tag>;
	friend class Iterator<Type, Tag>;
	friend class Iterator<const Type, Tag>;

	static void Link(Node* u, Node* v) {
		u->next_ = v;
		v->prev_ = u;
	}

	Node* next_;
	Node* prev_;
};


template<typename Type, typename Tag>
class Iterator : public std::iterator<std::bidirectional_iterator_tag, Type> {
public:
	using Node = typename detail::If<
		std::is_const<Type>::value,
		const Node<typename std::remove_const<Type>::type, Tag>,
		Node<Type, Tag>>::type;

	Iterator() = default;
	Iterator(Node* node) : node_(node) {}

	Iterator& operator++() {
		node_ = node_->next_;
		return *this;
	}

	Iterator& operator--() {
		node_ = node_->next_;
		return *this;
	}

	Iterator operator++(int) {
		Iterator it = *this;
		++it;
		return it;
	}

	Iterator operator--(int) {
		Iterator it = *this;
		--it;
		return it;
	}

	bool operator==(const Iterator& other) const {
		return other.node_ == node_;
	}

	bool operator!=(const Iterator& other) const {
		return other.node_ != node_;
	}

	Type* operator->() const {
		return node_->Object();
	}

	Type& operator*() const {
		return *node_->Object();
	}

private:
	Node* node_ = nullptr;
};


template<typename Type, typename Tag>
class List {
public:
	using Node = Node<Type, Tag>;
	using iterator = Iterator<Type, Tag>;
	using const_iterator = Iterator<const Type, Tag>;

	List() = default;
	~List() {
		Clear();
	}

	void UnlinkFront() {
		head_.next_->Unlink();
	}

	void UnlinkBack() {
		head_.prev_->Unlink();
	}

	void LinkFront(Node& u) {
		u.Unlink();
		Node::Link(&u, head_.next_);
		Node::Link(&head_, &u);
	}

	void LinkBack(Node& u) {
		u.Unlink();
		Node::Link(head_.prev_, &u);
		Node::Link(&u, &head_);
	}

	iterator begin() {
		return iterator(head_.next_);
	}

	iterator end() {
		return iterator(&head_);
	}

	const_iterator begin() const {
		return const_iterator(head_.next_);
	}

	const_iterator end() const {
		return const_iterator(&head_);
	}

	const_iterator cbegin() const {
		return const_iterator(head_.next_);
	}

	const_iterator cend() const {
		return const_iterator(&head_);
	}

	bool IsEmpty() const {
		return !head_.IsLinked();
	}

	void Clear() {
		while (!IsEmpty()) {
			head_.next_->Unlink();
		}
	}

	std::size_t Size() const {
		std::size_t size = 0;
		for (auto it = begin(), it_end = end(); it != it_end; ++it) {
			++size;
		}
		return size;
	}

	iterator Find(const Node& u) {
		std::size_t size = 0;
		for (auto it = begin(), it_end = end(); it != it_end; ++it) {
			if (&*it == &u) {
				return it;
			}
		}
		return end();
	}

	const_iterator Find(const Node& u) const {
		std::size_t size = 0;
		for (auto it = begin(), it_end = end(); it != it_end; ++it) {
			if (&*it == &u) {
				return it;
			}
		}
		return end();
	}

	iterator Erase(iterator it) {
		if (it.node_ != &head_) {
			(it++).node_->Unlink();
			return it;
		}
		return it;
	}

private:
	Node head_;
};


} // namespace
