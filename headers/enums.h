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
	TRASH
};

enum class EventType
{
    INTERACT
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
#endif
