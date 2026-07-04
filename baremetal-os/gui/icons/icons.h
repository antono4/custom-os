/* =============================================================================
 * NanoOS GUI Icons Header
 * =============================================================================
 */

#ifndef ICONS_H
#define ICONS_H

#include <stdint.h>

#define ICON_SIZE 32

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t data[ICON_SIZE * ICON_SIZE];
} icon_t;

// Icon IDs
typedef enum {
    ICON_COMPUTER,
    ICON_FOLDER,
    ICON_FILE,
    ICON_TERMINAL,
    ICON_TEXTEDITOR,
    ICON_CALCULATOR,
    ICON_SETTINGS,
    ICON_TRASH,
    ICON_NETWORK,
    ICON_SOUND,
    ICON_DISPLAY,
    ICON_HELP,
    ICON_WARNING,
    ICON_ERROR,
    ICON_INFO,
    ICON_QUESTION
} icon_id_t;

// Icon data
extern icon_t icons[];
icon_t* icon_get(icon_id_t id);
void icon_draw(int x, int y, icon_id_t id);

#endif