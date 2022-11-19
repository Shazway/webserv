#pragma once
#include <iterator>
#include <stdexcept>
#include <vector>
#include "traits.hpp"

namespace ft
{
	template<class T, class Allocator = std::allocator<T> >
	class vector
	{
		public:
			typedef T											value_type;
			typedef Allocator									allocator_type;
			typedef typename allocator_type::reference			reference;
			typedef typename allocator_type::const_reference	const_reference;
			typedef typename allocator_type::pointer			pointer;
			typedef typename allocator_type::const_pointer		const_pointer;
			typedef typename std::vector<T>::iterator			iterator;
			typedef typename std::vector<T>::const_iterator		const_iterator;
			typedef typename std::vector<T>::reverse_iterator			reverse_iterator;
			typedef typename std::vector<T>::const_reverse_iterator		const_reverse_iterator;
			typedef typename std::iterator_traits<iterator>::difference_type	difference_type;
			typedef	unsigned int									size_type;

		protected:
			value_type*	_ptr;
			size_type	_size;
			size_type	_capacity;
			allocator_type	_alloc;

		public:
			//constructors
			vector() : _ptr(NULL), _size(0), _capacity(0), _alloc(Allocator()) {};
			explicit vector( const Allocator& alloc ) : _ptr(NULL), _size(0), _capacity(0), _alloc(alloc){};
			explicit vector( size_type count,
				const T& value = T(),
				const Allocator& alloc = Allocator()): _ptr(NULL), _size(0), _capacity(0), _alloc(alloc)
				{
					this->insert(this->begin(), count, value);
				};
			template< class InputIt >
				vector( InputIt first, InputIt last,
					const Allocator& alloc = Allocator() ) : _ptr(NULL), _size(0), _capacity(0), _alloc(alloc)
					{
						this->insert(this->begin, first, last);
					};
			vector( const vector& other )
			{
				_size = other._size;
				_capacity = _size;
				_alloc = other._alloc;
				_ptr = _alloc.allocate(_size);
				this = other;
			};
			//destructor
			~vector()
			{
				if (_capacity > 0)
					_alloc.deallocate(_ptr, _capacity);
			};
			//operator=
			vector& operator=( const vector& other )
			{
				assign(other.begin(), other.end());
			};
			//assign
			void assign( size_type count, const T& value )
			{
				if (_capacity > count)
				{
					if (_capacity > 0)
						_alloc.deallocate(_capacity);
					_ptr = _alloc.allocate(count);
					_capacity = count;
				}
				for (size_type i = 0; i < count; i++)
					_ptr[i] = value;
				_size = count;
			};
			template< class InputIt >
				void assign( InputIt first, InputIt last )
				{
					size_type	size = 0;
					InputIt		tmp = first;

					while (tmp != last)
					{
						size++;
						tmp++;
					}
					if (_capacity > size)
					{
						if (_capacity > 0)
							_alloc.deallocate(_capacity);
						_ptr = _alloc.allocate(size);
						_capacity = size;
					}
					int i = 0;
					while (first != last)
					{
						_ptr[i] = *first;
						i++;
						first++;
					}
					_size = size;
				};
			//get_allocator
			allocator_type get_allocator() const
			{
				return (_alloc);
			};

			/*******************
			** Element access **
			********************/
			//at
			reference at( size_type pos )
			{
				if (!(pos < this->size()))
					throw (std::out_of_range("out of range"));
				return(*(_ptr + pos));
			};
			const_reference at( size_type pos ) const
			{
				if (!(pos < this->size()))
					throw (std::out_of_range("out of range"));
				return(*(_ptr + pos));
			};
			//operator[]
			reference operator[]( size_type pos )
			{
				return(*(_ptr + pos));
			};
			const_reference operator[]( size_type pos ) const
			{
				return(*(_ptr + pos));
			};
			//front
			reference front()
			{
				return (*_ptr);
			};
			const_reference front() const
			{
				return (*_ptr);
			};
			//back
			reference back()
			{
				if (_size < 2)
					return (*_ptr);
				return(*(_ptr + _size - 1));
			};
			const_reference back() const
			{
				if (_size < 2)
					return (*_ptr);
				return(*(_ptr + _size - 1));
			};
			//data
			T* data()
			{
				return (_ptr);
			};
			const T* data() const
			{
				return (_ptr);
			};

			/**************
			** Iterators **
			***************/
			//begin
			iterator begin()
			{
				iterator	it(_ptr);
				return (it);
			};
			const_iterator begin() const
			{
				const_iterator	it(_ptr);
				return (it);
			};
			//end
			iterator end()
			{
				iterator	it(_ptr + _size);
				return (it);
			};
			const_iterator end() const
			{
				iterator	it(_ptr + _size);
				return (it);
			};
			//rbegin
			reverse_iterator rbegin()
			{
				reverse_iterator	it(_ptr + _size);
				return (it);
			};
			const_reverse_iterator rbegin() const
			{
				const_reverse_iterator	it(_ptr + _size);
				return (it);
			};
			//rend
			reverse_iterator rend()
			{
				reverse_iterator	it(_ptr);
				return (it);
			};
			const_reverse_iterator rend() const
			{
				const_reverse_iterator	it(_ptr);
				return (it);
			};

			/*************
			** Capacity **
			**************/
			bool empty() const
			{
				if (_size == 0)
					return (true);
				return (false);
			};
			size_type size() const
			{
				return (_size);
			};
			size_type max_size() const
			{
				return (_alloc.max_size());
			};
			void reserve( size_type new_cap )
			{
				if (new_cap > max_size())
					throw (std::length_error("length error"));
				if (new_cap > _capacity)
				{
					T* new_ptr;
					new_ptr = _alloc.allocate(new_cap);
					for (int i = 0; i < _size; i++)
						new_ptr[i] = _ptr[i];
					if (_capacity > 0)
						_alloc.deallocate(_ptr, _capacity);
					_ptr = new_ptr;
					_capacity = new_cap;
				}
			};
			size_type capacity() const
			{
				return (_capacity);
			};

			/**************
			** Modifiers **
			***************/
			void clear()
			{
				_size = 0;
			};
			//insert
			iterator insert( iterator pos, const T& value )
			{
				int	i = 0;
				iterator	it;
				if (_size == _capacity)
				{
					T*	new_ptr = _alloc.allocate(_capacity * 2 < max_size() ? _capacity *2 : max_size());
					it = begin();
					while (it != pos)
					{
						new_ptr[i++] = *it;
						it++;
					}
					new_ptr[i++] = value;
					it = pos;
					while (it != end())
					{
						new_ptr[i++] = *it;
						it++;
					}
					if (_capacity > 0)
						_alloc.deallocate(_ptr, _capacity);
					_ptr = new_ptr;
					_capacity = _capacity * 2 < max_size() ? _capacity *2 : max_size();
				}
				else
				{
					it = end();
					while (it != pos)
					{
						*it = *it - 1;
						it--;
					}
					*it = value;
				}
				_size += 1;
				return (pos);
			};

			void insert( iterator pos, size_type count, const T& value)
			{
				iterator it;
				size_type	i = 0;
				size_type	space = _capacity;
				//	std::cout << "coucou" << std::endl;
				if (_size + count > _capacity)
				{
					while (space < _size + count)
						space *= 2;
					if (space > max_size())
						space = max_size();
					T*	new_ptr = _alloc.allocate(space);
					it = begin();
					while (it != pos)
					{
						new_ptr[i++] = *it;
						it++;
					}
					for (size_type j = 0; j < count; j++)
						new_ptr[i++] = value;
					it = pos;
					while (it != end())
					{
						new_ptr[i++] = *it;
						it++;
					}
					if (_capacity > 0)
						_alloc.deallocate(_ptr, _capacity);
					_ptr = new_ptr;
					_capacity = space;
				}
				else
				{
					if (_size > 0)
					{
						size_type	last = _size - 1;
						_size += count;
						it = begin();
						while (it != pos)
							i++;
						while (last >= i)
						{
							_ptr[last + count] = _ptr[last];
							last--;
						}
						for (size_type j = 0; j < count; j++)
							_ptr[last] = value;
					}
					else
					{
						_size = count;
						while (i < count)
							_ptr[i++] = value;
					}
					
				}
				_size += count;
			};
			template< class InputIt >
				void insert( iterator pos, InputIt first, InputIt last, typename enable_if<!is_integral<InputIt>::value, int>::type = 0)
				{
				iterator	it;
				size_type	i = 0;
				size_type count = 1;
				size_type	space = _capacity;
				InputIt inIt = first;
					std::cout << "coucou" << std::endl;

				while (inIt != last)
				{
					count++;
					inIt++;
				}
				if (_size + count > _capacity)
				{
					while (space < _size + count)
						space *= 2;
					if (space > max_size())
						space = max_size();
					T*	new_ptr = _alloc.allocate(space);
					it = begin();
					while (it != pos)
					{
						new_ptr[i++] = *it;
						it++;
					}
					for (size_type j = 0; j < count; j++)
						new_ptr[i++] = first++;
					it = pos;
					while (it != end())
					{
						new_ptr[i++] = *it;
						it++;
					}
					if (_capacity > 0)
						_alloc.deallocate(_ptr, _capacity);
					_ptr = new_ptr;
					_capacity = space;
				}
				else
				{
					if (_size > 0)
					{
						size_type	last = _size - 1;
						_size += count;
						it = begin();
						while (it != pos)
							i++;
						while (last >= i)
						{
							_ptr[last + count] = _ptr[last];
							last--;
						}
						for (size_type j = 0; j < count; j++)
							_ptr[last] = first;
					}
					else
					{
						_size = count;
						while (i < count)
							_ptr[i++] = first++;
					}
					
				}
				_size += count;
				return ;
			};
			//erase
			iterator erase( iterator pos )
			{
				iterator	it = begin();
				while (it != pos)
					it++;
				while (it + 1 != end())
				{
					*it = *it + 1;
					it++;
				}
				_size -= 1;
				return (pos);
			};
			iterator erase( iterator first, iterator last )
			{
				iterator	it = begin();
				int			bin = 1;
				while (it != first)
					it++;
				while (it != last)
				{
					bin++;
					it++;
				}
				while (it + bin != end())
				{
					*it = *it + bin;
					it++;
				}
				_size -= bin;
				return (end());
			};
			//
			void push_back( const T& value )
			{
				size_type	i = 0;
				if (_size == _capacity)
				{
					size_type	new_capacity = _capacity;
					new_capacity * 2 < max_size() ? new_capacity *2 : max_size();
					new_capacity = new_capacity ? new_capacity : 1;
					T*	new_ptr = _alloc.allocate(new_capacity);
					while (i < _size)
					{
						new_ptr[i] = _ptr[i];
						i++;
					}
					new_ptr[i++] = value;
					if (_capacity > 0)
						_alloc.deallocate(_ptr, _capacity);
					_ptr = new_ptr;
					_capacity = new_capacity;
				}
				else
					_ptr[_size] = value;
				_size += 1;
			};
			void pop_back()
			{
				_size--;
			};
			//resize
			void resize( size_type count, T value = T() )
			{
				size_type	i = 0;
				size_type	space = _capacity;
				if (count > _capacity)
				{
					while (space < count)
						space *= 2;
					if (space > max_size())
						space = max_size();
					T*	new_ptr = _alloc.allocate(space);
					while (i < _size)
					{
						new_ptr[i] = _ptr[i];
						i++;
					}
					while (i < count)
						new_ptr[i++] = value;
					if (_capacity > 0)
						_alloc.deallocate(_ptr, _capacity);
					_ptr = new_ptr;
					_capacity = space;
				}
				_size = count;
			};
			//
			void swap( vector& other )
			{
				value_type* tmp_ptr = other._ptr;
				size_type	tmp_size = other._size;
				size_type	tmp_capacity = other._capacity;
				allocator_type	tmp_alloc = other._alloc;
				other._ptr = _ptr;
				other._size = _size;
				other._capacity = _capacity;
				other._alloc = _alloc;
				_ptr = tmp_ptr;
				_size = tmp_size;
				_capacity = tmp_capacity;
				_alloc = tmp_alloc;
			};
	};

};
