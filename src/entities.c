static Entity* entities_find_next(SceneData* s) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
	    if (s->entities[i].id < 0) {
		    s->entities[i].id = s->nextEntityId;
		    s->nextEntityId += 1;
		    return &(s->entities[i]);
	    }
    }

    perror("NO ENTITIES AVAILABLE. INCREASE LIMIT");

    return NULL;
}

static Entity* entities_get(SceneData* s, unsigned int id) {
    for (int i = 0; i < MAX_ENTITIES; i++) {
	    if (s->entities[i].id == id) {
		    return &(s->entities[i]);
	    }
    }

    char errorMessage[512];
    sprintf(errorMessage, "COULD NOT FIND ENTITY WITH ID %d. GAME STATE CORRUPTED", id);

    perror(errorMessage);
    return NULL;
}

static Body* entities_get_body(SceneData* s, int entityId) {
	return &(entities_get(s, entityId)->body);
}

static Entity* entities_get_by_point(SceneData* s, float x, float y) {
	for (int i = 0; i < MAX_ENTITIES; i++) {
		if (s->entities[i].id < 0) {
			continue;
	    	}
		Body* b = &(s->entities[i].body);
		if (point_in_body(b, x, y)) {
			return &(s->entities[i]);
		}
    }
	return NULL;
}

static void entities_wall_add(SceneData* s, float x, float y, short w, short h) {
	Entity* e = entities_find_next(s);
	e->body = { x, y, w, h };
	e->isSolid = true;
}

static void entities_wall_remove(SceneData* s, unsigned int id) {
	Entity* e = entities_get(s, id);
	e->id = -1;
}


static void entities_item_add
(SceneData* s, unsigned int id, int tile, const char* description) {
	Entity* e = entities_get(s, id);
	e->type = ENTITY_TYPE_ITEM;
	e->sprite = tile;
	strcpy(e->text, description);
}

static Entity* entities_item_get(SceneData* s, unsigned int id) {
	Entity* e = entities_get(s, id);
	if (e->type != ENTITY_TYPE_ITEM) {
		return NULL;
	}

        return e;
}

static void entities_text_interaction_add(SceneData* s, unsigned int id, const char* text) {
	Entity* e = entities_get(s, id);
	e->type = EntityTYpeInteractable;
	strcpy(e->text, text);
}

static size_t
entities_text_interaction_get(SceneData* s, unsigned int id, char* buffer) {
	Entity* e = entities_get(s, id);
        strcpy(buffer, e->text);
	return 1;
}

static void entities_spawn_point_set(SceneData* s, float x, float y) {
    s->spawnPoint.x = x;
    s->spawnPoint.y = y;
}

