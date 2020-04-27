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
    NEWSPAPER_KIOSK
};

enum class SpriteSheetTexture
{
    BOTH_NEWSPAPER_KIOSKS = 289,
    TRASH = 498
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

enum class BucketHeadStateType
{
    NORMAL,
    STANDING
};
#endif
