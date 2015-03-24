/*
 * list.h
 *
 *  Created on: Feb 1, 2015
 *      Author: Lyle
 */

#ifndef LIST_H_
#define LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

template< class data_t >
class list;

template< class data_t >
class node
{
	node< data_t > * next;
	node< data_t > * prev;
	size_t indx;

	friend list< data_t > ;

public:
	data_t * data;

	/// Basic null-constructor
	node() :
			next( NULL ), prev( NULL ), data( NULL ), indx( 0 )
	{
	}

	/// Defaulted copy-constructor
	node( const node & ) = default;

	/// Preferred constructor
	node( const data_t & _data, size_t _indx = 0 ) :
			next( NULL ), prev( NULL ),  indx( _indx ), data( &_data )
	{
	}

	~node()
	{
		if( next != NULL )
			next->prev = prev;
		if( prev != NULL )
			prev->next = next;
	}

	inline const size_t index()
	{
		return indx;
	}
//	inline const size_t index(size_t _indx)
//	{
//		return indx=_indx;
//	}

/// Cast operator for data access
	inline
	operator data_t()
	{
		return *data;
	}

	inline
	data_t * operator->()
	{
		return data;
	}

	/// Get operator for data access
	inline data_t & get()
	{
		return (*data);
	}

	/**
	 * Remove the node from it's neighbors.
	 * \post Nodes \c prev and \c next will be linked.
	 * @return Pointer to the stored data.
	 */
	inline data_t * unlink()
	{
		if( this->next != NULL )
			this->next->prev = this->prev;
		if( this->prev != NULL )
			this->prev->next = this->next;
		return data;
	}

	/**
	 * Link a node immediately following this.
	 * \post Nodes \c prev and \c next will be linked.
	 * @return (*this)
	 */
	inline node< data_t > & link( const node< data_t > & _node )
	{
		_node.next = next;
		_node.prev = this;
		if( next != NULL )
			next->prev = _node;
		next = &_node;

		return (*this);
	}


	/**
	 * Assign new data to the node. The previous will be lost.
	 * @param _new_data  Data to be assigned
	 * @return (*this)
	 */
	inline node< data_t > & assign( const  data_t  & _new_data )
	{
		data = _new_data;
		return (*this);
	}

	/**
	 * Swap \c data_t pointers and \c indx numbers with another node.
	 * @param _node The other node to swap with.
	 */
	inline
	void swap( const node< data_t > & _node )
	{
		data_t * tmp_data = this->data;
		data_t * tmp_indx = this->indx;
		data = _node->data;
		indx = _node->indx;
		_node->data = tmp_data;
		_node->indx = tmp_indx;
	}

	/**
	 * The type of a comparison function of two nodes
	 * @param lhs 	The "left hand side" of the comparison function.
	 * @param rhs	The "right hand side" of the comparison function.
	 * @return 		The result of the comparison.
	 */
	typedef int (*compare_type)( node< data_t > * lhs, node< data_t > * rhs );

};

template< class data_t >
class list
{
	node< data_t > * first;
	node< data_t > * last;
	size_t number;

public:
//-----------------------------------------------------------------------------
//		Constructors and Destructors
//-----------------------------------------------------------------------------
	/**
	 * Null constructor
	 */
	list() :
			number( 0 )
	{
		first = last = NULL;
	}

	/**
	 * Move constructor
	 */
	list( list && ls ) :
			first( ls.first ), last( ls.last ), number( ls.number )
	{
		ls.first = ls.last = NULL;
		ls.number = 0;
	}

	/**
	 * Move constructor
	 */
	list( const node< data_t > & _first, const node< data_t > & _last ) :
			first( &_first ), last( &_last )
	{
		size_t ii = 0;
		for( node< data_t > * ptr = first; ptr != last; ++ii, ptr = ptr->next )
			;
		number = ii;
	}

	/**
	 * Adapter constructor
	 * @param node_array
	 * 		An array of nodes that the constructor can link together to
	 * 		construct a list.
	 */
	template< size_t N >
	list( node< data_t > (&node_array) [N] ) :
			first( &node_array [0] ), last( &node_array [N - 1] ), number( N )
	{
		for( size_t ii = 1; ii < N; ++ii )
		{
			node_array [ii - 1].indx = (ii - 1);
			node_array [ii - 1].next = &node_array [ii];
			node_array [ii].prev = &node_array [ii - 1];
		}
	}

	/**
	 * Destructor.
	 *
	 * Calls \c node::destroy() for all items in list.
	 */
	~list()
	{
		for( ; first != last && number != 0; number-- )
			first->unlink();
	}

//-----------------------------------------------------------------------------
//		Data access methods
//-----------------------------------------------------------------------------
	/**
	 * The number of nodes in the list.
	 * @note If the number of nodes is suspected to be incorrect,
	 * use \c recount() to find the true number of nodes.
	 * @return this->number
	 */
	inline size_t size() const
	{
		return number;
	}

	/**
	 * Whether the list is empty of not, basd on \c size()
	 * @note If the number of nodes is suspected to be incorrect,
	 * use \c recount() to find the true number of nodes.
	 * @return this->number
	 */
	inline bool empty() const
	{
		return number;
	}

	/**
	 * Access the \c first node in the list.
	 * @return	Reference to the first node.
	 */
	inline node< data_t > & front()
	{
		assert( first != NULL );
		return (*first);
	}

	/**
	 * Access the \c last node in the list.
	 * @return	Reference to the last node.
	 */
	inline node< data_t > & back()
	{
		assert( last != NULL );
		return (*last);
	}

	/** Access operator
	 * @note Index is interpreted modulo size, i.e. (index %= size).
	 * @param indx
	 * 		The index of the element desired.
	 * 		If (indx>=0), measured from front of list, i.e. front==[0].
	 * 		If (indx<0), measured from end of list, i.e. back==[-1].
	 * @return The node at the given index.
	 */
	inline node< data_t > & operator[]( long indx )
	{
		if( indx >= 0 )
		{
			node< data_t > * ptr = first;
			for( indx %= number; indx != 0; indx-- )
				ptr = ptr->next;
			return ptr;
		}
		else
		{
			node< data_t > * ptr = last;
			for( indx %= number; indx != 0; indx++ )
				ptr = ptr->prev;
			return ptr;
		}

	}

//-----------------------------------------------------------------------------
//		Data manipulators
//-----------------------------------------------------------------------------

	/**
	 * Remove a node from the front of the list.
	 * @return A pointer to the node removed, or NULL if list is empty.
	 */
	inline node< data_t > * pop_front()
	{
		if( first == last || number == 0 )
			return NULL;
		node< data_t > * ret = first;
		first->unlink();
		last = first->prev;
		first = last->next;
		return ret;
	}

	/**
	 * Remove a node from the back of the list.
	 * @return A pointer to the node removed, or NULL if list is empty.
	 */
	inline node< data_t > * pop_back()
	{
		if( first == last || number == 0 )
			return NULL;
		node< data_t > * ret = last;
		last->unlink();
		last = first->prev;
		first = last->next;
		return ret;
	}

	/**
	 * Put new node at the front of list.
	 * @param _node Reference to the node that will be added.
	 * @return (*this)
	 */
	inline list< data_t > & push_front( node< data_t > & _node )
	{
		_node.prev = first->prev;
		_node.next = first;
		last->next = &_node;
		first->prev = &_node;
		first = &_node;
		_node.indx = number++;
		return (*this);
	}

	/**
	 * Put new node at the back of list.
	 * @param _node Reference to the node that will be added.
	 * @return (*this)
	 */
	inline list< data_t > & push_back( node< data_t > & _node )
	{
		_node.next = last->next;
		_node.prev = last;
		last->next = &_node;
		first->prev = &_node;
		last = &_node;
		_node.indx = number++;
		return (*this);
	}

	/**
	 * Put new node at a given list index.
	 * @param _node Reference to the node that will be added.
	 * @return (*this)
	 */
	inline list< data_t > & insert( const node< data_t > & _node, size_t indx )
	{
		node< data_t > * ptr = first;
		while( ptr->indx < indx && ptr != last )
		{
			ptr = ptr->next;
		}
		switch( ptr )
		{
		case last:
			push_back( _node );
			break;
		case first:
			push_front( _node );
			break;
		default:
			ptr->link( _node );
			++number;
			break;
		}
		return (*this);
	}

	/**
	 * Shift-UP operator: cycles list items forward.
	 * @param indx	The number of indexes to cycle
	 * @return The list
	 */
	inline list< data_t > & operator<<=( size_t indx )
	{
		indx %= number;
		if( indx == 0 )
			return (*this);

		for( ; indx != 0; indx-- )
		{
			first = first->next;
			last = last->next;
		}
		return (*this);
	}

	/**
	 * Shift-DOWN operator: cycles list items backward.
	 * @param indx	The number of indexes to cycle
	 * @return The list
	 */
	inline list< data_t > & operator>>=( size_t indx )
	{
		indx %= number;
		if( indx == 0 )
			return (*this);

		for( ; indx != 0; indx-- )
		{
			first = first->prev;
			last = last->prev;
		}
		return (*this);
	}

	/**
	 * Count the number of nodes in the list and update
	 * the list's size if necessary.
	 * @return	The correct number of nodes in the list.
	 */
	inline size_t recount()
	{
		size_t count = 0;
		node<data_t> * p1 = first;
		while( p1 != last )
		{
			p1 = p1->next;
			++count;
		}
		return number = count;
	}

	/**
	 * Change the index of each node to match their current
	 * positions in the list.
	 * @return	The number of nodes in the list.
	 */
	inline size_t reindex()
	{
		size_t count = 0;
		node<data_t> * p1 = first;
		while( p1 != last )
		{
			p1->indx = count;
			p1 = p1->next;
			++count;
		}
		return number = count;
	}

	/**
	 * Sort the list using a hybrid of shaker and pivot
	 * \return (*this)
	 */
	inline list< data_t > & sort()
	{
		for( node< data_t > * f_ = first, l_ = last; f_ != l_; )
		{
			for( node< data_t > * m_ = f_->next; m_ != l_; )
			{
				if( l_->idx < f_->idx )
					l_->swap( *f_ );
				if( m_->idx < f_->idx )
					m_->swap( *f_ );
				if( m_->idx > l_->idx )
					m_->swap( *l_ );
			}
			f_ = f_->next;
			for( node< data_t > * m_ = l_->prev; m_ != f_; )
			{
				if( l_->idx < f_->idx )
					l_->swap( *f_ );
				if( m_->idx < f_->idx )
					m_->swap( *f_ );
				if( m_->idx > l_->idx )
					m_->swap( *l_ );
			}
			l_ = l_->prev;
		}
		return (*this);
	}

	/**
	 * Apply a function to every node in the list.
	 * @param node_function
	 * 		A pointer to the function to be called.
	 */
	inline
	void apply( void (*node_function)( node< data_t >& ) )
	{
		for( node< data_t > *ptr = first; ptr *= last; ptr = ptr->next )
		{
			(*node_function)( *ptr );
		}
	}
};

#endif /* LIST_H_ */

