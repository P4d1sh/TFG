#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
// #include "papi.h"
#include "my_papi.h"

// /* Number of the events to measure */
// int num_events;
// /* Must be initialized to PAPI_NULL before calling PAPI_create_event */
// int EventSet = PAPI_NULL;
// /* This is where we store the values we read from the eventset */
// long long *values;
/* We use retval to keep track of the number of the return value */
int retval;

// -----------------------------------------------------------------------
// Low_level
// -----------------------------------------------------------------------
int my_PAPI_add_event(int EventSet, int Event)
{
    if ((retval = PAPI_add_event(EventSet, Event)) != PAPI_OK)
        ERROR_RETURN(retval);
    return retval;
}

int my_PAPI_add_named_event(int EventSet, const char *EventName)
{
    // printf("\tPAPI: '%s'\n", EventName);
    if ((retval = PAPI_add_named_event(EventSet, EventName)) != PAPI_OK)
    {
        fprintf(stderr, "[Error?] Could not add event: '%s'\n", EventName);
        ERROR_RETURN(retval);
    }
    return retval;
}

int my_PAPI_assign_eventset_component(int EventSet, int cidx)
{
    if ((retval = PAPI_assign_eventset_component(EventSet, cidx)) != PAPI_OK)
        ERROR_RETURN(retval);
    return retval;
}

int my_PAPI_create_eventset(int *EventSet)
{
    if ((retval = PAPI_create_eventset(EventSet)) != PAPI_OK)
        ERROR_RETURN(retval);
    return retval;
}

const PAPI_hw_info_t *my_PAPI_get_hardware_info(void)
{
    const PAPI_hw_info_t *hwinfo;
    if ((hwinfo = PAPI_get_hardware_info()) == NULL)
    {
        fprintf(stderr, "[MyPapi] Error getting the HW info. %s:line %d: \n",
                __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }
    return hwinfo;
}

int my_PAPI_get_opt(int option, PAPI_option_t *ptr)
{
    return PAPI_get_opt(option, ptr);
}

int my_PAPI_is_initialized(void)
{
    return PAPI_is_initialized();
}

int my_PAPI_library_init(int version)
{
    if ((retval = my_PAPI_is_initialized()) == PAPI_NOT_INITED)
    {
        if ((retval = PAPI_library_init(version)) != PAPI_VER_CURRENT)
        {
            PAPI_perror("[MyPapi] Error initializing the PAPI library\n");
            ERROR_RETURN(retval);
        }
    }
    return retval;
}

int my_PAPI_list_events(int EventSet, int *Events, int *number)
{
    if ((retval = PAPI_list_events(EventSet, NULL, number)) != PAPI_OK)
        ERROR_RETURN(retval);
    return retval;
}

int my_PAPI_register_thread(void)
{
    if ((retval = PAPI_register_thread()) != PAPI_OK)
        ERROR_RETURN(retval);
    return retval;
}

int my_PAPI_unregister_thread(void)
{
    if ((retval = PAPI_unregister_thread()) != PAPI_OK)
        ERROR_RETURN(retval);
    return retval;
}

int my_PAPI_set_opt(int option, PAPI_option_t *ptr)
{
    if ((retval = PAPI_set_opt(option, ptr)) != PAPI_OK)
        ERROR_RETURN(retval);
    return retval;
}

void my_PAPI_shutdown(void)
{
    PAPI_shutdown();
}

int my_PAPI_start(int EventSet)
{
    // printf("[My_Papi] Starting event set = %d\n", EventSet);
    if ((retval = PAPI_start(EventSet)) != PAPI_OK)
        ERROR_RETURN(retval);
    return retval;
}

int my_PAPI_stop(int EventSet, long long *values)
{
    // printf("[My_Papi] Stoping event set = %d\n", EventSet);
    if ((retval = PAPI_stop(EventSet, values)) != PAPI_OK)
        ERROR_RETURN(retval);
    return retval;
}

int my_PAPI_thread_init(unsigned long (*id_fn)(void))
{
    if ((retval = PAPI_thread_init(id_fn)) != PAPI_OK)
        ERROR_RETURN(retval);
    return retval;
}

// -----------------------------------------------------------------------
// High_level
// -----------------------------------------------------------------------
int my_PAPI_hl_region_begin(const char *region)
{
    if ((retval = PAPI_hl_region_begin(region)) != PAPI_OK)
        ERROR_RETURN(retval);
    return retval;
}

int my_PAPI_hl_read(const char *region)
{
    if ((retval = PAPI_hl_read(region)) != PAPI_OK)
        ERROR_RETURN(retval);
    return retval;
}

int my_PAPI_hl_region_end(const char *region)
{
    if ((retval = PAPI_hl_region_end(region)) != PAPI_OK)
        ERROR_RETURN(retval);
    return retval;
}

// -----------------------------------------------------------------------
// Propios
// -----------------------------------------------------------------------
int my_attach_cpus(int num_cpus, const int cpus[], int *eventSets)
{
    size_t i;
    int aux;
    PAPI_option_t opts;
    // Se crea la libreria
    my_PAPI_library_init(PAPI_VER_CURRENT);
    if (num_cpus < 1)
    {
        fprintf(stderr, "[ERROR] Need at least 1 CPU\n");
        exit(EXIT_FAILURE);
    }
    aux = my_get_total_cpus(); // Gets the total num of cpus
    if (num_cpus > aux)
    {
        num_cpus = aux;
    }

    int cidx = 0;
    for (i = 0; i < num_cpus; i++)
    {
        eventSets[i] = PAPI_NULL;
        my_PAPI_create_eventset(&eventSets[i]);
        my_PAPI_assign_eventset_component(eventSets[i], cidx);

        /* Force granularity to PAPI_GRN_SYS */
        opts.granularity.eventset = eventSets[i];
        opts.granularity.granularity = PAPI_GRN_SYS;
        my_PAPI_set_opt(PAPI_GRANUL, &opts);

        // attach event set to cpu i
        opts.cpu.eventset = eventSets[i];
        // if cpus == NULL then, order by num
        if (cpus == NULL)
        {
            // The first "num_cpus" cpus to be attached
            opts.cpu.cpu_num = i;
        }
        else
        {
            opts.cpu.cpu_num = cpus[i];
        }
        my_PAPI_set_opt(PAPI_CPU_ATTACH, &opts);
    }
    return EXIT_SUCCESS;
}

void my_free(void *ptr)
{
    if (!ptr)
    {
        fprintf(stderr, "[MyPapi] Error, invalid pointer.\n");
        exit(EXIT_FAILURE);
    }
    free(ptr);
}

int my_get_total_cpus()
{
    const PAPI_hw_info_t *hwinfo;
    // Loads the library
    my_PAPI_library_init(PAPI_VER_CURRENT);
    // Load info of the HW and get the local num of cpus
    hwinfo = my_PAPI_get_hardware_info();
    return hwinfo->totalcpus;
}

void *my_malloc(size_t size)
{
    void *ptr;
    ptr = malloc(size);
    if (ptr == NULL)
    {
        fprintf(stderr, "[MyPapi] Error, couldn't allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

int my_start_events(const char *events[], int num_events, int *eventSets,
                    int num_eventSets)
{
    size_t i, j;
    for (i = 0; i < num_eventSets; i++)
    {
        // Se anhaden los eventos
        for (j = 0; j < num_events; j++)
        {
            my_PAPI_add_named_event(eventSets[i], events[j]);
        }
    }
    // Empieza las medidas
    for (i = 0; i < num_eventSets; i++)
    {
        my_PAPI_start(eventSets[i]);
    }
    return EXIT_SUCCESS;
}

int my_stop_events(int num_events, int *eventSets, int num_eventSets,
                   long long **values)
{
    if (num_eventSets < 1)
    {
        return EXIT_FAILURE;
    }
    if (num_eventSets == 1)
    {
        return my_PAPI_stop(*eventSets, *values);
    }
    else
    {
        int i, j;
        for (i = 0; i < num_eventSets; i++)
        {
            my_PAPI_stop(eventSets[i], values[i]);
        }
        // ! esto es un print - deberia ir aparte
        for (i = 0; i < num_eventSets; i++)
        {
            for (j = 0; j < num_events; j++)
            {
                printf("[CPU = %d] Event[%d] = %lld\n", i, j, values[i][j]);
            }
        }
    }
    return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------
int my_attach_all_cpus_and_start(const char *events[], int numEvents,
                                 int *eventSets, int num_cpus)
{
    size_t i, j;
    PAPI_option_t opts;

    // Se carga la liberia
    my_PAPI_library_init(PAPI_VER_CURRENT);

    int cidx = 0;
    for (i = 0; i < num_cpus; i++)
    {
        eventSets[i] = PAPI_NULL;
        my_PAPI_create_eventset(&eventSets[i]);
        my_PAPI_assign_eventset_component(eventSets[i], cidx);

        /* Force granularity to PAPI_GRN_SYS */
        opts.granularity.eventset = eventSets[i];
        opts.granularity.granularity = PAPI_GRN_SYS;
        my_PAPI_set_opt(PAPI_GRANUL, &opts);

        // attach event set to cpu i
        opts.cpu.eventset = eventSets[i];
        opts.cpu.cpu_num = i;
        my_PAPI_set_opt(PAPI_CPU_ATTACH, &opts);

        // Se anhaden los eventos
        for (j = 0; j < numEvents; j++)
        {
            my_PAPI_add_named_event(eventSets[i], events[j]);
        }
    }

    // Empieza las medidas
    for (i = 0; i < num_cpus; i++)
    {
        my_PAPI_start(eventSets[i]);
    }
    return EXIT_SUCCESS;
}

int my_attach_all_cpus_and_stop(int numEvents, int *eventSets,
                                long long **values, int num_cpus)
{
    size_t i;
    for (i = 0; i < num_cpus; i++)
    {
        // printf("[M_P] Eventset[%zu] = %d\n", i, eventSets[i]);
        my_PAPI_stop(eventSets[i], values[i]);
    }
    return EXIT_SUCCESS;
}

void my_stop_events_attached_cpus(int eventSets[], int num_eventSets,
                                  long long *values[], int numEvents)
{
    int i, j;
    // values = (long long *)malloc(sizeof(long long) * num_eventSets * numEvents);

    for (i = 0; i < num_eventSets; i++)
    {
        my_PAPI_stop(eventSets[i], values[i]);
    }

    for (i = 0; i < num_eventSets; i++)
    {
        for (j = 0; j < numEvents; j++)
        {
            printf("[CPU = %d] Event[%d] = %lld\n", i, j, values[i][j]);
        }
    }
}

// ! Se usan estas funciones para attach cpu --> !
int *my_attach_and_start(int num_cpus, const int cpus[],
                         const char *events[], int numEvents)
{
    size_t i, j;
    int *eventSet;
    PAPI_option_t opts;
    // Se crea la libreria
    my_PAPI_library_init(PAPI_VER_CURRENT);
    if (num_cpus < 2)
    {
        fprintf(stderr, "[ERROR] Need at least 1 CPU\n");
        exit(EXIT_FAILURE);
    }
    if (num_cpus > MAX_CPUS)
    {
        num_cpus = MAX_CPUS;
    }
    eventSet = (int *)my_malloc(sizeof(int) * num_cpus);

    for (i = 0; i < num_cpus; i++)
    {
        // Se crea el conjunto de eventos
        eventSet[i] = PAPI_NULL;
        my_PAPI_create_eventset(&eventSet[i]);
        /* Force event set to be associated with component 0 */
        /* (perf_events component provides all core events)  */
        my_PAPI_assign_eventset_component(eventSet[i], 0);

        /* Force granularity to PAPI_GRN_SYS */
        opts.granularity.eventset = eventSet[i];
        opts.granularity.granularity = PAPI_GRN_SYS; // PAPI_GRN_THR;
        my_PAPI_set_opt(PAPI_GRANUL, &opts);

        /* Attach this event set to cpu i */
        opts.cpu.eventset = eventSet[i];
        opts.cpu.cpu_num = cpus[i];
        my_PAPI_set_opt(PAPI_CPU_ATTACH, &opts);
        // Se anhaden los eventos
        for (j = 0; j < numEvents; j++)
        {
            my_PAPI_add_named_event(eventSet[i], events[j]);
        }
    }

    // Empieza las medidas
    for (i = 0; i < num_cpus; i++)
    {
        my_PAPI_start(eventSet[i]);
    }

    return eventSet;
}

long long **my_attach_and_stop(int num_cpus, int *eventSets, int numEvents)
{
    size_t i;
    long long **values;

    // 1er malloc
    values = (long long **)my_malloc(num_cpus * sizeof(long long *));
    // 2o malloc
    for (i = 0; i < num_cpus; i++)
    {
        values[i] = (long long *)my_malloc(numEvents * sizeof(long long));
    }

    for (i = 0; i < num_cpus; i++)
    {
        // printf("[M_P] Eventset[%zu] = %d\n", i, eventSets[i]);
        my_PAPI_stop(eventSets[i], values[i]);
    }

    return values;
}

void my_print_attached_values(int numEvents, const char *events[],
                              long long **values, int num_cpus,
                              const int cpus[])
{
    for (int i = 0; i < num_cpus; i++)
    {
        printf("[CPU = %d]\n", i);
        my_print_values(numEvents, events, values[i]);
    }
}
// ! <-- !

// int my_stop_events(int eventSet, int numEvents, long long *values)
// {
//     return my_PAPI_stop(eventSet, values);
// }

void my_print_values(int numEvents, const char *events[],
                     long long *values)
{
    int i, val;
    setlocale(LC_NUMERIC, "");
    printf("%s\n",
           "+-------------------------------------------+-----------------+");
    printf("| %-42s| %-16s|\n", "Event", "Value");
    printf("%s\n",
           "+===========================================+=================+");
    for (i = 0; i < numEvents; i++)
    {
        val = values[i];
        if (val != 0)
        {
            printf("| %-42s| %'-16lld|\n", events[i], values[i]);
        }
    }
    printf("%s\n",
           "+-------------------------------------------+-----------------+");
}

void my_print_values_perf(int numEvents, const char *events[],
                          long long *values)
{
    char separator = ':';
    for (int i = 0; i < numEvents; i++)
    {
        printf("%'lld%c%c%s\n", values[i], separator, separator, events[i]);
    }
}
