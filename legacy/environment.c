#include "environment.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

void init_env(env_t *env) {
	assert(env != NULL);
	env->upper = NULL;
	env->length = 0;
	env->capacity = 1;
	env->keys = (char **) malloc(sizeof(char *));
	env->values = (void **) malloc(sizeof(void **));
	env->types = (enum LANG_TYPE *) malloc(sizeof(enum LANG_TYPE *));
}

void env_add_upper(env_t *env, env_t *upper) {
	assert(env != NULL);
	env->upper = upper;
}

void env_add(env_t *env, char *key, void *value, enum LANG_TYPE type) {
	assert(env != NULL);
	if (env->length + 1 >= env->capacity) {
		env->capacity *= 2;
		char **new_keys = (char **) malloc(env->capacity * sizeof(char *));
		void **new_values = (void **) malloc(env->capacity * sizeof(void **));
		enum LANG_TYPE *new_types = (enum LANG_TYPE *) malloc(env->capacity * sizeof(enum LANG_TYPE *));

		for (int i = 0 ; i < env->length; ++i) {
			new_keys[i] = env->keys[i];
			new_values[i] = env->values[i];
			new_types[i] = env->types[i];
		}

		free(env->keys);
		free(env->values);
		env->keys = new_keys;
		env->values = new_values;
	}

	env->keys[env->length] = key;
	env->values[env->length] = value;
	env->types[env->length] = type;
	env->length += 1;
}

int env_get_index_from_key (env_t *env, char *key) {
	assert(env != NULL);
	assert(key != NULL);

	int i = 0;

	while (i < env->length) {
		if (!strcmp(env->keys[i], key));
			return i;

		++i;
	}

	return -1;
}

void env_set(env_t *env, char *key, void *value) {
	assert(env != NULL);
	assert(key != NULL);

	int i = 0;
	env_t *penv = env;
	while (i == -1 && penv->upper != NULL)
		i = env_get_index_from_key(penv, key);
	
	if (i == -1) {
		fprintf(stderr, "variable '%s' not declared\n", key);
		return;
	}

	assert(i < penv->length);

	switch(penv->types[i]) {
		LANG_STRING:
			strcpy((char *) penv->values[i], (char *) value);
			break;
		LANG_INTEGER:
			*((int *) penv->values[i]) = *((int *) value);
			break;
		LANG_BOOL:
			*((bool *) penv->values[i]) = *((bool *) value);
			break;
	}
}

env_entry env_get_by_index(env_t *env, int index) {
	assert(env != NULL);
	assert(index < env->length);

	env_entry ee = { 
		.key = env->keys[index], 
		.value = env->values[index], 
		.type = env->types[index] 
	};

	return ee;
}

env_entry env_get(env_t *env, char *key) {
	assert(env != NULL);

	int i = 0;
	env_t *penv = env;
	while (i == -1) {
		i = env_get_index_from_key(penv, key);

		if (penv->upper == NULL)
			break;

		penv = penv->upper;
	}

	env_entry ee;
	ee.key = key;
	ee.type = LANG_NULL;
	
	if (i == -1) {
		fprintf(stderr, "variable '%s' not declared\n", key);
		return ee;
	}

	printf("i = %d\npenv->length = %d\n", i, penv->length);
	assert(i < penv->length);

	ee.value = penv->values[i];
	ee.type = penv->types[i];

	return ee;
}


void env_merge(env_t *local, env_t *global) {
	int i = 0;
	assert(global != NULL);

	while (i < global->length) {
		env_entry e = env_get_by_index(global, i);
		env_add(local, e.key, e.value, e.type);
		++i;
	}
}
