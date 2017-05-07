#include <sl.h>
#include <stdio.h>
#include "push.h"


UINT32 BytesAllocated;
extern ULONG PushSessionId;


typedef enum _SECTION_INHERIT
{
    ViewShare = 1,
    ViewUnmap = 2
} SECTION_INHERIT;


NTSTATUS __stdcall NtMapViewOfSection(
    VOID* SectionHandle,
    VOID* ProcessHandle,
    VOID** BaseAddress,
    UINT_B ZeroBits,
    UINT_B CommitSize,
    LARGE_INTEGER* SectionOffset,
    UINT_B* ViewSize,
    SECTION_INHERIT InheritDisposition,
    ULONG AllocationType,
    ULONG Win32Protect
    );

SIZE_T __stdcall RtlSizeHeap(
    _In_ VOID* HeapHandle,
    _In_ ULONG Flags,
    _In_ VOID* BaseAddress
    );


VOID* PushBaseGetNamedObjectDirectory();


#define SEC_COMMIT   0x8000000
#define HEAP_GENERATE_EXCEPTIONS        0x00000004


VOID* Memory_CreateFileMapping( WCHAR* FileName, DWORD Size, HANDLE* SectionHandle )
{
    NTSTATUS status;
    HANDLE sectionHandle;
    OBJECT_ATTRIBUTES objAttrib = {0};
    UNICODE_STRING sectionName;
    LARGE_INTEGER sectionSize;
    VOID* viewBase;

    UnicodeString_Init(&sectionName, FileName);

    objAttrib.Length = sizeof(OBJECT_ATTRIBUTES);
    objAttrib.RootDirectory = PushBaseGetNamedObjectDirectory();
    objAttrib.Attributes = OBJ_OPENIF;
    objAttrib.ObjectName = &sectionName;
    objAttrib.SecurityDescriptor = NULL;
    objAttrib.SecurityQualityOfService = NULL;

    sectionSize.u.LowPart = Size;
    sectionSize.u.HighPart = 0;

    status = NtCreateSection(
        &sectionHandle,
        STANDARD_RIGHTS_REQUIRED | SECTION_MAP_READ | SECTION_MAP_WRITE | SECTION_QUERY,
        &objAttrib,
        &sectionSize,
        PAGE_READWRITE,
        SEC_COMMIT,
        NULL
        );

    *SectionHandle = sectionHandle;

    //must be NULL or will fail
    viewBase = NULL;

    status = NtMapViewOfSection(
        sectionHandle,
        NtCurrentProcess(),
        &viewBase,
        0,
        0,
        NULL,
        &sectionSize.u.LowPart,
        ViewShare,
        0,
        PAGE_READWRITE
        );

    return viewBase;
}


VOID* Memory_Allocate( UINT_B Size )
{
#if DEBUG
    //BytesAllocated += Size;

    //Log(L"Allocating %i bytes, BytesAllocated: %i", Size, BytesAllocated);
#endif

    return RtlAllocateHeap(PushHeapHandle, 0, Size);
}


VOID* Memory_AllocateEx( UINT_B Size, DWORD Flags )
{
    BytesAllocated += Size;

    return RtlAllocateHeap(PushHeapHandle, Flags, Size);
}


VOID* Memory_ReAllocate( VOID* Memory, SIZE_T Size )
{
    return RtlReAllocateHeap(PushHeapHandle, HEAP_GENERATE_EXCEPTIONS, Memory, Size);
}


VOID Memory_Free( VOID* Heap )
{
    if (!Heap)
    {
        Log(L"Why are you trying to free memory that doesn't exist!?");

        return;
    }

#if DEBUG
    //SIZE_T size;

    //size = RtlSizeHeap(PushHeapHandle, 0, Heap);

    //Log(L"Freeing %i bytes of memory", size);

    //BytesAllocated -= size;
#endif

    RtlFreeHeap(PushHeapHandle, 0, Heap);
}


#include <string.h>
VOID Memory_Copy( VOID* Destination, VOID* Source, UINT32 Length )
{
    memcpy(Destination, Source, Length);
}


VOID Memory_Clear( VOID* Region, UINT32 Size )
{
    memset(Region, 0, Size);
}


WCHAR* Memory_FindFirstChar(const WCHAR *ptr, WCHAR ch, UINT32 n)
{
    const WCHAR *end;
    for (end = ptr + n; ptr < end; ptr++)
        if (*ptr == ch)
            return (WCHAR *)(UINT_B)ptr;
    return NULL;
}