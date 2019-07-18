#ifndef __BTREE_H__
#define __BTREE_H__

#include <cstddef>
#include <utility>
#include <string>
#include <vector>
#include <memory>

namespace ltc {

	class Btree
	{
	public:
		// Member types
		using size_type = std::size_t;
		using Key = std::string;
		using T = std::string;
		using value_type = std::pair<Key, T>;

	private:
		class Node
		{
		public:
			using iterator = std::vector<Btree::value_type>::iterator;
			using const_iterator = std::vector<Btree::value_type>::const_iterator;

			Node(size_type order)
			{
				m_key_values.reserve(order);
			}

			size_t size() const
			{
				return m_key_values.size();
			}

			bool is_full() const
			{
				return size() == m_key_values.capacity();
			}

			std::pair<const_iterator, bool> find_insert_pos(const value_type& value)
			{
				for (auto it = begin(); it != end(); it++)
					if (value == *it)
						return std::make_pair(it, true);
					else if (value > *it)
						return std::make_pair(it, false);

				return std::make_pair(end(), false);
			}

			std::pair<iterator, bool> insert(const_iterator it, const value_type& value)
			{
				return std::make_pair(m_key_values.insert(it, value), true);
			}

			const_iterator begin() const
			{ return m_key_values.begin(); }

			const_iterator end() const
			{ return m_key_values.end(); }

		private:
			std::vector<Btree::value_type> m_key_values;
		};

		class BranchNode : public Node
		{
			friend Btree;

		public:
			BranchNode(size_type order) : Node(order)
			{
				m_child_nodes.reserve(order + 1);
			}

		private:
			std::vector<std::unique_ptr<Node>> m_child_nodes;
		};

		class LeafNode : public Node
		{
			friend Btree;

		public:
			LeafNode(size_type order) : Node(order) {}

		private:
		};

	public:

		class iterator 
		{
			friend Btree;

		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using value_type = Btree::value_type;
			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;


		private:
			iterator(Btree::Node* root) : m_curr(root->begin()) {}

			LeafNode::const_iterator m_curr;
		};

		using const_iterator = const iterator;

	public:
		// Construcors
		Btree(size_type order = 50)
		{ m_order = order; }

		// Element access
		T& at(const Key& key);
		const T& at(const Key& key) const;

		// Iterators
		iterator begin() noexcept
		{
			return iterator(m_root.get());
		}

		const_iterator begin() const noexcept
		{
			return const_iterator(m_root.get());
		}

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
		size_type m_order;
	};

};

#endif

