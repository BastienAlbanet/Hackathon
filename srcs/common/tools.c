
#include "common.h"

bool_t name_is_targz(char *path) {
	char *ptr;
	
	ptr = strchr(path, '\0');
	if (!ptr || (unsigned long)(ptr-path) < (sizeof(VALID_FORMAT)-1)) {
		fprintf(stderr, "Invalid archive name\n");
		return (FALSE);
	}
	if (!strcmp(ptr-(sizeof(VALID_FORMAT)-1), VALID_FORMAT)) {
		return (TRUE);
	}
	fprintf(stderr, "Invalid archive name\n");
	return (FALSE);
}