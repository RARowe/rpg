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
    BUCKET_HEAD,
	TRASH,
    LONELY_TOWN_SIGN,
    OBJECT_TILE,
    NEWSPAPER_RACK,
    CD,
    WARP_POINT
};

enum class SpriteSheetTexture
{
    BOTH_NEWSPAPER_RACKS = 289,
    TRASH = 498,
    WOODEN_DOOR_ROUNDED_WINDOW_CLOSED = 982
};

enum class ItemSheetTexture
{
    CD
};

enum class EventType
{
    INTERACT,
    CHANGE_SCENE
};

enum class ScriptType
{
    ITS_JUST_TRASH
};

enum class TextBoxType
{
    SIMPLE,
    DIALOGUE
};

enum class PlayerStateType
{
    NORMAL,
    ITEM_FOUND
};

enum class BucketHeadStateType
{
    NORMAL,
    STANDING
};

enum class InputState
{
    NORMAL,
    TEXT_BOX,
    PAUSE_MENU,
    SCRIPT_RUNNING
};

enum class Scenes
{
    LONELY_TOWN_OUTSKIRTS,
    LONELY_TOWN_OUTSKIRTS_BUILDING,
    LONELY_TOWN_ENTRANCE
};

enum class TileSets
{
    OUTDOOR,
    INDOOR
};

enum class GameEvent
{
    FOUND_ENCRYPTED_COMPACT_DISK
};
#endif
