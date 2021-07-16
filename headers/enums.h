#ifndef ENUMS_H
#define ENUMS_H
#define SCREEN_HEIGHT 416
#define SCREEN_WIDTH 608

enum class Direction
{
    LEFT,
    UP,
    RIGHT,
    DOWN
};

enum class EntityType
{
	PLAYER,
    INTERACTION,
    WARP_POINT,
    WARP_SPAWN_POINT
};

enum class SpriteSheetTexture
{
    TRASH = 498,
    WOODEN_DOOR_ROUNDED_WINDOW_CLOSED = 982
};

enum class ItemType
{
    ENCRYPTED_CD,
    BASS_STRINGS,
    CABINET_KEY
};

enum class ItemSheetTexture
{
    CD = 0,
    STRINGS = 1,
    KEY = 2
};

enum class PlayerStateType
{
    NORMAL,
    ITEM_FOUND
};

enum class GameState
{
    NORMAL,
    TEXTBOX,
    MENU,
    EDITOR
};

enum class Levels
{
    LONELY_TOWN
};

enum class Scenes
{
    LONELY_TOWN_OUTSKIRTS = 0,
    LONELY_TOWN_OUTSKIRTS_BUILDING = 1,
    LONELY_TOWN_ENTRANCE = 2,
    LONELY_TOWN_HOUSE = 3
};

// TODO(TILESET): Refactor this out
enum class TileSets
{
    OUTDOOR,
    INDOOR,
    ITEMS,
    MISC
};

enum class MiscSheetTexture
{
    CROSSHAIR = 0
};

enum class GameEvent
{
    FOUND_ENCRYPTED_COMPACT_DISK,
    FOUND_CABINET_KEY,
    LONELY_TOWN_OUTSKIRTS_BUILDING_ITEM_FOUND
};

enum class Color
{
    WHITE,
    BLUE,
    BLACK,
    RED
};
#endif
