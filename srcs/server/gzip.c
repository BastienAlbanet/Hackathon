

#include "server.h"

#define CHUNK	16384

int gzip(char *file_name) {
	char src_path[MAX_NAME_SIZE+(sizeof(TEMP_FOLDER)-1)+1] = TEMP_FOLDER;
	char dest_path[MAX_NAME_SIZE+(sizeof(DATAS_FOLDER)-1)+1] = DATAS_FOLDER;
	
	strcpy(src_path+(sizeof(TEMP_FOLDER)-1), file_name);
	strncpy(dest_path+(sizeof(DATAS_FOLDER)-1), file_name, strlen(file_name)-3);
	
	FILE *source = fopen(src_path, "rb");
	//gzFile source = gzopen(src_path, "rb");
	if (!source) {
		perror("can't open source file\n");
		return (0);
	}
	
	FILE *dest = fopen(dest_path, "w");
	if (!dest) {
		perror("can't open dest file\n");
		return (0);
	}
	printf("src path: %s\n", src_path);
	printf("dest path: %s\n", dest_path);
	int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];
	
	/* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit2(&strm, 16+MAX_WBITS);
    if (ret != Z_OK)
        return ret;
	
	/* decompress until deflate stream ends or end of file */
    do {
		printf("GO\n");
		strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;
		/* run inflate() on input until output buffer not full */
        do {
			strm.avail_out = CHUNK;
            strm.next_out = out;
			ret = inflate(&strm, Z_NO_FLUSH);
            //assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
			if (ret == Z_STREAM_ERROR) {
				return -1;
			}
            switch (ret) {
				case Z_NEED_DICT:
					ret = Z_DATA_ERROR;
					/* fallthrough */
				case Z_DATA_ERROR:
					/* fallthrough */
				case Z_MEM_ERROR:
					(void)inflateEnd(&strm);
					return ret;
				default:
					break;
            }
			have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }
		} while (strm.avail_out == 0);
		/* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);
	/* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}


