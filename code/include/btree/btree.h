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

			size_type order() const
			{ return m_key_values.capacity(); }

			std::pair<const_iterator, bool> find_insert_pos(const value_type& value)
			{
				for (auto it = begin(); it != end(); it++)
					if (value <= *it)
						return std::make_pair(it, value == *it);

				return std::make_pair(end(), false);
			}

			std::pair<iterator, bool> insert(const_iterator it, const value_type& value)
			{
				return std::make_pair(m_key_values.insert(it, value), true);
			}

			void erase(iterator first)
			{
				m_key_values.erase(first, end());
			}

			void move(iterator first, Node* node)
			{				
				std::move(first, end(), node->begin());
				m_key_values.erase(first);
			}

			virtual std::unique_ptr<Node> split(const_iterator it, value_type& value) = 0;

			const_iterator cbegin() const
			{ return m_key_values.begin(); }

			const_iterator cend() const
			{ return m_key_values.end(); }

			iterator begin()
			{ return m_key_values.begin(); }

			iterator end()
			{ return m_key_values.end(); }

		protected:
			std::back_insert_iterator< std::vector<Btree::value_type> > back_inserter()
			{ return std::back_insert_iterator< std::vector<Btree::value_type> >(m_key_values); }

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

			BranchNode(size_type order, std::unique_ptr<Node> l, value_type& value, std::unique_ptr<Node> r)
				: Node(order)
			{
				// TODO: Optimize memory allocation
				m_child_nodes.reserve(order + 1);
				insert(begin(), value);
				m_child_nodes.emplace_back(std::move(l));
				m_child_nodes.emplace_back(std::move(r));
			}

		protected:
			virtual std::unique_ptr<Node> split(const_iterator it, value_type& value)
			{ throw new std::exception("Illagal virtual method call: BranchNode::split"); }

		private:
			std::vector<std::unique_ptr<Node>> m_child_nodes;
		};

		class LeafNode : public Node
		{
			friend Btree;

		public:
			LeafNode(size_type order) : Node(order) {}
			LeafNode(size_type order, iterator first, iterator last) : Node(order)
			{ std::move(first, last, back_inserter()); }

			virtual std::unique_ptr<Node> split(const_iterator it, value_type& value) 
			{
				// Find split point and calculate offest
				const auto split_idx = (end() - begin()) / 2;
				const auto split = begin() + split_idx;
				const auto insert_idx = it - begin();

				// Get split value
				value_type& split_value = *split;
				if (insert_idx == split_idx)
					std::swap(split_value, value);

				// Create two leaf nodea to receive each half
				auto l = std::make_unique<LeafNode>(order(), begin(), split);
				auto r = std::make_unique<LeafNode>(order(), split + 1, end());

				// Insert new value
				if (insert_idx < split_idx)
					l->insert(l->begin() + split_idx, value);
				else
					r->insert(r->begin() + (split_idx - l->size()), value);

				// Create and return new branch node
				return std::make_unique<BranchNode>(order(), std::move(l), value, std::move(r));
			}

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

			bool operator<(const iterator& it)
			{ return m_curr_idx < it.m_curr_idx; }
			bool operator>(const iterator& it)
			{ return m_curr_idx > it.m_curr_idx; }
			bool operator<=(const iterator& it)
			{ return m_curr_idx <= it.m_curr_idx; }
			bool operator >=(const iterator& it)
			{ return m_curr_idx >= it.m_curr_idx; }
			bool operator==(const iterator& it)
			{ return m_curr_idx == it.m_curr_idx; }
			bool operator!=(const iterator& it)
			{ return m_curr_idx != it.m_curr_idx; }

			iterator& operator++()
			{
				m_curr++; // TODO: More complex increment
				m_curr_idx++;
				return *this;
			}
			
			const value_type& operator*() const
			{
				return *m_curr;
			}

		private:
			iterator(Btree::Node* root, size_type curr_idx, size_type end_idx) 
				: m_curr{ root->begin() + curr_idx }, m_curr_idx{ curr_idx }, m_end_idx{ end_idx } 
			{
			}

			LeafNode::const_iterator m_curr;
			size_type m_curr_idx;
			size_type m_end_idx;
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
		{ return iterator(m_root.get(), 0, m_size); }

		//const_iterator begin() const noexcept
		//{
		//	return const_iterator(m_root.get(), 0, m_size);
		//}

		iterator end() noexcept
		{
			return iterator(m_root.get(), m_size, m_size);
		}

		//const_iterator end() const noexcept
		//{ return const_iterator(m_root.get(), 0, m_size); }

		// Capacity
		bool empty() const
		{ return m_size == 0; }

		size_type size() const
		{ return m_size; }

		// Modifiers
		void clear() noexcept;
		std::pair<iterator, bool> insert(value_type& value);
		iterator erase(const_iterator pos);

		// Lookup
		iterator find(const Key& key);
		const_iterator find(const Key& key) const;

	private:
		std::unique_ptr<Node> m_root;
		size_type m_order;
		size_type m_size{ 0 };
	};

};

#endif

