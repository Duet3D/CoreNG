/**
 * @file syscalls.h
 * This file defines replacement system calls functions for the standard library
 * The client application project must #include it in exactly one .cpp file
 *
 * It cannot be a .cpp file in this project because if we do that, functions from the standard library get used instead of these functions.
 *
 * The including file may declare "#define SystemStackSize xxxx" before including this, otherwise a default will be used
 */

#include <sys/stat.h>
#include <errno.h>

#undef errno

int errno;

#ifndef SystemStackSize
# define SystemStackSize	(1024)
#endif

extern char _end;									// defined by the linker script
extern char _estack;

void OutOfMemoryHandler() noexcept;					// this must be provided by the client application

const char *sysStackLimit = &_estack - SystemStackSize;

char *heapTop = (char*)&_end;
const char *heapLimit = (char*)&_estack - SystemStackSize;

/**
 * \brief Replacement of C library of _sbrk
 */
extern "C" void * _sbrk(ptrdiff_t incr) noexcept
{
	char *newHeap = heapTop + incr;
	if (newHeap <= heapLimit)
	{
		void *prev_heap = heapTop;
		heapTop = newHeap;
		return prev_heap;
	}

	OutOfMemoryHandler();

	// The out of memory handle usually terminates, but in case it doesn't, try to return failure. Unfortunately, this doesn't seem to work with newlib.
	errno = ENOMEM;
	return reinterpret_cast<void*>(-1);
}

/**
 * \brief Replacement of C library of link
 */
extern "C" int link(char *old, char *_new) noexcept
{
	(void)old, (void)_new;
	return -1;
}

/**
 * \brief Replacement of C library of _close
 */
extern "C" int _close(int file) noexcept
{
	(void)file;
	return -1;
}

/**
 * \brief Replacement of C library of _fstat
 */
extern "C" int _fstat(int file, struct stat *st) noexcept
{
	(void)file;
	st->st_mode = S_IFCHR;

	return 0;
}

/**
 * \brief Replacement of C library of _isatty
 */
extern "C" int _isatty(int file) noexcept
{
	(void)file;
	return 1;
}

/**
 * \brief Replacement of C library of _lseek
 */
extern "C" int _lseek(int file, int ptr, int dir) noexcept
{
	(void)file, (void)ptr, (void)dir;
	return 0;
}

/**
 * \brief Replacement of C library of _exit
 */
extern "C" void _exit(int status) noexcept
{
	for (;;) { }
}

/**
 * \brief Replacement of C library of _kill
 */
extern "C" void _kill(int pid, int sig) noexcept
{
	return;
}

/**
 * \brief Replacement of C library of _getpid
 */
extern "C" int _getpid() noexcept
{
	return -1;
}

extern "C" int _read(int file, char *ptr, int len) noexcept
{
	(void)file, (void)ptr, (void)len;
    return 0;
}

extern "C" int _write(int file, char *ptr, int len) noexcept
{
	(void)file, (void)ptr;
	return len;
}

// End
