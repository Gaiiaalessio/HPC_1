#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

unsigned char mandelbrot(double real, double imag, int max_iter) {
    double z_real = real;
    double z_imag = imag;
    for (int n = 0; n < max_iter; n++) {
        double r2 = z_real * z_real;
        double i2 = z_imag * z_imag;
        if (r2 + i2 > 4.0) return n;
        z_imag = 2.0 * z_real * z_imag + imag;
        z_real = r2 - i2 + real;
    }
    return max_iter;
}

int main(int argc, char *argv[]) {
    double global_start_time = omp_get_wtime();

    int width = 800, height = 600;
    double x_left = -2.0, x_right = 1.0, y_lower = -1.0, y_upper = 1.0;
    int max_iterations = 255;
    int num_threads = 1;

    if (argc == 9) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
        x_left = atof(argv[3]);
        y_lower = atof(argv[4]);
        x_right = atof(argv[5]);
        y_upper = atof(argv[6]);
        max_iterations = atoi(argv[7]);
        num_threads = atoi(argv[8]);
    }

    omp_set_num_threads(num_threads);

    unsigned char* image_buffer = (unsigned char*)malloc(width * height * sizeof(unsigned char));
    if (image_buffer == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    #pragma omp parallel for collapse(2)
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            double x = x_left + i * (x_right - x_left) / width;
            double y = y_lower + j * (y_upper - y_lower) / height;
            unsigned char value = mandelbrot(x, y, max_iterations);
            image_buffer[j * width + i] = value;
        }
    }

    FILE *file = fopen("image.pgm", "w");
    fprintf(file, "P5\n%d %d\n255\n", width, height);
    fwrite(image_buffer, sizeof(unsigned char), width * height, file);
    fclose(file);
    free(image_buffer);

    double global_end_time = omp_get_wtime();
    printf("%f", global_end_time - global_start_time);

    return 0;
}

