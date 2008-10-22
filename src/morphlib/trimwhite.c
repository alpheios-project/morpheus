#include <gkstring.h>

#include "trimwhite.proto.h"

void	trimwhite(char *s)
{
	char*	ptr = s + strlen(s);
	while ((ptr > s) && isspace(*--ptr))
		*ptr = '\0';
}

