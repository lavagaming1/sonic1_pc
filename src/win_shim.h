#ifndef WIN_SHIM_H
#define WIN_SHIM_H
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#ifndef MAP_32BIT
#define MAP_32BIT 0
#endif
#ifndef MAP_FAILED
#define MAP_FAILED ((void*)-1)
#endif

static inline ssize_t win_readlink(const char *path, char *buf, size_t bufsiz) {
    if (path && strcmp(path, "/proc/self/exe") == 0) {
        DWORD len = GetModuleFileNameA(NULL, buf, (DWORD)bufsiz);
        if (len > 0 && len < bufsiz) {
            for (DWORD i = 0; i < len; i++) {
                if (buf[i] == '\\') buf[i] = '/';
            }
            return (ssize_t)len;
        }
    }
    return -1;
}
#ifndef readlink
#define readlink win_readlink
#endif

static inline void* win_mmap(void *addr, size_t len, int prot, int flags, int fd, long long offset) {
    (void)addr; (void)prot; (void)flags;
    if (fd != -1) {
        HANDLE hFile = (HANDLE)_get_osfhandle(fd);
        if (hFile == INVALID_HANDLE_VALUE) return MAP_FAILED;
        HANDLE hMap = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
        if (!hMap) return MAP_FAILED;
        void *ptr = MapViewOfFile(hMap, FILE_MAP_READ, 0, (DWORD)(offset >> 32), (DWORD)offset);
        CloseHandle(hMap);
        return ptr ? ptr : MAP_FAILED;
    }
    static uintptr_t low_mem_addr = 0x10000000;
    void *ptr = NULL;
    for (int i = 0; i < 512 && !ptr; i++) {
        ptr = VirtualAlloc((void*)low_mem_addr, len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        low_mem_addr += 0x00100000;
        if (low_mem_addr >= 0x7F000000) low_mem_addr = 0x10000000;
    }
    if (!ptr) ptr = VirtualAlloc(NULL, len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    return ptr ? ptr : MAP_FAILED;
}

static inline int win_munmap(void *addr, size_t len) {
    (void)len;
    if (UnmapViewOfFile(addr)) return 0;
    return VirtualFree(addr, 0, MEM_RELEASE) ? 0 : -1;
}

#define mmap win_mmap
#define munmap win_munmap
#endif
#endif
