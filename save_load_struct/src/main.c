#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct person_s {
	char *name;
	int age;
} *Person;

Person person_create(char *name, int age) {
	Person p = NULL;

	p = malloc(sizeof(struct person_s));
	if (!p)
		return NULL;

	p->age = age;
	p->name = strdup(name);
	if (!p->name) {
		free(p);
		return NULL;
	}
	
	return p;
}

void person_print(Person p) {
	if (!p)
		return;

	printf("name: %s, age: %d\n", p->name, p->age);
}

void person_free(Person p) {
	if (!p)
		return;

	free(p->name);
	free(p);
}

int person_save(Person p, char *filename) {
	FILE *fp = NULL;
	int state = 0;
	unsigned int name_size;
	unsigned int total_size = name_size + sizeof(int) + 1;

	if (!p)
		return state;

	fp = fopen(filename, "wb");
	if (!fp)
		return state;

	name_size = p->name ? strlen(p->name) : 0;
	if (!(state = fwrite(&name_size, sizeof(int), 1, fp)))
		goto end;
	if (name_size && !(state = fwrite(p->name, name_size, 1, fp)))
		goto end;
	state = fwrite(&(p->age), sizeof(int), 1, fp);

end:
	fclose(fp);
	return state;
}

Person person_load(char *filename) {
	FILE *fp = NULL;
	Person p = NULL;
	char *name = NULL;
	char name_length = 0;
	int age = 0;

	if (!filename)
		goto end;

	fp = fopen(filename, "rb");
	if (!fp)
		goto end;

	if (!fread(&name_length, sizeof(int), 1, fp))
		goto end;

	name = malloc(name_length + 1);
	if (!name || !fread(name, name_length, 1, fp))
		goto end;
	else
		name[name_length] = 0;

	if (!fread(&age, sizeof(int), 1, fp))
		goto end;

	p = person_create(name, age);

end:
	if (name)
		free(name);
	if (fp)
		fclose(fp);
	return p;
}

int main() {
	Person p = NULL;

	p = person_create("davenchy", 24);
	if (!p)
		return 1;

	printf("Memory: ");
	person_print(p);

	if (!person_save(p, "person_c.bin")) {
		person_free(p);
		return 2;
	}

	person_free(p);
	// p = person_load("person.bin");
	// if (!p)
	// 	return 3;
	//
	// printf("Loaded: ");
	// person_print(p);
	// person_free(p);

	return 0;
}
