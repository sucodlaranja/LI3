#include <stdlib.h>
#include <stdio.h>

#include <string.h>

#define BLOCK_SIZE 1048576 // 1MBytes


/*
	para fzr benchmark (corre o programa 100 vezes) usar comando abaixo
		 perf stat -r 100 -d ./a.out
	
	para comparar processamento do strtok, sscanf e char by char

	Testes

		load_bus_sscanf
			Performance counter stats for './a.out' (100 runs):

            100,83 msec task-clock:u              #    0,995 CPUs utilized            ( +-  0,38% )
                 0      context-switches:u        #    0,000 K/sec                  
                 0      cpu-migrations:u          #    0,000 K/sec                  
                55      page-faults:u             #    0,546 K/sec                    ( +-  0,23% )
         334028354      cycles:u                  #    3,313 GHz                      ( +-  0,36% )  (61,32%)
         771930907      instructions:u            #    2,31  insn per cycle           ( +-  0,07% )  (74,85%)
         178219031      branches:u                # 1767,593 M/sec                    ( +-  0,07% )  (75,58%)
           1315498      branch-misses:u           #    0,74% of all branches          ( +-  0,26% )  (75,85%)
         196699133      L1-dcache-loads:u         # 1950,881 M/sec                    ( +-  0,06% )  (76,06%)
            298429      L1-dcache-load-misses:u   #    0,15% of all L1-dcache accesses  ( +-  0,55% )  (76,16%)
             10023      LLC-loads:u               #    0,099 M/sec                    ( +-  4,26% )  (48,35%)
              5529      LLC-load-misses:u         #   55,17% of all LL-cache accesses  ( +-  6,29% )  (47,99%)

          0,101383 +- 0,000394 seconds time elapsed  ( +-  0,39% )
		


		load_bus_table_strtok
			 Performance counter stats for './a.out' (100 runs):

             39,58 msec task-clock:u              #    0,991 CPUs utilized            ( +-  0,29% )
                 0      context-switches:u        #    0,000 K/sec                  
                 0      cpu-migrations:u          #    0,000 K/sec                  
                53      page-faults:u             #    0,001 M/sec                    ( +-  0,21% )
         123718006      cycles:u                  #    3,126 GHz                      ( +-  0,27% )  (54,48%)
         199915822      instructions:u            #    1,62  insn per cycle           ( +-  0,17% )  (69,63%)
          42245954      branches:u                # 1067,481 M/sec                    ( +-  0,17% )  (69,72%)
            906383      branch-misses:u           #    2,15% of all branches          ( +-  0,58% )  (72,76%)
          38681912      L1-dcache-loads:u         #  977,424 M/sec                    ( +-  0,12% )  (79,69%)
            276009      L1-dcache-load-misses:u   #    0,71% of all L1-dcache accesses  ( +-  0,09% )  (84,26%)
              4227      LLC-loads:u               #    0,107 M/sec                    ( +-  2,12% )  (50,59%)
              1358      LLC-load-misses:u         #   32,12% of all LL-cache accesses  ( +-  7,80% )  (42,97%)

          0,039953 +- 0,000118 seconds time elapsed  ( +-  0,29% )



		load_bus_table_bychar
			 Performance counter stats for './a.out' (100 runs):

             41,53 msec task-clock:u              #    0,989 CPUs utilized            ( +-  0,12% )
                 0      context-switches:u        #    0,000 K/sec                  
                 0      cpu-migrations:u          #    0,000 K/sec                  
                52      page-faults:u             #    0,001 M/sec                    ( +-  0,18% )
         132260614      cycles:u                  #    3,184 GHz                      ( +-  0,15% )  (56,68%)
         241046529      instructions:u            #    1,82  insn per cycle           ( +-  0,05% )  (71,09%)
          17216146      branches:u                #  414,512 M/sec                    ( +-  0,04% )  (71,08%)
              2845      branch-misses:u           #    0,02% of all branches          ( +-  0,56% )  (71,36%)
         120875455      L1-dcache-loads:u         # 2910,313 M/sec                    ( +-  0,05% )  (75,71%)
            275028      L1-dcache-load-misses:u   #    0,23% of all L1-dcache accesses  ( +-  0,04% )  (82,66%)
              1950      LLC-loads:u               #    0,047 M/sec                    ( +-  1,90% )  (53,21%)
               391      LLC-load-misses:u         #   20,04% of all LL-cache accesses  ( +-  3,80% )  (45,98%)

         0,0420031 +- 0,0000498 seconds time elapsed  ( +-  0,12% )


*/


void load_bus_table_strtok(/*SGR sgr, */FILE *f){
    // Check if file is valid.
    if(!f)
    {
        perror("bus_file_error: ");
        return;
    }

    char *buffer = malloc(sizeof(char) * BLOCK_SIZE);
    size_t bytes_read = 0;
    while((bytes_read = fread(buffer, sizeof(char), BLOCK_SIZE, f)) > 0){
		// Ignore the last (incomplete) line, if needed; file ends with \n   	
    	int i;
    	for(i = bytes_read / sizeof(char); buffer[i] != '\n'; --i);
    	buffer[i] = '\0';
    	// rearrange the file pointer to read this last line in the next block
    	fseek(f, - (bytes_read - i * sizeof(char))+1, SEEK_CUR);

    	char *rest = NULL;
    	char *line = strtok_r(buffer, "\n", &rest);
    	for (;line != NULL;line = strtok_r(NULL, "\n", &rest)){
    		// process line -> id;name;city;state
    		char *restL = NULL;
    		char *id = strtok_r(line, ";", &restL);
    		char *name = strtok_r(NULL, ";", &restL);
    		char *city = strtok_r(NULL, ";", &restL);
    		char *state = strtok_r(NULL, ";", &restL);
    		
    		//process categories -> c1, c2, c3...\n
    		char *category = strtok_r(NULL, ",", &restL);
    		for(; category != NULL; category = strtok_r(NULL, ",", &restL)){
    			// category
    		}

    	}
    }
    
    free(buffer);
    fclose(f);
}



void load_bus_table_bychar(/*SGR sgr,*/ FILE *f){
	// Check if file is valid.
    if(!f)
    {
        perror("bus_file_error: ");
        return;
    }
	
    char *buffer = malloc(sizeof(char) * BLOCK_SIZE);
    size_t bytes_read = 0;
    while((bytes_read = fread(buffer, sizeof(char), BLOCK_SIZE, f)) > 0){
		// Ignore the last (incomplete) line, if needed; file ends with \n   	
    	int i;
    	for(i = bytes_read / sizeof(char); buffer[i] != '\n'; --i);
    	buffer[i] = '\0';
    	// rearrange the file pointer to read this last line in the next block
    	fseek(f, - (bytes_read - i * sizeof(char))+1, SEEK_CUR);

    	// ler até encontrar '\0' (fim do buffer)
    	for(i = 0; buffer[i] != '\0';i++){
    		char c = buffer[i];
    	}

    }


    free(buffer);
	fclose(f);	
}



void load_bus_sscanf(/*SGR sgr, */FILE *f){
    // Check if file is valid.
    if(!f)
    {
        perror("bus_file_error: ");
        return;
    }

    char *buffer = malloc(sizeof(char) * BLOCK_SIZE);
    size_t bytes_read = 0;
    while((bytes_read = fread(buffer, sizeof(char), BLOCK_SIZE, f)) > 0){
		// Ignore the last (incomplete) line, if needed; file ends with \n   	
    	int i;
    	for(i = bytes_read / sizeof(char); buffer[i] != '\n'; --i);
    	buffer[i] = '\0';
    	// rearrange the file pointer to read this last line in the next block
    	fseek(f, - (bytes_read - i * sizeof(char))+1, SEEK_CUR);

    	char *line = strtok(buffer, "\n");
    	for (;line != NULL;line = strtok(NULL, "\n")){
    		// process line -> id;name;city;state

    		/*char *id = calloc(64, sizeof(char));
    		char *name = calloc(64, sizeof(char));
    		char *city = calloc(64, sizeof(char));
    		char *state = calloc(64, sizeof(char));
			char *categories = calloc(512, sizeof(char));*/

			
			char id[128];
			char name[128];
			char city[128];
			char state[128];
			char categories[512];
			

			int r = sscanf(line, "%[^;];%[^;];%[^;];%[^;];%s", id, name, city, state, categories);
			//printf("%s;%s;%s;%s;", id, name, city, state);

			if(r > 4){// lemos o campo das categorias

    			//process categories -> c1, c2, c3...\n
				char *rest;
    			char *category = strtok_r(categories, ",", &rest);
				//if(category != NULL) printf("%s", category);
				//category = strtok_r(NULL, ",", &rest);
    			for(; category != NULL; category = strtok_r(NULL, ",", &rest)){
    				//printf(",%s", category);
    			}
			}
			//printf("\n");

			/*free(id);
			free(name);
			free(city);
			free(state);
			free(categories);*/
    	}
    }
    
    free(buffer);
    fclose(f);
}



int main()
{
	char filepath[] = "input_files/business_full.csv";

	//load_bus_table_strtok(/*sgr,*/ fopen(filepath, "r"));
	load_bus_table_bychar(/*sgr,*/ fopen(filepath, "r"));
	//load_bus_sscanf(/*sgr,*/fopen(filepath, "r"));

	return 0;
}