#include <stdint.h>
#include <gbdk/platform.h>
#include <gbdk/metasprites.h>

//these only exist to so that I can "move" these metasprites and they will still reference tiles 0-6 which I loaded in before
//this is a workaround for the fact that I can't use the same tile twice in a metasprite

const metasprite_t* const maeveSprite_layout[2] = {
    maeveSprite_idle,
    maeveSprite_gun
};

const metasprite_t maeveSprite_gun[] = {
	METASPR_ITEM(-12, -8, 0, S_PAL(0)),
	METASPR_ITEM(0, 8, 1, S_PAL(0)),
	METASPR_ITEM(8, -8, 2, S_PAL(0)),
	METASPR_ITEM(0, 8, 3, S_PAL(0)),
	METASPR_ITEM(0, 8, 4, S_PAL(0)),
	METASPR_ITEM(8, -16, 5, S_PAL(0)),
	METASPR_ITEM(0, 8, 6, S_PAL(0)),
	METASPR_TERM
};

const metasprite_t maeveSprite_idle[] = {
	METASPR_ITEM(-12, -8, 0, S_PAL(0)),
	METASPR_ITEM(0, 8, 1, S_PAL(0)),
	METASPR_ITEM(8, -8, 2, S_PAL(0)),
	METASPR_ITEM(0, 8, 3, S_PAL(0)),
	METASPR_ITEM(8, -8, 4, S_PAL(0)),
	METASPR_ITEM(0, 8, 5, S_PAL(0)),
	METASPR_TERM
};