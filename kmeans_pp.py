import math
import sys
import numpy as np
import pandas as pd
import mykmeanssp
from numpy import random
np.random.seed(0)




def create_output(index_array,vectors_array):
    st =""
    for i in range (len(index_array)):
        if (i == len(index_array)-1):
            st += str(index_array[i])
        else:
            st += str(index_array[i]) 
            st += ","
    print(st)
    for vector in vectors_array:
        for i in range(len(vector)):
            vector[i] = '{:.4f}'.format(vector[i])
        print(','.join(vector))

def calc_distance(vector1, vector2):
    i = 0
    summ = 0
    for i in range(len(vector1)):
        summ += pow((vector1[i] - vector2[i]), 2)
    return math.sqrt(summ)

def find_closest_cluster(vector1,arr_of_vectors):
    distance = float('inf')
    for i in range (len(arr_of_vectors)):
        curr_distance = calc_distance(vector1,arr_of_vectors[i])
        if (curr_distance < distance):
            distance = curr_distance
    return distance

def sum_distance_arr(arr):
    sum = 0
    for i in arr:
        sum += i
    return sum

def main():
    lst_of_arguments = sys.argv
    # Check the number of command-line arguments
    i = 0
    k = 0
    iter = 0
    eps = 0
    main_df = ""
    dataframe_1 = ""
    dataframe_2 = ""
    first_file = ""
    second_file = ""
    first_argument = lst_of_arguments[1]
    second_argument = lst_of_arguments[2]
    cnt = 0
    str11 = "An Error Has Occurred"
    str22 = "Invalid number of clusters!"
    str33 = "Invalid number of iteration!"
    err =[0,0,0]
    if (len(lst_of_arguments)<5 or len(lst_of_arguments)>6 ):
        print ("An Error Has Occurred")
        return 1
    if (len(lst_of_arguments) == 5):
        if first_argument.isdigit():
            k = int(first_argument)
            if (k < 1 or k >= 100000):
                cnt += 1
                err[1] = str22
        else:
            cnt += 1
            err[1] = str22
        iter = 300
        if second_argument.isdigit():
            eps = float(second_argument)
        else:
            cnt += 1
            err[0] = str11
        i=3
    else:
        if first_argument.isdigit():
            k = int(first_argument)
            if (k < 1 or k >= 100000):
                cnt += 1
                err[1] = str22
        else:
            cnt += 1
            err[1] = str22
        if second_argument.isdigit():
            iter = int(second_argument)
            if (iter < 1 or iter >1000):
                cnt += 1
                err[2] = str33
        else:
            cnt += 1
            err[2] = str33
        try:
            eps = float(lst_of_arguments[3])
        except:
            cnt += 1
            err[0] = str11
        i=4
    if (cnt > 0):
        for i in range(3):
            if (err[i] != 0 ):
                print(err[i])
        return 1
    first_file = lst_of_arguments[i]
    if (first_file[-4:] == ".txt"):
        try:
            data = np.genfromtxt(first_file, dtype=float, encoding=None, delimiter=",")
            len_of_text = len(data[0])
            cols = [i for i in range(len_of_text)]
            dataframe_1 = pd.DataFrame(data, columns=cols)
        except:
            cnt += 1
            err[0] = str11
    elif ((first_file[-4:] == ".csv")):
        try:
            dataframe_1 = pd.read_csv(first_file,header=None)
        except:
            cnt += 1
            err[0] = str11
    else:
        cnt += 1
        err[0] = str11
    second_file = lst_of_arguments[i+1]
    if (second_file[-4:] == ".txt"):
        try:
            data = np.genfromtxt(second_file, dtype=float, encoding=None, delimiter=",")
            len_of_text = len(data[0])
            cols = [i for i in range(len_of_text)]
            dataframe_2 = pd.DataFrame(data, columns=cols)
        except:
            cnt += 1
            err[0] = str11
    elif ((second_file[-4:] == ".csv")):
        try:
            dataframe_2 = pd.read_csv(second_file,header=None)
        except:
            cnt += 1
            err[0] = str11
    else:
        cnt += 1
        err[0] = str11
    if (cnt > 0):
        for i in range(3):
            if err[i] != 0 :
                print(err[i])
        return 1
    main_df = pd.merge(dataframe_1,dataframe_2,on = dataframe_1.columns[0])
    cols = ["index"]
    num_of_cols = main_df.shape[1]
    cols += ["cord" +str(i) for i in range (1,num_of_cols)]
    main_df.columns = cols
    main_df["index"] = main_df["index"].astype(int)
    main_df = main_df.set_index("index")
    num_of_cols = main_df.shape[1]
    cols = ["cord" +str(i) for i in range (num_of_cols)]
    main_df.columns = cols
    main_df = main_df.sort_index()
    N = main_df.shape[0]
    if (k<=1) or (k>=N):
        print("Invalid number of clusters!")
        return 1
    array_of_centroids = []
    array_of_vectors = []
    index_arr = []
    array_of_distances = [None for i in range(N)]
    for index,row in main_df.iterrows():
        array_of_vectors.append(list(row))
    rand_index = random.randint(N)
    array_of_centroids.append(array_of_vectors[rand_index])
    index_arr.append(rand_index)
    while (len(array_of_centroids)<k):
        for i in range(N):
            array_of_distances[i] = find_closest_cluster(array_of_vectors[i],array_of_centroids)
        sum_of_distances = sum(array_of_distances)
        for i in range(len(array_of_distances)):
            array_of_distances[i] = array_of_distances[i]/sum_of_distances
        curr_index = np.random.choice(N,1,p=array_of_distances)[0]
        vector = array_of_vectors[curr_index]
        index_arr.append(curr_index)
        array_of_centroids.append(vector)
    lst1 = mykmeanssp.fit(array_of_vectors, array_of_centroids, iter,eps)
    create_output(index_arr,lst1)
    return 0


if __name__ == "__main__":
    main()
