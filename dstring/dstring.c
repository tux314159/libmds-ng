#include <dstring.h>

/* INIT/DEINIT */
enum status string_init(struct string *in)
{
	NULLCHK(in);

	in->len = 0;
	in->str = calloc(1, sizeof(char));
	return OK;
}

enum status string_deinit(struct string *in)
{
	NULLCHK(in);

	free(in->str);
	return OK;
}

/* MISC */
enum status string_grow(struct string *in, const size_t add)
{
	char *t;

	NULLCHK(in);

	t = realloc(in->str, (in->len + add + 1) * sizeof(char));

	if (t == NULL)
		return ALLOC_FAIL;

	in->str = t;
	in->len += add;
	in->str[in->len] = '\0';

	return OK;
}

enum status string_append(struct string *in, const char *str)
{
	NULLCHK(in);
	NULLCHK(str);

	RETIFNOK(string_grow(in, strlen(str)));

	strcat(in->str, str);

	return OK;
}

enum status string_insert(struct string *in, const size_t idx, const char *str)
{
	size_t oldlen;
	size_t len;

	NULLCHK(in);
	NULLCHK(str);

	len = strlen(str);
	oldlen = in->len;

	RETIFNOK(string_grow(in, len));
	memmove(in->str + idx + len, in->str + idx,
			(oldlen - idx + 1 /* the NULL byte */) * sizeof(char));
	memcpy(in->str + idx, str, len * sizeof(char));

	return OK;
}

enum status string_delete(struct string *in, const size_t start, const size_t end)
{
	NULLCHK(in);
	if (end >= in->len)
		return OOB;

	memmove(in->str + start, in->str + end + 1, (end - start + 1) * sizeof(char));
	in->len -= (end - start + 1);
	in->str[in->len] = '\0';

	return OK;
}
