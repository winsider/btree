#ifndef __BTREE_H__
#define __BTREE_H__

#include <cstddef>
#include <utility>
#include <string>
#include <array>
#include <memory>

namespace ltc { namespace btree {

	class Btree
	{
	public:
		// Member types
		using size_type = std::size_t;
		using Key = std::string;
		using T = std::string;
		using value_type = std::pair<Key, T>;
		struct iterator {};
		using const_iterator = const iterator;

		class Node
		{
		private:
		};

		class BranchNode : public Node
		{
		private:
			std::array<std::string, 50> m_keys;
			std::array<std::string, 50> m_values;
			std::array<int, 51> m_child_nodes;
		};

		class LeafNode : public Node
		{
		private:
			std::array<std::string, 50> m_keys;
			std::array<std::string, 50> m_values;
		};


		// Element access
		T& at(const Key& key);
		const T& at(const Key& key) const;

		// Iterators
		iterator begin() noexcept;
		const_iterator begin() const noexcept;
		iterator end() noexcept;
		const_iterator end() const noexcept;

		// Capacity
		bool empty() const;
		size_type size() const;

		// Modifiers
		void clear() noexcept;
		std::pair<iterator, bool> insert(value_type&& value);
		iterator erase(const_iterator pos);

		// Lookup
		iterator find(const Key& key);
		const_iterator find(const Key& key) const;

	private:
		std::unique_ptr<Node> m_root;
	};

} };

#endif
