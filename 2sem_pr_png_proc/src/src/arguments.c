#include "arguments.h"

int read_arguments(int* arg_c, char*** arg_v, argStruct* data){
    enum error_types err=ERR_NO;

    int argc=*arg_c;
    char** argv=*arg_v;

    data->help = 0; data->info = 0; data->in = 0; data->out = 0;

    data->mirror = 0; data->pentagram = 0; data->hexagon = 0; data->rect = 0;
    data->compress = 0;

    data->axis = -1; data->radius = -1; data->thick = -1;
    data->col = 0; data->fill = 0; data->fc = 0;
    data->lu = 0; data->rd = 0; data->cent = 0;
    data->num = 0;

    data->left_up = (point*)calloc(1, sizeof(point));
    data->right_down = (point*)calloc(1, sizeof(point));
    data->center = (point*)calloc(1, sizeof(point));
    data->color = (colorRGB*)calloc(1, sizeof(colorRGB));
    data->fill_color = (colorRGB*)calloc(1, sizeof(colorRGB));
    /*освобождение проиходит в main(...), после выполнения основных действий;
        если корректно выделилась память под структуру data, проверяется отдельно корректности выделения памяти под каждое из данных полей и его очистка,
            в случае ошибки выделения data, данная функция не будет вызвана*/
    if (!data->right_down || !data->left_up || !data->center || !data->color || !data->fill_color){
        fprintf(stderr, "Memory error\n");
        err = ERR_MEMORY;
    }

    int opt, arg_err=0, last=0;
    while (!err && (opt = getopt_long(argc, argv, "hIi:o:MPRHAn:a:u:d:c:r:t:C:fF:?", options, NULL)) != -1){
        switch (opt){
            case 'h':
                data->help = 1;
                if (optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "This option does not take arguments\n");
                    err = ERR_ARGUMENT;
                }
                else if (optind == argc)
                    last = 1;
                break;

            case 'I':
                data->info = 1;
                if (optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "This option does not take arguments\n");
                    err = ERR_ARGUMENT;
                }
                break;

            case 'i':
                data->in = 1;
                data->input = (char*)calloc(strlen(optarg), sizeof(char));
                /*освобождение проиходит в main(...), после выполнения основных действий;
                    если корректно выделилась память под структуру data, проверяется корректность выделения памяти под data->input и его очистка,
                        в случае ошибки выделения data, данная функция не будет вызвана*/
                if (!data->input){
                    fprintf(stderr, "Memory error\n");
                    err = ERR_MEMORY;
                }
                if (!err)
                    strncpy(data->input, optarg, strlen(optarg));

                if (optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "Excess arguments\n");
                    err = ERR_ARGUMENT;
                }
                break;
        
            case 'o':
                data->out = 1;
                data->output = (char*)calloc(strlen(optarg), sizeof(char));
                /*освобождение проиходит в main(...), после выполнения основных действий;
                    если корректно выделилась память под структуру data, проверяется корректность выделения памяти под data->output и его очистка,
                        в случае ошибки выделения data, данная функция не будет вызвана*/
                if (!data->output){
                    fprintf(stderr, "Memory error\n");
                    err = ERR_MEMORY;
                }
                if (!err)
                    strncpy(data->output, optarg, strlen(optarg));

                if (optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "Excess arguments\n");
                    err = ERR_ARGUMENT;
                }
                break;

            case 'M':
                data->mirror = 1;
                if (optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "This option does not take arguments\n");
                    err = ERR_ARGUMENT;
                }
                break;

            case 'P':
                data->pentagram = 1;
                if (optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "This option does not take arguments\n");
                    err = ERR_ARGUMENT;
                }
                break;

            case 'R':
                data->rect = 1;
                if (optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "This option does not take arguments\n");
                    err = ERR_ARGUMENT;
                }
                break;

            case 'H':
                data->hexagon = 1;
                if (optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "This option does not take arguments\n");
                    err = ERR_ARGUMENT;
                }
                break;

            case 'A':
                data->compress = 1;
                if (optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "This option does not take arguments\n");
                    err = ERR_ARGUMENT;
                }
                break;

            case 'n':
                arg_err = sscanf(optarg, "%d", &(data->num));
                if (arg_err == -1 || data->num <= 1){
                    fprintf(stderr, "Incorrect num\n");
                    err = ERR_ARGUMENT;
                }
                break;

            case 'a':
                data->axis = optarg[0];
                if (strlen(optarg) != 1 || (data->axis != 'x' && data->axis != 'y')){
                    fprintf(stderr, "Incorrect axis\n");
                    err = ERR_ARGUMENT;
                }

                if (!err && optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "Excess arguments\n");
                    err = ERR_ARGUMENT;
                }
                break;

    
            case 'u':
                arg_err = sscanf(optarg, "%d.%d", &(data->left_up->x), &(data->left_up->y));
                if (arg_err == -1){
                    fprintf(stderr, "Incorrect left_up\n");
                    err = ERR_ARGUMENT;
                }
                if (!err)
                    data->lu = 1;

                if (!err && optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "Excess arguments\n");
                    err = ERR_ARGUMENT;
                }
                break;
    
            case 'd':
                arg_err = sscanf(optarg, "%d.%d", &(data->right_down->x), &(data->right_down->y));
                if (arg_err == -1){
                    fprintf(stderr, "Incorrect right_down\n");
                    err = ERR_ARGUMENT;
                }
                if (!err)
                    data->rd = 1;

                if (!err && optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "Excess arguments\n");
                    err = ERR_ARGUMENT;
                }
                break;

            case 'c':
                arg_err = sscanf(optarg, "%d.%d", &(data->center->x), &(data->center->y));
                if (arg_err == -1){
                    fprintf(stderr, "Incorrect center\n");
                    err = ERR_ARGUMENT;
                }
                if (!err)
                    data->cent = 1;

                if (!err && optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "Excess arguments\n");
                    err = ERR_ARGUMENT;
                }
                break;

            case 'r':
                data->radius = atoi(optarg);
                if (data->radius <= 0){
                    fprintf(stderr, "Incorrect radius\n");
                    err = ERR_ARGUMENT;
                }

                if (!err && optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "Excess arguments\n");
                    err = ERR_ARGUMENT;
                }
                break;

            case 't':
                data->thick = atoi(optarg);
                if (data->thick <= 0){
                    fprintf(stderr, "Incorrect thickness\n");
                    err = ERR_ARGUMENT;
                }

                if (!err && optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "Excess arguments\n");
                    err = ERR_ARGUMENT;
                }
                break;

            case 'C':
                arg_err = sscanf(optarg, "%d.%d.%d", &(data->color->r), &(data->color->g), &(data->color->b));
                if (arg_err == -1 || data->color->r < 0 || data->color->r > 255 || data->color->g < 0 || data->color->g > 255 || data->color->b < 0 || data->color->b > 255){
                    fprintf(stderr, "Incorrect color\n");
                    err = ERR_ARGUMENT;
                }
                if (!err)
                    data->col = 1;

                if (!err && optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "Excess arguments\n");
                    err = ERR_ARGUMENT;
                }
                break;

            case 'f':
                data->fill = 1;
                if (optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "This option does not take arguments\n");
                    err = ERR_ARGUMENT;
                }
                break;
            
            case 'F':
                arg_err = sscanf(optarg, "%d.%d.%d", &(data->fill_color->r), &(data->fill_color->g), &(data->fill_color->b));
                if (arg_err == -1 || data->fill_color->r < 0 || data->fill_color->r > 255 || data->fill_color->b < 0 || data->fill_color->b > 255 || data->fill_color->g < 0 || data->fill_color->g > 255){
                    fprintf(stderr, "Incorrect fill_color\n");
                    err = ERR_ARGUMENT;
                }
                if (!err)
                    data->fc = 1;

                if (!err && optind < argc-1 && (argv[optind][0] != '-' || (strlen(argv[optind]) == 1 && argv[optind][0] == '-'))){
                    fprintf(stderr, "Excess arguments\n");
                    err = ERR_ARGUMENT;
                }
                break;

            case '?':
                fprintf(stderr, "Incorrect argument\n");
                err = ERR_ARGUMENT;
        }
    }

    if (!err){
        if (!data->in){
            if (argc < 2 && (data->mirror || data->rect || data->pentagram || data->hexagon)){
                fprintf(stderr, "Absent input file\n");
                err = ERR_FILE;
            }
            else if (argc >= 2 && !last){
                data->in = 1;
                data->input = (char*)calloc(strlen(argv[argc-1]), sizeof(char));
                /*второй случай выеления памяти под data->input, в один запуск программы выполниться оба выделение не могут, т.к. вызываются при наличии либо отсутствии ввода конкретного флага;
                    освобождение проиходит в main(...), после выполнения основных действий;
                        если корректно выделилась память под структуру data, проверяется корректность выделения памяти под data->input и его очистка,
                            в случае ошибки выделения data, данная функция не будет вызвана*/
                if (!data->input){
                    fprintf(stderr, "Memory error\n");
                    err = ERR_MEMORY;
                }
                if (!err)
                    strncpy(data->input, argv[argc-1], strlen(argv[argc-1]));
            }
        }

        if (!data->out){
            data->output = (char*)calloc(8, sizeof(char));
            /*второй случай выеления памяти под data->output, в один запуск программы выполниться оба выделение не могут, т.к. вызываются при наличии либо отсутствии ввода конкретного флага;
                освобождение проиходит в main(...), после выполнения основных действий;
                    если корректно выделилась память под структуру data, проверяется корректность выделения памяти под data->output и его очистка,
                        в случае ошибки выделения data, данная функция не будет вызвана*/
            if (!data->output){
                fprintf(stderr, "Memory error\n");
                err = ERR_MEMORY;
            }
            if (!err)
                strncpy(data->output, "out.png\0", 8);
        }
        
        if (!err && data->in && data->out && !strcmp(data->input, data->output)){
            fprintf(stderr, "Input and output files are the same\n");
            err = ERR_FILE;
        }

        if (!err && data->mirror+data->pentagram+data->rect+data->hexagon+data->help+data->info+data->compress > 1){
            fprintf(stderr, "Too many options\n");
            err = ERR_ARGUMENT;
        }

        if (!err && ((data->mirror && (data->axis == -1 || !data->lu || !data->rd))
        || (data->pentagram && (!data->cent || data->radius == -1 || data->thick == -1 || !data->col))
        || (data->rect && (!data->lu || !data->rd || data->thick == -1 || !data->col || (data->fill && !data->fc) || (!data->fill && data->fc)))
        || (data->hexagon && (!data->cent || data->radius == -1 || data->thick == -1 || !data->col || (data->fill && !data->fc) || (!data->fill && data->fc)))
        || data->compress && !data->num)){
            fprintf(stderr, "Missing required flags\n");
            err = ERR_ARGUMENT;
        }

        if (!err && ((data->mirror && (data->num || data->cent || data->radius != -1 || data->thick != -1 || data->col || data->fill || data->fc))
        || (data->pentagram && (data->num || data->axis != -1 || data->lu || data->rd || data->fill || data->fc))
        || (data->rect && (data->num || data->axis != -1 || data->cent || data->radius != -1))
        || (data->hexagon && (data->num || data->axis != -1 || data->lu || data->rd))
        || (data->num && (data->cent || data->radius != -1 || data->thick != -1 || data->col || data->fill || data->fc || data->axis != -1 || data->lu || data->rd)))){
            fprintf(stderr, "Unnecessary parameters\n");
            err = ERR_ARGUMENT;
        }

        if (!err && data->rd && data->lu){
            if (data->left_up->x > data->right_down->x){
                int tmp=data->left_up->x;
                data->left_up->x = data->right_down->x;
                data->right_down->x = tmp;
            }
            if (data->left_up->y > data->right_down->y){
                int tmp=data->left_up->y;
                data->left_up->y = data->right_down->y;
                data->right_down->y = tmp;
            }
        }
    }

    return err;
}