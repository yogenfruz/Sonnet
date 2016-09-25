#include <malloc.h>
#include <cstdlib>

void* operator new(size_t size)
{
	return _aligned_malloc(size, 16);
}

void* operator new[](size_t size)
{ 
	return _aligned_malloc(size, 16); 
}

void* operator new[](size_t size, const char* /*name*/, int /*flags*/, unsigned /*debugFlags*/, const char* /*file*/, int /*line*/)
{ 
	return _aligned_malloc(size, 16); 
}

void* operator new[](size_t size, size_t alignment, size_t /*alignmentOffset*/, const char* /*name*/, int /*flags*/, unsigned /*debugFlags*/, const char* /*file*/, int /*line*/)
{ 
	return _aligned_malloc(size, alignment); 
}

void* operator new(size_t size, size_t alignment)
{
	return _aligned_malloc(size, alignment);
}

void operator delete(void* p, std::size_t)
{
	_aligned_free(p);
}

void operator delete[](void* p, std::size_t)
{
	_aligned_free(p);
}

void operator delete(void* p)
{
	_aligned_free(p);
}

void operator delete[](void* p)
{
	_aligned_free(p);
}