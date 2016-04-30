#ifndef HTML_H
#define HTML_H

#include "player.h"

struct tab {
	char *name, *href;
};
extern const struct tab DM_TAB, ABOUT_TAB;
extern struct tab CUSTOM_TAB;

void html_header(const struct tab *active, char *title, char *search);
void html_footer(void);

const char *name_to_html(const char *name);

void html_print_player(struct player *player, int show_clan_link);


#endif /* HTML_H */
