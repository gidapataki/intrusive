#pragma once


namespace intrusive {

// Node

template<typename Type, typename Tag>
Node<Type, Tag>::Node() {
	next_ = prev_ = this;
}

template<typename Type, typename Tag>
void Node<Type, Tag>::Unlink() {
	prev_->next_ = next_;
	next_->prev_ = prev_;
	next_ = prev_ = this;
}

template<typename Type, typename Tag>
bool Node<Type, Tag>::IsLinked() const {
	return next_ != this;
}

template<typename Type, typename Tag>
Type* Node<Type, Tag>::Object() {
	return static_cast<Type*>(this);
}

template<typename Type, typename Tag>
const Type* Node<Type, Tag>::Object() const {
	return static_cast<const Type*>(this);
}

template<typename Type, typename Tag>
void Node<Type, Tag>::Link(Node* u, Node* v) {
	u->next_ = v;
	v->prev_ = u;
}


// Iterator

template<typename Type, typename Tag>
Iterator<Type, Tag>::Iterator(Node* node)
	: node_(node)
{}

template<typename Type, typename Tag>
Iterator<Type, Tag>& Iterator<Type, Tag>::operator++() {
	node_ = node_->next_;
	return *this;
}

template<typename Type, typename Tag>
Iterator<Type, Tag>& Iterator<Type, Tag>::operator--() {
	node_ = node_->next_;
	return *this;
}

template<typename Type, typename Tag>
Iterator<Type, Tag> Iterator<Type, Tag>::operator++(int) {
	Iterator it = *this;
	++it;
	return it;
}

template<typename Type, typename Tag>
Iterator<Type, Tag> Iterator<Type, Tag>::operator--(int) {
	Iterator it = *this;
	--it;
	return it;
}

template<typename Type, typename Tag>
bool Iterator<Type, Tag>::operator==(const Iterator& other) const {
	return other.node_ == node_;
}

template<typename Type, typename Tag>
bool Iterator<Type, Tag>::operator!=(const Iterator& other) const {
	return other.node_ != node_;
}

template<typename Type, typename Tag>
Type* Iterator<Type, Tag>::operator->() const {
	return node_->Object();
}

template<typename Type, typename Tag>
Type& Iterator<Type, Tag>::operator*() const {
	return *node_->Object();
}


// List

template<typename Type, typename Tag>
List<Type, Tag>::~List() {
	Clear();
}

template<typename Type, typename Tag>
void List<Type, Tag>::UnlinkFront() {
	head_.next_->Unlink();
}

template<typename Type, typename Tag>
void List<Type, Tag>::UnlinkBack() {
	head_.prev_->Unlink();
}

template<typename Type, typename Tag>
void List<Type, Tag>::LinkFront(Node& u) {
	u.Unlink();
	Node::Link(&u, head_.next_);
	Node::Link(&head_, &u);
}

template<typename Type, typename Tag>
void List<Type, Tag>::LinkBack(Node& u) {
	u.Unlink();
	Node::Link(head_.prev_, &u);
	Node::Link(&u, &head_);
}

template<typename Type, typename Tag>
typename List<Type, Tag>::iterator List<Type, Tag>::begin() {
	return iterator(head_.next_);
}

template<typename Type, typename Tag>
typename List<Type, Tag>::iterator List<Type, Tag>::end() {
	return iterator(&head_);
}

template<typename Type, typename Tag>
typename List<Type, Tag>::const_iterator List<Type, Tag>::begin() const {
	return const_iterator(head_.next_);
}

template<typename Type, typename Tag>
typename List<Type, Tag>::const_iterator List<Type, Tag>::end() const {
	return const_iterator(&head_);
}

template<typename Type, typename Tag>
typename List<Type, Tag>::const_iterator List<Type, Tag>::cbegin() const {
	return const_iterator(head_.next_);
}

template<typename Type, typename Tag>
typename List<Type, Tag>::const_iterator List<Type, Tag>::cend() const {
	return const_iterator(&head_);
}

template<typename Type, typename Tag>
bool List<Type, Tag>::IsEmpty() const {
	return !head_.IsLinked();
}

template<typename Type, typename Tag>
void List<Type, Tag>::Clear() {
	while (!IsEmpty()) {
		head_.next_->Unlink();
	}
}

template<typename Type, typename Tag>
std::size_t List<Type, Tag>::Size() const {
	std::size_t size = 0;
	for (auto it = begin(), it_end = end(); it != it_end; ++it) {
		++size;
	}
	return size;
}

template<typename Type, typename Tag>
typename List<Type, Tag>::iterator List<Type, Tag>::Find(const Node& u) {
	for (auto it = begin(), it_end = end(); it != it_end; ++it) {
		if (&*it == &u) {
			return it;
		}
	}
	return end();
}

template<typename Type, typename Tag>
typename List<Type, Tag>::const_iterator List<Type, Tag>::Find(const Node& u) const {
	for (auto it = begin(), it_end = end(); it != it_end; ++it) {
		if (&*it == &u) {
			return it;
		}
	}
	return end();
}

template<typename Type, typename Tag>
typename List<Type, Tag>::iterator List<Type, Tag>::Remove(iterator it) {
	if (it.node_ != &head_) {
		(it++).node_->Unlink();
		return it;
	}
	return it;
}

} // namespace
