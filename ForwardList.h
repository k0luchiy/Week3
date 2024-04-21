#include <iostream>


template <typename T>
class ForwardList {
private:
	struct Node {
		Node* next;
		T value;

		Node() = default;
		explicit Node(const T& value, Node* ptr = nullptr) : value(value), next(ptr) {}
		
		//template <typename... Args>
		//Node(Args&&... args) : value(std::forward<Args>(args)...), next(nullptr) {}

	};

	struct Iterator {
		Node* ptr;

		explicit Iterator(Node* other = nullptr) : ptr(other) {}
		Iterator(const Iterator& other) : ptr(other.ptr) {}

		void operator=(const Iterator& other) { this->ptr = other.ptr; }

		T& operator*() { return ptr->value; }
		Node* operator->() { return ptr; }

		Iterator& operator++() { 
			ptr = ptr->next;
			return *this;
		}

		friend bool operator==(const Iterator& lhs, const Iterator& rhs) { return lhs.ptr == rhs.ptr; }
		friend bool operator!=(const Iterator& lhs, const Iterator& rhs) { return !(lhs == rhs); }
	};

private:
	Node* m_head;
	Node* m_tail;

private:
	void add_node_to_front(Node* new_node) {
		new_node->next = m_head;
		m_head = new_node;
		if (m_tail == nullptr) {
			m_tail = new_node;
		}
	}

public:
	ForwardList() : m_head(nullptr), m_tail(nullptr) {};

	ForwardList(size_t count, const T& value) {
		Node* current = new Node(value);
		m_head = current;
		for (size_t i = 0; i < count-1; ++i) {
			current->next = new Node(value);
			current = current->next;
		}
		m_tail = current->next;
		current->next = nullptr;
	}

	explicit ForwardList(size_t count) : ForwardList(count, T{}) {}

	ForwardList(const ForwardList& other) {
		if (other.is_empty()) {
			ForwardList();
		}

		Iterator first = other.begin();
		Node* new_node = new Node(*first);
		add_node_to_front(new_node);
		insert_after(begin(), ++first, other.end());
	}


	~ForwardList() {
		Node* current = m_head->next;
		
		while (current) {
			delete m_head;
			m_head = current;
			current = current->next;
		}
		m_head = nullptr;
		m_tail = nullptr;
	}

	bool is_empty() const {
		return m_head == nullptr;
	}

	Iterator begin() const {
		return Iterator(m_head);
	}

	Iterator end() const {
		return Iterator(m_tail);
	}

	Iterator insert_after(Iterator pos, const T& value) {
		if (pos.ptr == nullptr) {
			return Iterator();
		}

		Node* new_node = new Node(value, pos->next);
		pos->next = new_node;
		if (pos.ptr == m_tail) {
			m_tail = new_node;
		}
		return Iterator(new_node);
	}

	template<typename InputIt>
	Iterator insert_after(Iterator pos, InputIt first, InputIt last) {
		if (pos.ptr == nullptr) {
			return Iterator();
		}

		for (auto it = first; it != last; ++it) {
			insert_after(pos, *it);
			++pos;
		}
		return pos;
	}

	void push_front(const T& value) {
		Node* new_node = new Node(value, nullptr);
		add_node_to_front(new_node);
	}

	/*template <typename... Args>
	void emplace_front(Args&&... args) {
		Node* new_node = new Node(std::forward<Args>(args)...);
		add_node_to_front(new_node);
	}*/

	void pop_front() {
		Node* old_head = m_head;
		m_head = m_head->next;
		delete old_head;
	}

	Iterator find(const T& value) const {
		auto it = begin();
		while (it != end() && *it !=value) {
			++it;
		}
		return it;
	}


	void remove(const T& value) {
		Node* current = m_head;
		while (current != nullptr) {
			if (current->next == nullptr) {
				return;
			}
			if (current->next->value == value) {
				erase_after(Iterator(current));
			}
			current = current->next;
		}
	}

	void erase_after(Iterator pos) {
		if (!pos->next) {
			return;
		}
		if (pos->next->next == nullptr) {
			m_tail = pos.ptr;
		}
		Node* temp = pos->next;
		pos->next = temp->next;
		delete temp;
	}

};


