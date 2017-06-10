/*!
  \example calculate_xy.c Calculates X-Y coordinates

  Having the X axis aligned to the front step of the sensor, calculates the coordinates for measurement data
  \author Satofumi KAMIMURA

  $Id: calculate_xy.c,v e5d1719877a2 2015/05/07 04:12:14 jun $
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include "open_urg_sensor.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "lo/lo.h"
#include "kmeans.h"
#define Xmin 100.0
#define Xmax 1200.0
#define Ymin 100.0
#define Ymax 1200.0
 
/*
void* say_hello(void* data)
{
    char *str;
    str = (char*)data;
    while(1)
    {
        printf("%s\n",str);
        sleep(1);
    }
}
*/
int main(int argc, char *argv[])
{
    urg_t urg;
    long *data;
    long max_distance;
    long min_distance;
    long time_stamp;
    int i;
    int n;
    int dim = 2;
    double *X;
    int k, kk;
    double cluster_centroid[32];
    int   *cluster_assignment_final;
    double last[2];

    if (open_urg_sensor(&urg, argc, argv) < 0) {
        return 1;
    }

    data = (long *)malloc(urg_max_data_size(&urg) * sizeof(data[0]));
    if (!data) {
        perror("urg_max_index()");
        return 1;
    }
    /*
    pthread_t t1;

    pthread_create(&t1,NULL,say_hello,"hello from 1");
    pthread_join(t1,NULL);
    */
    lo_address t = lo_address_new("127.0.0.1", "7770");
    //urg_start_measurement(&urg, URG_DISTANCE, URG_SCAN_INFINITY, 0);
    while(1)
    {
        // Gets measurement data
        urg_start_measurement(&urg, URG_DISTANCE, 1, 0);
        n = urg_get_distance(&urg, data, &time_stamp);
        sleep(0.01);
        if (n <= 0) {
            printf("urg_get_distance: %s\n", urg_error(&urg));
            urg_close(&urg);
            return 1;
        }
        else
        {
            //cluster_assignment_final = (int *)malloc(sizeof(int) * n);
            X = (double *)malloc(sizeof(double) * dim * n );
            for (int ii = 0; ii < 16; ii++)
            {
                cluster_centroid[ii*dim] = 0.0;
                cluster_centroid[ii*dim+1] = 0.0;
            }
            k = 0;
			kk = 0;
            last[0] = 0.0;
            last[1] = 0.0;
        }

        // Outputs X-Y coordinates
        urg_distance_min_max(&urg, &min_distance, &max_distance);
        for (i = 0; i < n && k < 16; ++i) 
		{
            long distance = data[i];
            double radian;
            double x;
            double y;

            if ((distance < min_distance) || (distance > max_distance)) {
                continue;
            }

            radian = urg_index2rad(&urg, i);
            y = distance * cos(radian) * -1.0;
            x = distance * sin(radian);

            if( ( abs(x) > Xmin || abs(y) > Ymin ) && abs(x) < Xmax && abs(y) < Ymax )
            {
                double distprint;
                X[kk*dim] = x;
                X[kk*dim+1] = y;
                if( distprint = calc_distance(dim, &X[kk*dim], last) > 8100.0 )
                {
                    printf("%lf\n", radian);
                    cluster_centroid[k*dim] = x;
                    cluster_centroid[k*dim+1] = y;
					//printf("%ld, %ld\n", (long)x, (long)y);
                    k++;
				}
                kk++;
                last[0] = x;
                last[1] = y;
                //printf("%ld, %ld\n", (long)x, (long)y);

            }
        }
        /*
		for (int ii = 0; ii < k; ii++)
        {
            if( ( abs(cluster_centroid[ii*dim]) > Xmin || abs(cluster_centroid[ii*dim+1]) > Ymin ) && abs(cluster_centroid[ii*dim]) < Xmax && abs(cluster_centroid[ii*dim+1]) < Ymax )
                printf("%lf, %lf\n", cluster_centroid[ii*dim], cluster_centroid[ii*dim+1]);
		}
        */
		kmeans(dim, X, kk, k, cluster_centroid, cluster_assignment_final);
        //printf("%ld", k);
		for (int ii = 0; ii < k; ii++)
        {
            if( ( abs(cluster_centroid[ii*dim]) > Xmin || abs(cluster_centroid[ii*dim+1]) > Ymin ) && abs(cluster_centroid[ii*dim]) < Xmax && abs(cluster_centroid[ii*dim+1]) < Ymax )
            {
                if(lo_send(t, "/xy", "ff", (float)cluster_centroid[ii*dim], (float)cluster_centroid[ii*dim+1]) == -1)
                    printf("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
                else if(0)
                    printf("%ld = %lf, %lf\n", ii, cluster_centroid[ii*dim], cluster_centroid[ii*dim+1]);	
	        }
        }
        
		/*
        for (int ii = 0; ii < n; ii++)
        {
            printf("%ld, ", cluster_assignment_final);
        }
        */
        //printf("\n\n");

        //free(cluster_assignment_final);
        free(X);
    }
    // Disconnects
    free(data);
    urg_close(&urg);

#if defined(URG_MSC)
    getchar();
#endif
    return 0;
}
