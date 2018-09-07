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
//#define littlestar
#define TopRightX 1262.0
#define TopLeftX 199.0
#define BottomRightX 1262.0
#define BottomLeftX 199.0
#define TopRightY 641.0
#define TopLeftY 641.0
#define BottomRightY 1938.0
#define BottomLeftY 1938.0
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
    //int dim = 2;
    int XY[1024];
    int ghost = 0;
    int why[8][8];

    double Xmin, Xmax, Ymin, Ymax;
    double X1, X2, X3, X4;
    double Y1, Y2, Y3, Y4;
    double radian, x, y;

    double unitX = 0.0;
    double unitY = 0.0;

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
    int first_step = urg_deg2step(&urg, -54.0); //urg_rad2step(&urg, 0);
    int last_step = urg_deg2step(&urg, -7.0); //urg_rad2step(&urg, 0.65);
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
    int lastAluanX = 0;
    int lastAluanY = 0;
    /*
    pthread_t t1;

    pthread_create(&t1,NULL,say_hello,"hello from 1");
    pthread_join(t1,NULL);
    */
    lo_address t = lo_address_new("192.168.0.252", "12002");
    //urg_start_measurement(&urg, URG_DISTANCE, URG_SCAN_INFINITY, skip_scan);
    int milisec = 33; // length of time to sleep, in miliseconds
    struct timespec req = {0};
    req.tv_sec = 0;
    req.tv_nsec = milisec * 1000000L;

    for(int ii = 0; ii < 8; ii++)
        for(int jj = 0; jj < 8; jj++)
            why[ii][jj] = 0;
    int keypress = 0;

    Xmin = min(TopLeftX,BottomLeftX);
    Xmax = max(TopRightX,BottomRightX);
    Ymin = min(TopLeftY,TopRightY);
    Ymax = max(BottomLeftY,BottomRightY);

    while(1)
    {
        // Gets measurement data
        urg_start_measurement(&urg, URG_DISTANCE, scan_times, skip_scan);
        n = urg_get_distance(&urg, data, &time_stamp);
        if (n <= 0) {
            printf("urg_get_distance: %s\n", urg_error(&urg));
			continue;
            //urg_close(&urg);
            //return 1;
        }
        else
        {
            //cluster_assignment_final = (int *)malloc(sizeof(int) * n);
            //XY = (int *)malloc( (sizeof(int) * n) << 1 );
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
#if 1 //def littlestar
                if(0 == keypress)
#endif
                    keypress = 1;
            }
        }

        if(counter > 5)
        {
            qsort (XY, counter >> 1, sizeof(int) << 1, compareYD);
            aluanX = XY[2];
            qsort (XY, counter >> 1, sizeof(int) << 1, compareXD);
            aluanY = XY[5];

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
            setUpRotationMatrix(-0.6, 0.0, 0.0, 1.0);
            multiplyMatrix();
            showPoint();
            */
            X1 = (TopRightX-fabs(outputMatrix[0][0]));
            X2 = (fabs(outputMatrix[0][0])-TopLeftX);
            Ymin = (X2 * TopRightY + X1 * TopLeftY) / (X2 + X1);
            X3 = (BottomRightX-fabs(outputMatrix[0][0]));
            X4 = (fabs(outputMatrix[0][0])-BottomLeftX);
            Ymax = (X4 * BottomRightY + X3 * BottomLeftY) / (X4 + X3);
            Y1 = (TopLeftY-fabs(outputMatrix[1][0]));
            Y2 = (fabs(outputMatrix[1][0])-BottomLeftY);
            Xmin = (Y1 * BottomLeftX + Y2 * TopLeftX) / (Y2 + Y1);
            Y3 = (TopRightY-fabs(outputMatrix[1][0]));
            Y4 = (fabs(outputMatrix[1][0])-BottomRightY);
            Xmax = (Y3 * BottomRightX + Y4 * TopRightX) / (Y4 + Y3);
            //printf("(%lf, %lf) ~ (%lf, %lf)\n", Xmin, Ymin, Xmax, Ymax);
            unitX = (Xmax - Xmin) / 448.0;
            unitY = (Ymax - Ymin) / 448.0;            
            aluanX = (int)( (outputMatrix[0][0] - Xmin) / unitX );
            aluanY = (int)( (Ymin + outputMatrix[1][0]) / -unitY );
            //printf("%ld ,%ld\n", aluanX, aluanY);            
#if 0 //def littlestar
            why[3][aluanX/56]++;
#else
            why[aluanY/56][aluanX/56]++;
#endif
            //printf("%ld\n", ghost);
            if(ghost++ > 7)
            {
                int lastone = -1;
                int iii, jjj;
                for(int ii = 0; ii < 8; ii++)
                    for(int jj = 0; jj < 8; jj++)
                        if( why[ii][jj] > lastone )
                        {
                            lastone = why[ii][jj];
                            iii = ii;
                            jjj = jj;
                        }
                aluanY = iii * 56 + 28;
                aluanX = jjj * 56 + 28;
#if 1 //def littlestar
                if (aluanX != lastAluanX || aluanY != lastAluanY
                   )
                {
                    //printf("%ld, %ld\n", aluanX, lastAluanX);
                    lastAluanX = aluanX;
                    lastAluanY = aluanY;
                    keypress = 1;
                }
#endif
                if( 1 == keypress )
                {
                    keypress = 2;
#if 0 //def littlestar
                    if ( lo_send(t, "/radar", "iii", 3, aluanX, 196 ) == -1 )
#else
                    if ( lo_send(t, "/radar", "iii", 3, aluanX, aluanY ) == -1 )
#endif
                        printf("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
                    else if(0)
                        printf("%ld ,%ld\n", aluanX, aluanY);
					if ( lo_send(t, "/button", "iff", 3, outputMatrix[0][0], outputMatrix[0][1] ) == -1 )
                        printf("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
                    //nanosleep(&req, (struct timespec *)NULL);
                }
                ghost = 0;
                for(int ii = 0; ii < 8; ii++)
                    for(int jj = 0; jj < 8; jj++)
                        why[ii][jj] = 0;
            }

        }
        else
        {
            if(keypress > 0)
            {
#if 1 //def littlestar
                if ( lo_send(t, "/radar", "iii", 3, 0, 0 ) == -1 )
                    printf("OSC error %d: %s\n", lo_address_errno(t), lo_address_errstr(t));
                else if(1)
                    printf("%ld ,%ld\n", 0, 0);
#endif
                ghost = 0;
                keypress = 0;
                //printf("%ld\n", ghost);
                for(int ii = 0; ii < 8; ii++)
                    for(int jj = 0; jj < 8; jj++)
                        why[ii][jj] = 0;
            }

        }
        //free(XY);
    }
    // Disconnects
    free(data);
    urg_close(&urg);

#if defined(URG_MSC)
    getchar();
#endif
    return 0;
}
