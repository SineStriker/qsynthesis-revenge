/*
  Moresampler - More than a resampler
  ===
  Copyright (c) 2015-2016 Kanru Hua. All rights reserved.
  
  Jan 30, 2016 - add mrq_enumerate (by Masao)
  Apr 5, 2016 - version 2: add timestamp and modification status into mrq_entry,
    while being forward & backward compatible with version 1.
  Apr 8, 2016 - fix a bug in mrq_enumerate;
                add mrq_set_version.

  License for mrq.c
  ===
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

  3. Neither the name of the copyright holder nor the names of its contributors
  may be used to endorse or promote products derived from this software without
  specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "mrq.h"
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

static int write_version = MRQ_VERSION;

FILE* _wfopen(const wchar_t *filename, const wchar_t *mode);

static FILE* generic_wfopen(const wchar_t* filename, const wchar_t* mode) {
  FILE* fp;
# ifdef __MINGW32__
  fp = _wfopen(filename, mode);
# else
  char* cfilename = calloc(2048, sizeof(wchar_t));
  wcstombs(cfilename, filename, 2048);
  char cmode[64];
  wcstombs(cmode, mode, 64);
  fp = fopen(cfilename, cmode);
  free(cfilename);
# endif
  return fp;
}

void mrq_set_version(int version) {
  write_version = version;
}

mrq_entry* create_mrq_entry(int nf0) {
  mrq_entry* ret = calloc(1, sizeof(mrq_entry));
  ret -> nf0 = nf0;
  ret -> f0 = calloc(nf0, sizeof(double));
  return ret;
}

void delete_mrq_entry(mrq_entry* dst) {
  if(dst == NULL) return;
  free(dst -> f0);
  free(dst);
}

FILE* mrq_open(const wchar_t* path, const wchar_t* mode) {
  wchar_t* fopen_mode = NULL;
  if(! wcscmp(mode, L"rw"))
    fopen_mode = L"rb+";
  else
    fopen_mode = L"rb";
  FILE* fp = generic_wfopen(path, fopen_mode);
  if(! wcscmp(mode, L"r")) return fp;
  if(fp == NULL) {
    fp = generic_wfopen(path, L"w"); // create an empty file
    if(fp == NULL) return NULL;
    fclose(fp);
    fp = generic_wfopen(path, fopen_mode);
    if(fp == NULL) return NULL;
  }
  fseek(fp, 0, SEEK_END);
  int size = ftell(fp);
  if(size == 0) { // initialize
    int32_t version = write_version;
    int32_t nentry = 0;
    fwrite("mrq ", 1, 4, fp);
    fwrite(& version, 4, 1, fp);
    fwrite(& nentry, 4, 1, fp);
  }
  return fp;
}

int mrq_get_version(FILE* src) {
  int32_t ret = 0;
  if(fseek(src, 4, SEEK_SET) == -1) return -1;
  if(fread(& ret, 4, 1, src) == 0) return -1;
  return ret;
}

/* wchar_t on windows: 2 bytes
   wchar_t on linux:   4 bytes
   thus conversion is needed
*/
static wchar_t* short_to_wchar(const short int* src, int nsrc) {
  wchar_t* ret = calloc(nsrc + 1, sizeof(wchar_t));
  for(int i = 0; i < nsrc; i ++) ret[i] = src[i];
  return ret;
}

static short int* wchar_to_short(const wchar_t* src, int nsrc) {
  short int* ret = calloc(nsrc + 1, sizeof(short int));
  for(int i = 0; i < nsrc; i ++) ret[i] = src[i];
  return ret;
}

static int mrq_seek_entry(FILE* src, const wchar_t* name) {
  int nentry = 0;
  if(fseek(src, 8, SEEK_SET) == -1) return -1;
  if(fread(& nentry, 4, 1, src) == 0) return -1;
  for(int i = 0; i < nentry; i ++) {
    int32_t pos = 0, nfilename = 0, size = 0;
    pos = ftell(src);
    if(fread(& nfilename, 4, 1, src) == 0) return -1;
    short int* filename = calloc(nfilename, sizeof(short int));
    if(fread(filename, 2, nfilename, src) == 0) { free(filename); return -1;}
    wchar_t* wfilename = short_to_wchar(filename, nfilename);
    free(filename);
    if(fread(& size, 4, 1, src) == 0) { free(wfilename); return -1;}
    if(! wcscmp(wfilename, name)) {
      free(wfilename);
      fseek(src, pos, SEEK_SET);
      return i;
    } else {
      free(wfilename);
      if(fseek(src, size, SEEK_CUR) == -1) return -1; // skip nf0, fs, nhop, f0[]
    }
  }
  return -1;
}

static mrq_entry* mrq_get_entry_without_seeking(FILE* src) {
  int32_t nfilename, size = 0, nf0 = 0, fs = 0, nhop = 0, timestamp = 0, modified = 0;
  fread(& nfilename, 4, 1, src);
  fseek(src, nfilename * 2, SEEK_CUR);
  fread(& size, 4, 1, src);
  fread(& nf0, 4, 1, src);
  if(fread(& fs, 4, 1, src) == 0) return NULL;
  if(fread(& nhop, 4, 1, src) == 0) return NULL;
  float* f0_float = calloc(nf0, sizeof(float));
  if(fread(f0_float, 4, nf0, src) == 0) { free(f0_float); return NULL;}
  if(size >= 20 + nf0 * 4) { // version with timestamp and modification status extension
    if(fread(& timestamp, 4, 1, src) == 0) return NULL;
    if(fread(& modified, 4, 1, src) == 0) return NULL;
  }
  mrq_entry* ret = create_mrq_entry(nf0);
  ret -> fs = fs;
  ret -> nhop = nhop;
  ret -> timestamp = timestamp;
  ret -> modified = modified;
  for(int j = 0; j < nf0; j ++) ret -> f0[j] = f0_float[j];
  free(f0_float);
  return ret;
}

mrq_entry* mrq_get_entry(FILE* src, const wchar_t* name) {
  if(mrq_seek_entry(src, name) == -1) return NULL;
  return mrq_get_entry_without_seeking(src);
}

int mrq_write_entry(FILE* dst, const wchar_t* name, mrq_entry* src) {
  int32_t nfilename, size = 0, nf0 = 0, fs = 0, nhop = 0, modified = 0;
  int append = 0;
  
  int fixed_size = write_version == 1 ? 12 : 20;
  
  if(mrq_seek_entry(dst, name) != -1) { // found an existing entry
    int pos = ftell(dst);
    fread(& nfilename, 4, 1, dst);
    fseek(dst, nfilename * 2, SEEK_CUR);
    fread(& size, 4, 1, dst);
    if(size - fixed_size < src -> nf0 * 4) { // insufficient space for overwriting
      append = 1;
      fseek(dst, pos + 4, SEEK_SET);
      short int* null_string = calloc(nfilename, sizeof(short int));
      if(fwrite(null_string, 2, nfilename, dst) == 0) { free(null_string); return 0;} // invalidate the original entry
      free(null_string);
    }
  } else
    append = 1;
  if(append) {
    int32_t nentry = 0;
    fseek(dst, 8, SEEK_SET);
    fread(& nentry, 4, 1, dst);
    fseek(dst, 8, SEEK_SET);
    nentry ++;
    if(fwrite(& nentry, 4, 1, dst) == -1) return 0; // increase the number of entries by 1
    if(fseek(dst, 0, SEEK_END) == -1) return 0;
    nfilename = wcslen(name);
    if(fwrite(& nfilename, 4, 1, dst) == 0) return 0;
    short int* name_short = wchar_to_short(name, nfilename);
    if(fwrite(name_short, 2, nfilename, dst) == 0) { free(name_short); return 0;} // write the new filename to be created
    free(name_short);
    size = fixed_size + src -> nf0 * 4;
    if(fwrite(& size, 4, 1, dst) == 0) return 0; // write the size of the data trunk
  }
  nf0 = src -> nf0; fs = src -> fs; nhop = src -> nhop; modified = src -> modified;
  size_t write_pos = ftell(dst);

  // update version
  if(fseek(dst, 4, SEEK_SET) == -1) return -1;
  int32_t version = write_version;
  if(fwrite(& version, 4, 1, dst) == 0) { return -1;};

  fseek(dst, write_pos, SEEK_SET); // switch back and prepare for writing
  if(fwrite(& nf0, 4, 1, dst) == 0) return 0;
  if(fwrite(& fs, 4, 1, dst) == 0) return 0;
  if(fwrite(& nhop, 4, 1, dst) == 0) return 0;
  float* f0_float = calloc(nf0, sizeof(float));
  for(int i = 0; i < nf0; i ++) f0_float[i] = src -> f0[i];
  if(fwrite(f0_float, 4, nf0, dst) == 0) { free(f0_float); return 0;};
  
  if(write_version > 1) {
    time_t timestamp_time_t;
    time(& timestamp_time_t); src -> timestamp = timestamp_time_t;
    if(fwrite(& src -> timestamp, 4, 1, dst) == 0) { free(f0_float); return 0;};
    if(fwrite(& modified, 4, 1, dst) == 0) { free(f0_float); return 0;};
  }
  free(f0_float);
  return 1;
}

int mrq_delete_entry(FILE* dst, const wchar_t* name) {
  if(mrq_seek_entry(dst, name) == -1) return -1;
  int32_t nfilename = 0;
  if(fread(& nfilename, 4, 1, dst) == 0) return 0;
  fseek(dst, 0, SEEK_CUR); // prepare for writing
  short int* dummy = calloc(nfilename, sizeof(short int));
  if(fwrite(dummy, 2, nfilename, dst) == 0) { free(dummy); return 0;};
  free(dummy);
  return 1;
}

int mrq_enumerate(FILE* src, mrq_fenum enumproc, void* param) {
  int nentry = 0;
  if(fseek(src, 8, SEEK_SET) == -1) return 0;
  if(fread(& nentry, 4, 1, src) == 0) return 0;

  for(int i = 0; i < nentry; i ++) {
    int32_t pos = 0, nfilename = 0, size = 0;
    wchar_t* wfilename = NULL;
    mrq_entry* entry = NULL;
    pos = ftell(src);

    if(fread(& nfilename, 4, 1, src) == 0) return 0;
    short int* filename = calloc(nfilename, sizeof(short int));
    if(fread(filename, 2, nfilename, src) == 0) { free(filename); return 0; }
    wfilename = short_to_wchar(filename, nfilename);
    free(filename);
    if(fread(& size, 4, 1, src) == 0) { free(wfilename); return 0; }
    fseek(src, pos, SEEK_SET);
    entry = mrq_get_entry_without_seeking(src);
    if(enumproc(wfilename, entry, param) != 1) { delete_mrq_entry(entry); free(wfilename); return 0; }
    delete_mrq_entry(entry);
    free(wfilename);
    fseek(src, pos + 8 + nfilename * 2 + size, SEEK_SET);
  }
  return 1;
}

int mrq_defragment(const wchar_t* path) {
  FILE* dst = mrq_open(path, L"rw");
  if(dst == NULL) return 0;
  
  if(mrq_get_version(dst) > MRQ_VERSION) { fclose(dst); return -1;}

  int nentry = 0;
  int nfragment = 0;
  if(fseek(dst, 8, SEEK_SET) == -1) { fclose(dst); return 0;}
  if(fread(& nentry, 4, 1, dst) == 0) { fclose(dst); return 0;}
  wchar_t** names = calloc(nentry, sizeof(wchar_t*));
  mrq_entry** entries = calloc(nentry, sizeof(mrq_entry*));
  
  #define free_and_return(n, retval) { \
      for(int j = n; j >= 0; j --) { \
        free(names[j]); \
        delete_mrq_entry(entries[j]); \
      } \
      free(names); free(entries); \
      if(dst != NULL) fclose(dst); \
      return retval; \
    }
  
  for(int i = 0; i < nentry; i ++) {
    int32_t nfilename = 0, size = 0;
    int pos = ftell(dst);
    if(fread(& nfilename, 4, 1, dst) == 0) free_and_return(i - 1, 0);
    short int* name_short = calloc(nfilename, sizeof(short int));
    if(fread(name_short, 2, nfilename, dst) == 0) { free(name_short); free_and_return(i - 1, 0);}
    names[i] = short_to_wchar(name_short, nfilename);
    free(name_short);
    if(fread(& size, 4, 1, dst) == 0) { free(names[i]); free_and_return(i - 1, 0);}
    if(names[i][0] == 0) { // fragment detected
      free(names[i]); entries[i] = NULL; names[i] = NULL;
      nfragment ++;
    } else {
      fseek(dst, pos, SEEK_SET);
      entries[i] = mrq_get_entry_without_seeking(dst);
      if(entries[i] == NULL) { free(names[i]); free_and_return(i - 1, 0);}
    }
    if(fseek(dst, pos + 4 + nfilename * 2 + 4 + size, SEEK_SET) == -1) free_and_return(i, 0);
  }
  fclose(dst);
  dst = generic_wfopen(path, L"w"); // wipe out the file
  if(dst == NULL) free_and_return(nentry - 1, 0);
  fclose(dst);
  dst = mrq_open(path, L"rw");
  if(dst == NULL) free_and_return(nentry - 1, 0);
  
  for(int i = 0; i < nentry; i ++) {
    if(names[i] == NULL) continue;
    if(! mrq_write_entry(dst, names[i], entries[i]))
      free_and_return(nentry - 1, 0);
    free(names[i]); names[i] = NULL;
    delete_mrq_entry(entries[i]); entries[i] = NULL;
  }
  fclose(dst); dst = NULL;
  
  free_and_return(nentry - 1, 1);
}

