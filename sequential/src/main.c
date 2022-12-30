#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "array.h"
#include "args.h"
#include "measurement.h"
#include <sys/resource.h>
/*! @function
  @abstract      call sequential tarjan on graph from file or from shared memory
  @param  argc   number of parameter, considering the 1st is name of program
  @param  argv   in case of graph from file the second parameters is the path of grapf file, in case
                graph is from shared memory the second paramenters is ID of shared memory
 */

scc_set_t *SCCs;

void callback(array_int * scc){
    int scc_id;
    scc_id = array_int_get_min(scc);
    scc_set_add(SCCs,scc_id,scc);
}

int main(int argc, char* argv[]){
    main_parameters_t c;
    struct rlimit rl;
    getrlimit(RLIMIT_STACK, &rl);
    //printf("%d %d\n",rl.rlim_cur,rl.rlim_max);
    rl.rlim_cur=128000000;
    setrlimit(RLIMIT_STACK, &rl);
    //printf("%d %d\n",rl.rlim_cur,rl.rlim_max);
    c = get_input(argc, argv);
    graph_t *graph;

    int num;
    double time_tarjan = 0.0, time_init = 0.0,time_destroy=0.0;

    switch (c.t){
        case INPUT_ERROR:
            printf("incorrent parameters:\ninserire come primo parametro path del grafo ingresso\ninserire come secondo parametro path del file contenente gli SCC trovati\n");
            exit(1);
            break;
        case INPUT_TYPE_FILE:
            {
            STARTTIME(1);
            graph = graph_load_from_file(c.first_param);
            SCCs= scc_set_init();
            //printf("start graph\n");
            //graph_print_debug(graph);
            ENDTIME(1,time_init);

            STARTTIME(2);
            graph_tarjan_foreach(graph, callback);
            ENDTIME(2,time_tarjan);

            num = graph_get_num_vertex(graph);
            STARTTIME(3);
            scc_set_save_to_file(SCCs,c.second_param);
            graph_free(graph);
            scc_set_free(SCCs);
            ENDTIME(3,time_destroy);

            printf("%d,%f,%f,%f,",num,time_init,time_destroy,time_tarjan);
            break;
            }
    }

}

