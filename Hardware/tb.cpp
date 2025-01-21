#include <iostream>
#include <cmath>
#include "ap_axi_sdata.h"
#include "hls_stream.h"
using namespace std;

void cordic_stream(hls::stream<ap_axis<32,2,5,6>>& input_stream,
                  hls::stream<ap_axis<32,2,5,6>>& output_stream);

int main() {
    float test_angles[] = {0, 30, 45, 60, 90};
    int num_tests = sizeof(test_angles) / sizeof(test_angles[0]);
    float epsilon = 0.01;

    for(int j = 0; j < num_tests; j++) {
        hls::stream<ap_axis<32,2,5,6>> input_stream, output_stream;
        ap_axis<32,2,5,6> input_data, output_data;

        float angle = test_angles[j];
        input_data.data = *(int*)&angle;
        input_data.keep = 1;
        input_data.strb = 1;
        input_data.user = 1;
        input_data.id = 0;
        input_data.dest = 1;
        input_data.last = 1;

        input_stream.write(input_data);
        cordic_stream(input_stream, output_stream);

        if (output_stream.empty()) {
            cout << "ERROR: Output stream is empty for angle " << angle << " degrees" << endl;
            return 1;
        }

        output_stream.read(output_data);
        float result = *(float*)&output_data.data;
        float expected = sin(angle * M_PI / 180.0);

        cout << "Test " << j + 1 << ":" << endl;
        cout << "Angle: " << angle << " degrees" << endl;
        cout << "Expected sin: " << expected << endl;
        cout << "CORDIC result: " << result << endl;

        if (abs(result - expected) > epsilon) {
            cout << "ERROR: Results mismatch for angle " << angle << " degrees" << endl;
            cout << "Difference: " << abs(result - expected) << endl;
            return 1;
        }

        cout << "-------------------" << endl;
    }

    cout << "Success: All tests passed within error margin of " << epsilon << endl;
    return 0;
}
