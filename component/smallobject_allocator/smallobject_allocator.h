#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define chunk_limit 16384
#define max_number  64
#define align_size  8
#define chunk_number chunk_limit/align_size

class smallobject_allocator
{
	private:
		struct memory_list{
			struct memory_list *_next;
		};
		
		struct chunk_list{
			struct chunk_list *_next;
			struct memory_list *_data;
		};
		
		memory_list *_free_list[chunk_number];
		
		long _guard[chunk_number];
		
		chunk_list *_chunk_list;
		
		long _chunk_guard;
		
		static smallobject_allocator* _instance;
		
		static long _singleton_guard;
		
		static bool _singleton_destroyed;
		
		static void create_instance();
		
		static size_t chunk_index(size_t bytes){
			
			size_t idx=(bytes - 1) / align_size;
			assert(idx >= 0 && idx < chunk_number);
			return idx;
			
		}
		
		smallobject_allocator();
		
		memory_list* alloc_chunk(size_t idx);
		
		public:
		
		~smallobject_allocator();
		
		static smallobject_allocator& instance(){
			if(!_instance){
				 create_instance();
			}
			return *_instance;
			
		}

		void* allocate(size_t size);

		void deallocate(void *p,size_t size);
};

