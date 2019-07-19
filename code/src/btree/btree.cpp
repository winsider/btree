#include "btree.h"

using namespace ltc;

std::pair<Btree::iterator, bool> Btree::insert(value_type& value)
{
	if (!m_root)
		m_root = std::make_unique<LeafNode>(m_order);

	const auto ip = m_root->find_insert_pos(value);
	if (ip.second)
		return std::make_pair(begin(), false); // Key exists

	if (!m_root->is_full())
		m_root->insert(ip.first, value);
	else
		m_root = m_root->split(ip.first, value);

	m_size++;

	return std::make_pair(begin(), true);

	// Find insert postion
		// Is key in tree?
			// Yes: return false
			// No: 
				// Node has room?
					// Yes: insert value and return true
					// No:
						// Split node
						// Find insert position in parent node
							// Node has room?
								// Yes: insert value and return orignal insert position
								// No:
									// Repeat split...
								
			



}
