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
#include <time.h>
#include "lo/lo.h"
#include "kmeans.h"
#include "rotate.h"

#define Xmin 217.0
#define Xmax 1459.0
#define Ymin 2173.0
#define Ymax 3517.0
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
    int *XY;
    /*
    int k, kk;
    double cluster_centroid[32];
    int   *cluster_assignment_final;
    double last[2];
    */
    double unitX = 0.0;
    double unitY = 0.0;
    unitX = (Xmax - Xmin) / 448.0;
    unitY = (Ymax - Ymin) / 448.0;
    if (open_urg_sensor(&urg, argc, argv) < 0) {
        return 1;
    }

    data = (long *)malloc(urg_max_data_size(&urg) * sizeof(data[0]));
    if (!data) {
        perror("urg_max_index()");
        return 1;
    }

    // \~english Defines the measurement scope (start, end steps)
    // \~english Defines a measurement scope of 90 [deg] at the front of the sensor, and no step grouping in this example
    /*
    int first_step = urg_rad2step(&urg, -22);
    int last_step = urg_rad2step(&urg, +22);
    */
    int first_step = urg_deg2step(&urg, 0.0); //urg_rad2step(&urg, 0);
    int last_step = urg_deg2step(&urg, 37.0); //urg_rad2step(&urg, 0.65);
    int skip_step = 0;
    int ret = urg_set_scanning_parameter(&urg, first_step, last_step, skip_step);
    // \todo check error code

    // \~english Defines the number of scans
    // \~english 123 scans are requested, and no scan skipping in this example
    int scan_times = 1;
    int skip_scan = 0;
    int counter = 0;
    int aluanX = 0;
    int aluanY = 0;
    /*
    pthread_t t1;

    pthread_create(&t1,NULL,say_hello,"hello from 1");
    pthread_join(t1,NULL);
    */
    lo_address t = lo_address_new("127.0.0.1", "12002");
    //urg_start_measurement(&urg, URG_DISTANCE, URG_SCAN_INFINITY, skip_scan);
    int milisec = 200; // length of time to sleep, in miliseconds
    struct timespec req = {0};
    req.tv_sec = 0;
    req.tv_nsec = milisec * 1000000L;

    while(1)
    {
        // Gets measurement data
        urg_start_measurement(&urg, URG_DISTANCE, scan_times, skip_scan);
        n = urg_get_distance(&urg, data, &time_stamp);
        if (n <= 0) {
            printf("urg_get_distance: %s\n", urg_error(&urg));
            urg_close(&urg);
            return 1;
        }
        else
        {
            //cluster_assignment_final = (int *)malloc(sizeof(int) * n);
            XY = (int *)malloc( (sizeof(int) * n) << 1 );
            counter = 0;
        }

        // Outputs X-Y coordinates
        urg_distance_min_max(&urg, &min_distance, &max_distance);
        for (i = 0; i < n; ++i)
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
            if( fabs(x) > Xmin && fabs(y) > Ymin && fabs(x) < Xmax && fabs(y) < Ymax )
            {
                XY[counter++] = (int)x;
                XY[counter++] = (int)y;
            }
        }

        if(counter > 0)
        {
            if(counter >= 2)
            {
                qsort (XY, counter >> 1, sizeof(int) << 1, compareYD);
                aluanX = XY[0];
                qsort (XY, counter >> 1, sizeof(int) << 1, compareXA);
                aluanY = XY[1];
            }
            inputMatrix[0][0] = (double)aluanX;
            inputMatrix[1][0] = (double)aluanY;
            inputMatrix[2][0] = 0.0;
            inputMatrix[3][0] = 1.0;
            outputMatrix[0][0] = (double)aluanX;
            outputMatrix[1][0] = (double)aluanY;
            outputMatrix[2][0] = 0.0;
            outputMatrix[3][0] = 1.0;
            showPoint();

            /*
            setUpRotationMatrix(0.0, 1.0, 0.0, 0.0);
            multiplyMatrix();
            showPoint();
            setUpRotationMatrix(0.0, 0.0, 1.0, 0.0);
            multiplyMatrix();
            showPoint();
            */
            setUpRotationMatrix(-0.6, 0.0, 0.0, 1.0);
            multiplyMatrix();
            //showPoint();
            aluanX = (int)( (outputMatrix[0][0] - Xmin) / unitX );
            aluanY = (int)( (Ymin + outputMatrix[1][0]) / -unitY );
            if ( lo_send(t, "/radar", "iii", 5, aluanX, aluanY ) == -1 )
                printf("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
            else if(0)
                printf("%ld ,%ld\n", aluanX, aluanY);
            nanosleep(&req, (struct timespec *)NULL);
        }

        free(XY);
    }
    // Disconnects
    free(data);
    urg_close(&urg);

#if defined(URG_MSC)
    getchar();
#endif
    return 0;
}
