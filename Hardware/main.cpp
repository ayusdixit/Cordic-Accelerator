#include "ap_axi_sdata.h"
#include "hls_stream.h"
#include <ap_fixed.h>

typedef ap_fixed<32,12> angle_type;
typedef ap_fixed<32,12> cos_sin_type;

void cordic_stream(hls::stream<ap_axis<32,2,5,6>>& input_stream,
                  hls::stream<ap_axis<32,2,5,6>>& output_stream)
{
#pragma HLS INTERFACE axis port=input_stream
#pragma HLS INTERFACE axis port=output_stream
#pragma HLS INTERFACE s_axilite port=return

    // CORDIC constants
    const int NUM_ITERATIONS = 16;

    angle_type cordic_phase[16] = {
        (angle_type)0.785398163,  // 45.000 degrees
        (angle_type)0.463647609,  // 26.565 degrees
        (angle_type)0.244978663,  // 14.036 degrees
        (angle_type)0.124354995,  // 7.125 degrees
        (angle_type)0.062418810,  // 3.576 degrees
        (angle_type)0.031239833,  // 1.790 degrees
        (angle_type)0.015623729,  // 0.895 degrees
        (angle_type)0.007812341,  // 0.447 degrees
        (angle_type)0.003906230,  // 0.224 degrees
        (angle_type)0.001953122,  // 0.112 degrees
        (angle_type)0.000976562,  // 0.056 degrees
        (angle_type)0.000488281,  // 0.028 degrees
        (angle_type)0.000244141,  // 0.014 degrees
        (angle_type)0.000122070,  // 0.007 degrees
        (angle_type)0.000061035,  // 0.003 degrees
        (angle_type)0.000030518   // 0.002 degrees
    };


    const cos_sin_type CORDIC_SCALE = (cos_sin_type)0.607252935;
    const angle_type PI = (angle_type)3.14159265;
    const angle_type PI_HALF = (angle_type)1.5708;
    const angle_type DEG_TO_RAD = (angle_type)0.0174532925;

    while(1) {
        ap_axis<32,2,5,6> input_data;
        ap_axis<32,2,5,6> output_data;

        // Read input
        input_stream.read(input_data);

        // Convert input float to radians
        float input_angle = *(float*)&input_data.data;
        angle_type theta = (angle_type)input_angle; // First convert to angle_type
        theta = theta * DEG_TO_RAD; // Then multiply with DEG_TO_RAD

        // Normalize angle to [-pi/2, pi/2]
        bool negate_result = false;
        if (theta > PI_HALF) {
            theta = PI - theta;
            negate_result = true;
        } else if (theta < -PI_HALF) {
            theta = -PI - theta;
            negate_result = true;
        }

        // Initialize CORDIC variables
        cos_sin_type current_cos = CORDIC_SCALE;
        cos_sin_type current_sin = (cos_sin_type)0;
        angle_type current_theta = (angle_type)0;

        // CORDIC iterations
        CORDIC_LOOP: for(int j = 0; j < NUM_ITERATIONS; j++) {
            #pragma HLS PIPELINE II=1

            int direction = (theta > current_theta) ? 1 : -1;

            // Store current values
            cos_sin_type cos_temp = current_cos;
            cos_sin_type sin_temp = current_sin;

            // Update values
            current_cos = cos_temp - (direction * (sin_temp >> j));
            current_sin = sin_temp + (direction * (cos_temp >> j));
            current_theta = current_theta + (direction * cordic_phase[j]);
        }


        if (negate_result) {
            current_sin = -current_sin;
        }


        float result = current_sin.to_float();
        output_data.data = *(int*)&result;

        // Copy AXI stream signals
        output_data.keep = input_data.keep;
        output_data.strb = input_data.strb;
        output_data.user = input_data.user;
        output_data.last = input_data.last;
        output_data.id = input_data.id;
        output_data.dest = input_data.dest;

        // Write output
        output_stream.write(output_data);

        if(input_data.last) {
            break;
        }
    }
}
