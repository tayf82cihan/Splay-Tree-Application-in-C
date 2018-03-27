#include <stdio.h>
#include <stdlib.h>

#include "Splay_Tree.h"


/*
*   compare function must afford 
*   the following conditions
*
*   lhs < rhs,    1
*   lhs > rhs,   -1
*   lhs = rhs,    0
*
*/
int compare_ints(Object lhs, Object rhs) {

    int rhs_int = *((Integer) rhs);
    int lhs_int = *((Integer) lhs);

    if (rhs_int > lhs_int) return 1;
    else if (rhs_int < lhs_int) return -1;
    else return 0;

} /* end of int compare_ints(Object lhs, Object rhs) */

Object copy_int(Object int_to_be_copied) {

    Integer ret_val = (Integer) malloc(sizeof(int));

    *ret_val = *((Integer) int_to_be_copied);

    return ret_val;

} /* end of Object copy_int(Object) */

void destroy_int(Object int_to_be_destroyed) {

    if (int_to_be_destroyed)
        free((Integer) int_to_be_destroyed);

} /* end of void destroy_int(Object) */

Object wrap_int(int int_to_be_wrapped) {

    Integer ip = (Integer) malloc(sizeof(int));
    *ip = int_to_be_wrapped;

    return ip;

} /* end of Object wrap_int(int) */

Object* prepareObjectArray(int* int_arr, int n) {
    
	Object* ret_arr = (Object*) malloc(sizeof(Object) * n);
	int i;
	
	for (i = 0; i < n; i++)
	    ret_arr[i] = wrap_int(int_arr[i]);
	
	return ret_arr;
	
} /* end of Object* prepareObjectArray(int*, int) */


int main(void) {

    Splay_Tree_Component_Funcs int_funcs = { &compare_ints, &copy_int, &destroy_int };
    Splay_Tree_Root root;
	
    int insertList[] = { 
2283,7363,602,7889,5312,252,888,8876,5956,6280,2093,9368,1701,9500,2779,3671,5689,2999,8967,5209,
6624,6049,3562,7183,2794,5675,1443,1254,504,4511,1397,60,8526,3286,7578,1856,2410,3858,4419,5199,
7364,7567,7393,5501,8082,8304,6303,523,7339,9574,3785,1946,4538,1332,3966,4233,3786,3214,8731,7818,
120,9936,6459,6239,9547,3667,7882,7143,6528,2013,1100,775,2466,3760,3618,2374,786,4851,1639,7737,
5536,9323,9444,7569,5167,7942,4710,4280,4397,4610,3703,2812,9464,713,6809,2068,4459,5888,2394,3873,
3981,2480,8925,5792,7221,2217,92,8513,1345,9499,715,782,399,2050,2275,9259,878,5627,471,5169,4453,
7595,4843,9622,2472,3735,2,3167,1414,1819,9181,7404,7059,4730,390,4715,6559,1848,7275,9589,4647,669,
3073,1023,1982,8802,8874,5804,8814,6886,2330,2116,217,6270,4516,4158,9969,6076,4933,1446,1571,1869,
8494,3241,5566,5846,8354,5601,7033,7706,6012,8554,5946,642,4383,3322,1884,7710,5481,3197,3347,3272,
1567,4628,1295,6391,2562,66,427,3930,9586,6301,8581,5875,1654,1923,762,7217,4817,9658,1235,7433,4023,
877,4284,9088,166,9313,8545,3864,7166,1051,1866,7841,922,5453,9086,6853,413,212,395,3220,9570,9206,
6293,7973,9432,2322,4389,547,4250,8959,5532,8336,5175,2424,7407,8817,6115,1510,7069,9576,3262,8884,
5830,4114,7685,1421,5060,370,5977,3475,4341,9971,6491,8050,4289,3359,1598,8422,4697,4941,405,7553,
3432,9787,9581,8703,1037,2379,1467,381,6661,8608,1589,8276,7662,5391,3177,1725,6267,7624,6021,2144,
5961,8389,548,7645,2072,1506,6174,2526,2187,4683,6112,3074,3572,5766,1288,8249,7319,7720,4978,633,721,
2121,5716,2879,6134,9950,6714,8473,2362,5941,2606,5341,3421,2759,3535,3598,1508,552,8134,7229,7713,64,
5316,5539,8947,3302,7219,9844,7346,9103,7020,1111,5041,1641,1365,8205,2574,5892,2511,9467,2833,8571,
6473,9972,4992,8048,9325,5351,2366,8449,3939,6243,2676,6770,3011,7734,1887,9466,4748,6634,629,9989,
8521,562,1436,3641,9973,4202,9033,1815,74,4020,581,7504,763,3757,7359,819,161,856,9776,568,1876,3781,
6380,3752,810,965,7930,9992,4242,4960,6203,3098,6444,1402,8401,8080,4448,1707,843,8338,9075,5023,4621,
9497,8002,137,5310,6034,7401,5641,4560,2883,690,6002,2742,9961,5258,5615,3666,2556,8100,790,1623,2309,
1520,3590,1626,7280,2201,3863,4147,7108,9256,9635,274,3057,3620,8039,9760,129,4296,5878,8873,4756,9142,
1376,3687,663,1712,7652,9913,7187,4594,2649,4081,5706,6849,5944,6872,1594,2811,7689,8412,3750,9084,
8723,846,4031,50,7603,6988,5676,5794,1731,8736,5802,8302,7494,5656,9130,3547,1737,9495,8083,510,5255,
356,6157,4442,3137,5646,1841,7987,5379 
   
    };
	
	int deleteList[] = { 
 
1567,4628,1295,6391,2562,66,427,3930,9586,6301,8581,5875,1654,1923,762,7217,4817,9658,1235,7433,4023,
3432,9787,9581,8703,1037,2379,1467,4341,6661,8608,1589,8276,7662,5391,3177,1725,6267,7624,6021,2144,
8723,846,4031,50,7603,6988,5676,5794,1731,8736,5802,8302,7494,5656,9130,3547,1737,9495,8083,510,5255,
7364,7567,7393,5501,8082,8304,6303,523,7339,9574,3785,1946,4538,1332,3966,4233,3786,3214,8731,7818,
7553,4114,7685,1421,5060,370,5977,3475,381,9971,6491,8050,4289,3359,1598,8422,4697,4941,405,5830
  
  };
	
	  int size_of_insertList = sizeof(insertList)/sizeof(int);
	  int size_of_deleteList = sizeof(deleteList)/sizeof(int);
    Object *obj_array, *deleting_array;				 
    Object founded_obj, to_be_found = wrap_int(9500);    
    int i;

    obj_array = prepareObjectArray(insertList, sizeof(insertList)/sizeof(int));
	  deleting_array = prepareObjectArray(deleteList, sizeof(deleteList)/sizeof(int));
	
    if ((root = Splay_Tree_Create(int_funcs)) == NULL) return -1;

    for (i = 0; i < size_of_insertList; i++)
        Splay_Tree_InsertElement(root, obj_array[i]);
		
	  printf("\nAfter insertion, size of Splay tree: %d\n", Splay_Tree_GetNumberOfElements(root));
	  printf("\nLargest Element: %d\n", *(Integer) Splay_Tree_FindLargestElement(root));
	  printf("\nSmallest Element: %d\n",*(Integer) Splay_Tree_FindSmallestElement(root));
     
    founded_obj = Splay_Tree_BFS_Search(root, to_be_found);
    printf("\nBFS tree before finding...\n");

//    Splay_Tree_GetElement(root, to_be_found);
    
//    founded_obj = Splay_Tree_BFS_Search(root, to_be_found);
    
    if (founded_obj)    
        printf("By using Splay_Tree_BFS_Search, find %d in Splay tree: %d\n", *(Integer)to_be_found,  *(Integer)founded_obj);
    destroy_int(to_be_found);
	
	
	  printf("Removing Operation:\n");
	  for (i = 0; i < size_of_deleteList; i++) {
	      printf("\nRemoving %d\n",  *(Integer)deleting_array[i]);
        founded_obj = Splay_Tree_DeleteElement(root, deleting_array[i]);
        if (compare_ints(founded_obj, deleting_array[i]) == 0)    
            destroy_int(deleting_array[i]);
        else 
            printf("\nCould'nt find %d for removing\n",  *(Integer)deleting_array[i]);
	  }

#ifdef DEBUG	
    Splay_Tree_BFS_Search(root, Splay_Tree_FindLargestElement(root)); 
#endif
	
/*	  printf("\nAfter deletion, size of Splay tree: %d\n", Splay_Tree_GetNumberOfElements(root));
	
#ifdef DEBUG    
    Splay_Tree_PreorderTraversal(root);  
    Splay_Tree_InorderTraversal(root);   
    Splay_Tree_PostorderTraversal(root);  
#endif   
*/
    Splay_Tree_Destroy(&root);
   
    return 0;

} /* end of int main(void) */

