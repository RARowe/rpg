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

size_t entities_text_interaction_get(SceneData* s, void* entity, char* buffer) {
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
