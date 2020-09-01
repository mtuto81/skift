#include <libsystem/Logger.h>
#include <libsystem/thread/Atomic.h>
#include <libsystem/utils/List.h>

#include "kernel/memory/Memory.h"
#include "kernel/memory/MemoryObject.h"
#include "kernel/memory/Physical.h"

static int _memory_object_id = 0;
static List *_memory_objects;

void memory_object_initialize()
{
    _memory_objects = list_create();
}

MemoryObject *memory_object_create(size_t size)
{
    size = PAGE_ALIGN_UP(size);

    MemoryObject *memory_object = __create(MemoryObject);

    memory_object->id = _memory_object_id++;
    memory_object->refcount = 1;
    memory_object->_range = physical_alloc(size);

    atomic_begin();
    list_pushback(_memory_objects, memory_object);
    atomic_end();

    return memory_object;
}

void memory_object_destroy(MemoryObject *memory_object)
{
    list_remove(_memory_objects, memory_object);

    physical_free(memory_object->range());
    free(memory_object);
}

MemoryObject *memory_object_ref(MemoryObject *memory_object)
{
    __atomic_add_fetch(&memory_object->refcount, 1, __ATOMIC_SEQ_CST);

    return memory_object;
}

void memory_object_deref(MemoryObject *memory_object)
{
    atomic_begin();

    if (__atomic_sub_fetch(&memory_object->refcount, 1, __ATOMIC_SEQ_CST) == 0)
    {
        memory_object_destroy(memory_object);
    }

    atomic_end();
}

MemoryObject *memory_object_by_id(int id)
{
    atomic_begin();

    list_foreach(MemoryObject, memory_object, _memory_objects)
    {
        if (memory_object->id == id)
        {
            memory_object_ref(memory_object);

            atomic_end();

            return memory_object;
        }
    }

    atomic_end();

    return nullptr;
}
