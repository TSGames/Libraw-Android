
int fgetpos(FILE* __fp, fpos_t* __pos) __RENAME(fgetpos);
int fsetpos(FILE* __fp, const fpos_t* __pos) __RENAME(fsetpos);
int fseeko(FILE* __fp, off_t __offset, int __whence) __RENAME(fseek);
off_t ftello(FILE* __fp) __RENAME(ftell);