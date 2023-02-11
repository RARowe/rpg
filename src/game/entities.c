#include "game.h"

static Body* entities_get_body(SceneData* s, int entityId) {
    return &s->bodies[entityId];
}

static Body* entities_get_body_by_point(SceneData* s, float x, float y) {
    for (int i = 0; i < s->bodies.size(); i++) {
        Body* b = &s->bodies[i];
        if (point_in_body(b, x, y)) {
            return b;
        }
    }
    return NULL;
}

static void entities_wall_add(SceneData* s, float x, float y, short w, short h) {
    int entityId = s->nextEntityId++;
    Body b = { x, y, w, h };
    s->solidEntities.insert(entityId);
    s->bodies[entityId] = b;
}

static void entities_wall_remove(SceneData* s, void* entity) {
    for (int i = 0; i < s->bodies.size(); i++) {
        Body* b = &s->bodies[i];

        if (b == (Body*)entity) {
            s->bodies.erase(i);
            s->solidEntities.erase(i);
            break;
        }
    }
}

static void entities_item_add
(SceneData* s, void* entity, int tile, const char* description) {
    Item item;
    item.tile = tile;
    strcpy(item.description, description);
    for (int i = 0; i < s->bodies.size(); i++) {
        Body* b = &s->bodies[i];

        if (b == (Body*)entity) {
            s->items[i] = item;
            break;
        }
    }
}

static int entities_item_get(SceneData* s, void* entity, Item* item) {
    for (int i = 0; i < s->bodies.size(); i++) {
        Body* b = &s->bodies[i];

        if (b == (Body*)entity) {
            item->tile = s->items[i].tile;
            strcpy(item->description, s->items[i].description);
            return 1;
        }
    }
    return 0;
}

static void entities_text_interaction_add(SceneData* s, void* entity, const char* text) {
    for (int i = 0; i < s->bodies.size(); i++) {
        Body* b = &s->bodies[i];

        if (b == (Body*)entity) {
            s->textInteractions[i] = text;
            break;
        }
    }
}

static size_t entities_text_interaction_get(SceneData* s, void* entity, char* buffer) {
    for (auto&& pair : s->bodies) {
        if (&pair.second == (Body*)entity) {
            if (s->textInteractions.count(pair.first) > 0) {
                strcpy(buffer, s->textInteractions[pair.first].c_str());
                return s->textInteractions[pair.first].length();
            } else {
                return 0;
            }
        }
    }

    return 0;
}

static void entities_spawn_point_set(SceneData* s, float x, float y) {
    s->spawnPoint.x = x;
    s->spawnPoint.y = y;
}

