#define _GNU_SOURCE
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
};

double **build_table(int num_of_clusters, double **input_data, int dim);
double calc_distance(double *vector1, double *vector2, int dim);
void update_centroid(double **clusters, int* clusters_cnt, int dim, double **new_centroids, int num_of_clusters);
double calc_delta(double **new_centroids, double **prev_centroids, int num_of_clusters,int dim);
void create_output(double **vectors_array, int num_of_clusters, int d);
double** kmeans(int K, int iter, double **input_data, double **array_of_centroids,int N, int dim, double epsilon);
//int check_if_string_is_ok(char* string);
//double **createArrayfromInput(int *N ,int *dim_size,struct vector *head,int k,struct cord *a);
//double* turn_list_to_array(struct vector *vec,int d);
struct vector free_vector(struct vector *vec);
//int number_of_input(struct vector * vectors);
//int get_len_from_list(struct vector *vec);
void build_clusters(double **array_of_centroids,double **clusters,int* clusters_cnt,double **input_data, int dim, int N, int num_of_clusters);
void free_matrices(double ** array,int num);
void copy(double **array_of_centroids,double **prev_array_of_centroids,int dim, int num_of_clusters);
void freeVectors(struct vector* headVec);
double **allocate_memory(int num_of_clusters, int dim);

void freeVectors(struct vector* headVec)
{
    struct vector* tmp;
    while (headVec != NULL)
    {
        tmp = headVec;
        headVec = headVec->next;
        free(tmp);
    }
}
struct vector free_vector(struct vector *vec){
    struct vector *v_next = vec->next;
    struct cord* cordin = vec->cords;
    while (cordin != NULL){
        struct cord* next = cordin->next;
        free (cordin);
        cordin = next;
    }
    free(vec);
    return *v_next;
}


void free_matrices(double ** array,int num){
    int i;
    i = 0;
    while (i <num){
        free(array[i]);
        i += 1;
    }
    free(array);
}

double ** kmeans(int K, int iter, double **input_data, double **array_of_centroids,int N, int dim, double epsilon){
    int num_of_clusters = K;
    double **new_centroids = allocate_memory(num_of_clusters, dim);
    copy(array_of_centroids, new_centroids, dim, num_of_clusters);
    //double **temp_array = new_centroids;
    double **temp_array1 = array_of_centroids;
    int iter_cnt = 0;
    double delta = epsilon + 1 ;
    double **prev_array_of_centroids = allocate_memory(num_of_clusters, dim);
    while (iter_cnt < iter && delta >= epsilon){
        double **clusters =(double**) calloc(num_of_clusters, sizeof(double*));
        int *clusters_cnt = (int*) calloc(num_of_clusters, sizeof(int*));
        int i;
        iter_cnt ++;
        for(i = 0; i < num_of_clusters; i++){
            clusters[i] = (double*) calloc(dim ,sizeof(double));
        }
        build_clusters(array_of_centroids,clusters,clusters_cnt,input_data, dim, N, num_of_clusters);
        update_centroid(clusters, clusters_cnt,dim, new_centroids, num_of_clusters);
        array_of_centroids = new_centroids;
        if(iter_cnt != 1){
            delta = calc_delta(new_centroids, prev_array_of_centroids, num_of_clusters,dim);
        }
        copy(array_of_centroids,prev_array_of_centroids,dim,num_of_clusters);
        free_matrices(clusters, num_of_clusters);
        free(clusters_cnt);
    }
    free_matrices(temp_array1, num_of_clusters);
    free_matrices(prev_array_of_centroids, num_of_clusters);
    //free_matrices(temp_array, num_of_clusters);
    return array_of_centroids;
}

void copy(double **array_of_centroids, double **prev_array_of_centroids, int dim, int num_of_clusters){
    int i, j;
    for(i = 0; i < num_of_clusters; i++){
        for(j = 0; j < dim; j++){
            prev_array_of_centroids[i][j] = array_of_centroids[i][j];
        }
    }
}
void build_clusters(double **array_of_centroids,double **clusters,int* clusters_cnt,double **input_data, int dim, int N, int num_of_clusters) {
    int i;
    int k;
    for (i = 0; i < N; i++) {
        double *vector = input_data[i];
        double min_distance;
        int min_distance_index;
        int j;

        vector = input_data[i];
        min_distance = INFINITY;
        min_distance_index = -1;

        for (j = 0; j < num_of_clusters; j++) {
            double *centroid = array_of_centroids[j];
            double curr_distance = calc_distance(vector, centroid, dim);
            if (curr_distance < min_distance) {
                min_distance = curr_distance;
                min_distance_index = j;
            }
        }
        clusters_cnt[min_distance_index] += 1;
        for (k = 0; k < dim; k++) {
            clusters[min_distance_index][k] += vector[k];
        }
    }
}


double **allocate_memory(int num_of_clusters, int dim){
    double **array = (double**) malloc(num_of_clusters * sizeof(double*));
    int i;
    for(i = 0; i < num_of_clusters; i++){
        array[i] = (double*) calloc(dim , sizeof(double));
    } return array;
}


double calc_distance(double *vector1, double *vector2, int dim){
    double summ = 0.0;
    int i;
    for(i = 0; i < dim; i++){
        summ += pow((vector1[i] - vector2[i]),2);
    }
    return sqrt(summ);
}

void update_centroid(double **clusters, int* clusters_cnt, int dim, double **new_centroids, int num_of_clusters) {
    int i;
    int j;
    double clusters_sum;
    int cluster_num_of_points;
    for (i = 0; i < num_of_clusters; i++){
        for(j = 0; j < dim; j++){
            clusters_sum = clusters[i][j];
            cluster_num_of_points = clusters_cnt[i];
            new_centroids[i][j] =  clusters_sum / cluster_num_of_points;
        }
    }
}


double calc_delta(double **new_centroids, double **prev_centroids, int num_of_clusters,int dim){
    double delta = 0.0;
    int i;
    for(i = 0; i < num_of_clusters; i++){
        double distance = calc_distance(new_centroids[i], prev_centroids[i],dim);
        if (distance > delta){
            delta = distance;
        }
    }
    return delta;
}


void create_output(double **vectors_array, int num_of_clusters, int dim){
    int i = 0;
    while (i < num_of_clusters){
        int j = 0;
        while (j < dim){
            double num = vectors_array[i][j];
            if (j == dim-1){
                printf("%.4f\n",num);
            }
            else {
                printf("%.4f,",num);
            }
            j += 1;
        }
        i += 1;
    }
}
