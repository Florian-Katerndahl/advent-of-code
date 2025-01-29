#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

struct Map {
	unsigned long destination_start;
	unsigned long source_start;
	unsigned long sequence_length;
};

unsigned long *parse_seeds(FILE *f, size_t *n_seeds);
struct Map *parse_map(FILE *f, size_t *map_length);
unsigned long map_sequence(const struct Map *map, size_t map_length, unsigned long val);

/*
 * A single big map would be nicer, but it seems the effort to create one is a bit to high?
 */
int main(void) {
	FILE *fp = fopen("input-05.txt", "rt");
	if (fp == NULL) {
		fprintf(stderr, "ERROR: Could not open input file\n");
		exit(1);
	}
	size_t num_seeds, seed_to_soil_l, soil_to_fertilizer_l, fertilizer_to_water_l, water_to_light_l,
		   light_to_temperature_l, temperature_to_humidity_l, humidity_to_location_l;

	unsigned long *seeds = parse_seeds(fp, &num_seeds);

	struct Map *seed_to_soil = parse_map(fp, &seed_to_soil_l);
	struct Map *soil_to_fertilizer = parse_map(fp, &soil_to_fertilizer_l);
	struct Map *fertilizer_to_water = parse_map(fp, &fertilizer_to_water_l);
	struct Map *water_to_light = parse_map(fp, &water_to_light_l);
	struct Map *light_to_temperature = parse_map(fp, &light_to_temperature_l);
	struct Map *temperature_to_humidity = parse_map(fp, &temperature_to_humidity_l);
	struct Map *humidity_to_location = parse_map(fp, &humidity_to_location_l);

	unsigned long *locations = calloc(num_seeds, sizeof(unsigned long));

	unsigned long val;
	for (size_t i = 0; i < num_seeds; i++) {
		val = map_sequence(seed_to_soil, seed_to_soil_l, seeds[i]);
		val = map_sequence(soil_to_fertilizer, soil_to_fertilizer_l, val);
		val = map_sequence(fertilizer_to_water, fertilizer_to_water_l, val);
		val = map_sequence(water_to_light, water_to_light_l, val);
		val = map_sequence(light_to_temperature, light_to_temperature_l, val);
		val = map_sequence(temperature_to_humidity, temperature_to_humidity_l, val);
		locations[i] = map_sequence(humidity_to_location, humidity_to_location_l, val);
	}

	int temp;
	for (size_t i = 0; i < num_seeds; i++) {
		for (size_t j = 0; j < num_seeds - 1 - i; j++) {
			if (locations[j] < locations[j + 1]) {
				temp = locations[j + 1];
				locations[j + 1] = locations[j];
				locations[j] = temp;
			}
		}
		break;
	}
	
	printf("Solution 1: %ld\n", locations[num_seeds - 1]);
	
	free(seeds);
	free(locations);
	free(seed_to_soil);
	free(soil_to_fertilizer);
	free(fertilizer_to_water);
	free(water_to_light);
	free(light_to_temperature);
	free(temperature_to_humidity);
	free(humidity_to_location);
	fclose(fp);
	return 0;
}

unsigned long *parse_seeds(FILE *f, size_t *n_seeds) {
	size_t s_len = 1024, inserted = 0;
	unsigned long *s = calloc(s_len, sizeof(unsigned long));
	char line[1024];
	char *p = line;
	char *endptr = p;
	fgets(line, 1024, f);
	while (!isspace(*p)) p++;
	while (*endptr != '\n' && endptr) {
		if (inserted >= s_len - 1) {
			s_len *= 2;
			s = reallocarray(s, s_len, sizeof(unsigned long));
		}
		s[inserted] = (int) strtol(p, &endptr, 10);
		p = endptr;
		inserted++;
	}
	
	*n_seeds = inserted;
	// move past newline
	fgets(line, 1024, f);
	return s;
}

struct Map *parse_map(FILE *f, size_t *map_length) {
	size_t s_len = 1024, inserted = 0;
	struct Map *s = calloc(s_len, sizeof(struct Map));
	char line[1024];
	struct Map map;
	while (fgets(line, 1024, f)) { 
		char *p = line;
		char *endptr = p;
		if (*p == '\n') break;
	   	if (!isdigit(*p)) continue;
		map.destination_start = (int) strtol(p, &endptr, 10);
		map.source_start = (int) strtol(endptr, &p, 10);
		map.sequence_length = (int) strtol(p, &endptr, 10);

		if (*endptr == '\n' && *p == '\n' && map.sequence_length == 0) break;
		if (inserted >= s_len - 1) {
			s_len *= 2;
			s = reallocarray(s, s_len, sizeof(struct Map));
		}
		s[inserted] = map;
		inserted++;
	}
	
	*map_length = inserted;	
	return s;
}

unsigned long map_sequence(const struct Map *map, size_t map_length, unsigned long val) {
	for (size_t map_index = 0; map_index < map_length; map_index++) {
		if (val >= map[map_index].source_start
			&& val < map[map_index].source_start + map[map_index].sequence_length)
			return val - map[map_index].source_start + map[map_index].destination_start;
	}
	return val;
}

