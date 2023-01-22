/*
  Moresampler - More than a resampler
  ===
  Copyright (c) 2015-2016 Kanru Hua. All rights reserved.
*/

#ifndef MORESMPL_MRQ
#define MORESMPL_MRQ

#ifdef __cplusplus
extern "C" {
#endif

#include <wchar.h>
#include <stdio.h>
#include <stdint.h>

#define MRQ_VERSION 2

/* sequence of storage:
   int32_t     nfilename
   short int[] filename
   int32_t     size of the following data
   int32_t     nf0
   int32_t     fs
   int32_t     nhop
   float[]     f0
   int32_t     unix timestamp
   int32_t     modified by hand (0: true, >=1: false)
*/
typedef struct {
  int nhop;
  int fs;
  double* f0;
  int nf0;
  int32_t timestamp; // read-only
  int modified;
} mrq_entry;

typedef int (*mrq_fenum)(const wchar_t*, mrq_entry*, void*);

void mrq_set_version(int version);

mrq_entry* create_mrq_entry(int nf0);
void delete_mrq_entry(mrq_entry* dst);

/*
  note: 
  (1) UTAU passes SHIFT-JIS-encoded filenames to resampler & wavtool. Through Windows
        API, the filenames are converted to UTF-16 in Moresampler. Then UTF-16-encoded
        filenames are stored as arrays of 2-byte wchars in a mrq file.
  (2) the current version of mrq stores all key-value pairs in a linear structure
        whose time complexity of random accessing is O(N) where N is the number of
        data records. Comparing to the classical way of storing a bunch of frq files
        in the same directory (whose indexing is based on B-Tree, as for NTFS),
        mrq is less efficient. However in most of the cases N is a small number so
        this inefficiency can be neglected. So in overall, mrq is more efficient.
        I would still be glad if someone could implement more efficient indexing
        (e.g. hash table) for future versions.
*/

// mode can be either L"r" or L"rw"
// returns NULL on error
FILE* mrq_open(const wchar_t* path, const wchar_t* mode);

// returns -1 on error
int mrq_get_version(FILE* src);

// returns NULL on error
mrq_entry* mrq_get_entry(FILE* src, const wchar_t* name);

// returns 0 on error
int mrq_write_entry(FILE* dst, const wchar_t* name, mrq_entry* src);

// mrq_delete_entry doesn't actually delete the entry but it wipes out the entry
//   by marking it as invalid. The space occupied is released when defragmentation
//   takes place.
// returns -1 if the designated entry is not found
// returns 0 on error
int mrq_delete_entry(FILE* dst, const wchar_t* name);

// returns 0 on error; so does enumproc
int mrq_enumerate(FILE* src, mrq_fenum enumproc, void* param);

// returns 0 on error
// CLOSE all file pointers to <path> before calling this function
int mrq_defragment(const wchar_t* path);

#ifdef __cplusplus
}
#endif

#endif

