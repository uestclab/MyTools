#include "smallobject_allocator.h"
#include <iostream>

using namespace std;

smallobject_allocator* smallobject_allocator::_instance=0;

long smallobject_allocator::_singleton_guard = 0;

bool smallobject_allocator::_singleton_destroyed=false;

void smallobject_allocator::create_instance()
{
	//thread_guard guard(&_singleton_guard);
	if(_instance)
		return;
	assert(!_singleton_destroyed);
	static smallobject_allocator obj;
	_instance = &obj;
}

smallobject_allocator::smallobject_allocator()
{
	_chunk_list=0;
	_chunk_guard=0;
	::memset(_free_list,0,sizeof(_free_list));
	::memset(_guard,0,sizeof(_guard));
}

smallobject_allocator::~smallobject_allocator()
{
	int s=0;
	chunk_list *temp=_chunk_list;
	while(temp){
		++s;
		temp=temp->_next;
	}
	
	void **chunk=reinterpret_cast<void **>(malloc(s*sizeof(void *)));
	temp=_chunk_list;
	int i=0;
	while(temp){
		chunk[i]=temp->_data;
		++i;
		temp=temp->_next;
	}
	for(i=0;i<s;i++){
		::free(chunk[i]);
	}
	::free(chunk);
	
	_singleton_destroyed=true;
	_instance=0;
}

inline size_t _min(size_t a,size_t b)
{
	return a<b?a:b;
	
}

smallobject_allocator::memory_list*
smallobject_allocator::alloc_chunk(size_t idx)
{
	const size_t node_size = (idx+1)*align_size;
	const size_t chunk_size=_min(chunk_limit/node_size*node_size,node_size*max_number);
	//thread_guard guard(&_chunk_guard);
	memory_list * &current_list=_free_list[idx];
	if(current_list)
		return current_list;
	memory_list *ret=current_list=reinterpret_cast
									<memory_list*>(::malloc(chunk_size));
	memory_list *iter = ret;
	for(size_t i=0;i<=chunk_size-node_size*2;i+=node_size){
		iter->_next=iter+node_size/sizeof(*iter);
		iter=iter->_next;
	}
	iter->_next=0;
	
	return ret;
}

void* smallobject_allocator::allocate(size_t size)
{
	size_t idx=chunk_index(size);
	assert(idx<chunk_number);
	
	//thread_guard guard(&_guard[idx]);
	
	memory_list * &temp=_free_list[idx];//这里引用很关键
	
	if(!temp){
		memory_list *new_chunk = alloc_chunk(idx);
		
		chunk_list *chunk_node;
		if(chunk_index(sizeof(chunk_list))==idx){
			chunk_node=reinterpret_cast<chunk_list *>(temp);
			temp=temp->_next;
		}
		else{
			chunk_node=reinterpret_cast<chunk_list*>(allocate(sizeof(chunk_list)));
			
		}
		
		//thread_guard guard(&_chunk_guard);
		chunk_node->_next=_chunk_list;
		chunk_node->_data=new_chunk;
		_chunk_list=chunk_node;
	}
	
	void *ret=temp;
	temp=temp->_next;
	
	return ret;
}

void smallobject_allocator::deallocate(void *p,size_t size)
{
	size_t idx=chunk_index(size);
	assert(idx<chunk_number);
	
	memory_list * free_block = reinterpret_cast<memory_list*>(p);
	
	//thread_guard guard(&_guard[idx]);
	
	memory_list * &temp=_free_list[idx];
	free_block->_next = temp;
	temp=free_block;
}