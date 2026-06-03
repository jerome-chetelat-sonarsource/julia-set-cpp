#include <cstdlib>
#include <iostream>
#include <memory>

constexpr int WIDTH    = 80;
constexpr int HEIGHT   = 40;
constexpr int MAX_ITER = 100;

// global mutable state
int g_render_count = 0; // sonar-resolve [accept] cpp:S5421 "Easier to track changes :kappa:"

class Renderer {
public:
    virtual void render() {
      // rendering
    }
    virtual ~Renderer() = default;
};

class JuliaRenderer : public Renderer {
public:
    void render() override {
        g_render_count++;
        draw();
    }

private:
    void draw() {
        double cr = (rand() % 2 == 0) ? -0.7    : -0.4; // sonar-resolve [accept] cpp:S5020,cpp:S2245 "This is fine"
        double ci = (rand() % 2 == 0) ?  0.27015 :  0.6;

        auto pixels = std::make_unique<int[]>(WIDTH * HEIGHT);

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

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                int iter = pixels[y * WIDTH + x];

                const char* shade;
                if (iter == MAX_ITER) {
                    shade = "@";
                } else if (iter > 80) {
                    shade = "#";
                } else if (iter > 60) {
                    shade = "*";
                } else if (iter > 40) {
                    shade = "+";
                } else if (iter > 20) {
                    shade = ".";
                } else {
                    shade = " ";
                }

                std::cout << shade;
            }
            std::cout << '\n';
        }
    }
};

int main() {
    auto r = std::make_unique<JuliaRenderer>();
    r->render();
    return 0;
}
