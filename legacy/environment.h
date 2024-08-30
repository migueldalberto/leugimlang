#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "lang_type.h"

typedef struct env_s env_t;

struct env_s {
	env_t *upper;
	int length;
	int capacity;
	char **keys;
	void **values;
	enum LANG_TYPE *types;
};

typedef struct {
	char *key;
	void *value;
	enum LANG_TYPE type;
} env_entry;

void init_env(env_t *env);
void env_add_upper(env_t *env, env_t *upper);
void env_add(env_t *env, char *key, void *value, enum LANG_TYPE type);
void env_set(env_t *env, char *key, void *value);
env_entry env_get(env_t *env, char *key);
void env_merge(env_t *local, env_t *global);

#endif
