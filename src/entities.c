#include "types.h"

inline Body* entities_get_body(SceneData* s, int entityId) {
    return &s->bodies[entityId];
}

void entities_wall_add(SceneData* s, float x, float y, short w, short h) {
    int entityId = s->nextEntityId++;
    Body b = { x, y, w, h };
    s->solidEntities.insert(entityId);
    s->bodies[entityId] = b;
}

void entities_wall_remove(SceneData* s, void* entity) {
    for (int i = 0; i < s->bodies.size(); i++) {
        Body* b = &s->bodies[i];

        if (b == (Body*)entity) {
            s->bodies.erase(i);
            s->solidEntities.erase(i);
            break;
        }
    }
}

void entities_text_interaction_add(SceneData* s, void* entity, const char* text) {
    for (int i = 0; i < s->bodies.size(); i++) {
        Body* b = &s->bodies[i];

        if (b == (Body*)entity) {
            s->textInteractions[i] = text;
            break;
        }
    }
}
