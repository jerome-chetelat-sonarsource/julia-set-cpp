#include <stdio.h>       // MISRA M5-2-12: prefer <cstdio>
#include <math.h>        // MISRA M5-2-12: prefer <cmath>

// MISRA M2-13-2: use constexpr, not #define
#define WIDTH  80
#define HEIGHT 40
#define MAX_ITER 100

int g_render_count = 0;  // MISRA M3-3-1: global mutable state

union Pixel {            // MISRA M9-5-1: unions forbidden
    unsigned int rgba;
    unsigned char channel[4];
};

class Renderer {
public:
    virtual void render() {}
    // MISRA M12-1-1: no virtual destructor
};

class JuliaRenderer : public Renderer {
public:
    void render() {      // MISRA M10-3-1: missing override keyword
        g_render_count++;
        draw();
    }

private:
    void draw() {
        double cr = -0.7;
        double ci = 0.27015;

        int* pixels = new int[WIDTH * HEIGHT];  // MISRA M18-5-1: raw new, use unique_ptr

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                double zr = (x - WIDTH  / 2.0) * 3.5 / WIDTH;
                double zi = (y - HEIGHT / 2.0) * 2.0 / HEIGHT;

                int iter = 0;
                while (iter < MAX_ITER && zr * zr + zi * zi < 4.0) {
                    double tmp = zr * zr - zi * zi + cr;
                    zi = 2.0 * zr * zi + ci;
                    zr = tmp;
                    iter++;
                }

                pixels[y * WIDTH + x] = iter;
            }
        }

        for (unsigned int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                int idx = y * WIDTH + x;  // MISRA M5-0-4: signed/unsigned mismatch (y * WIDTH)
                int iter = pixels[idx];

                const char* shade;
                if (iter == MAX_ITER)
                    shade = "@";          // MISRA M6-4-1: no braces on single-statement if
                else if (iter > 80)
                    shade = "#";
                else if (iter > 60)
                    shade = "*";
                else if (iter > 40)
                    shade = "+";
                else if (iter > 20)
                    shade = ".";
                else
                    shade = " ";

                printf("%s", shade);      // MISRA M27-0-1: use std::cout
            }
            printf("\n");
        }

        Pixel p;
        p.rgba = 0xFF0000FF;
        printf("r=%d\n", (int)p.channel[0]);  // MISRA M5-2-4: C-style cast; M9-5-1: union member access

        void* raw = pixels;
        int* recovered = reinterpret_cast<int*>(raw);  // MISRA M5-2-7: reinterpret_cast
        (void)recovered;

        delete[] pixels;
    }
};

int main() {
    JuliaRenderer* r = NULL;   // MISRA M4-10-2: use nullptr
    r = new JuliaRenderer();   // MISRA M18-5-1: raw new
    r->render();
    delete r;
    return 0;
}
