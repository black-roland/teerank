#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <libgen.h>
#include <string.h>

#include "config.h"
#include "html.h"
#include "player.h"
#include "clan.h"

static const struct player_array PLAYER_ARRAY_ZERO;

static int cmp_player(const void *p1, const void *p2)
{
	const struct player *a = p1, *b = p2;

	/* We want them in reverse order */
	if (b->rank > a->rank)
		return -1;
	if (b->rank < a->rank)
		return 1;
	return 0;
}

int main(int argc, char **argv)
{
	struct clan clan;
	unsigned missing_members, i;
	char clan_name[NAME_LENGTH];

	load_config();
	if (argc != 2) {
		fprintf(stderr, "usage: %s <clan_name>\n", argv[0]);
		return 500;
	}

	/* Load players */

	if (!is_valid_hexname(argv[1]))
		return 404;
	if (!read_clan(&clan, argv[1]))
		return 404;
	missing_members = load_members(&clan);

	/* Sort members */
	qsort(clan.members, clan.length, sizeof(*clan.members), cmp_player);

	/* Eventually, print them */
	hexname_to_name(clan.name, clan_name);
	html_header(&DM_TAB, clan_name, NULL);
	printf("<h2>%s</h2>\n", clan_name);

	if (!missing_members)
		printf("<p>%u member(s)</p>\n", clan.length);
	else
		printf("<p>%u member(s), %u could not be loaded</p>\n",
		       clan.length + missing_members, missing_members);

	printf("<table><thead><tr><th></th><th>Name</th><th>Clan</th><th>Score</th></tr></thead>\n<tbody>\n");

	for (i = 0; i < clan.length; i++)
		html_print_player(&clan.members[i], 0);

	printf("</tbody></table>\n");
	html_footer();

	return EXIT_SUCCESS;
}
