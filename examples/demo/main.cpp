#include <circle/application.hpp>
#include <circle/column.hpp>
#include <circle/reactive/bind.hpp>
#include <circle/rectangle.hpp>
#include <circle/window.hpp>

int main(int argc, char* args[])

{
    using namespace circle;

    application app;
    window window;

    rectangle& r1 = window.content_item.add<rectangle>();
    r1.anchors.fill(r1.parent);
    r1.anchors.margins = 10;
    r1.radius = 18;
    r1.color = 0x800020;

    auto& r2 = r1.add<rectangle>();
    r2.anchors.fill(&r1);
    r2.anchors.set_right(&r1, anchors::horizontal_center);
    r2.anchors.margins = 30;
    r2.radius = BIND((r2.height, rh), rh / 3);
    r2.gradient = gradient{gradient::vertical,
                           {
                               gradient_stop{0, 0xe6c47e},
                               gradient_stop{0.05, 0x936a29},
                               gradient_stop{0.47, 0xe9be5a},
                               gradient_stop{0.82, 0x7a4b1e},
                               gradient_stop{1, 0x996632},
                           }};

    auto& col = r2.add<column>();
    {
        for (int i=0; i<25; ++i)
        {
            auto& r1 = col.add<rectangle>();
            r1.width = i * 2;
            r1.height = 10;
            r1.color = color{std::uint8_t(i * 10), 128, 128};
        }
    }
    col.top_padding = BIND((r2.radius, r2r), r2r);
    col.left_padding = 4;
    col.spacing = 1;

    window.show();
    app.exec();
    return 0;
}