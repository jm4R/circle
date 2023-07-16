#include <circle/application.hpp>
#include <circle/column.hpp>
#include <circle/image.hpp>
#include <circle/mouse_area.hpp>
#include <circle/reactive/bind.hpp>
#include <circle/rectangle.hpp>
#include <circle/row.hpp>
#include <circle/text.hpp>
#include <circle/timer.hpp>
#include <circle/window.hpp>

int main(int argc, char* args[])

{
    using namespace circle;

    application app;
    window window;

    // rectangle
    rectangle& r1 = window.content_item.add<rectangle>();
    r1.anchors.fill(r1.parent);
    r1.anchors.margins = 10;
    r1.radius = 18;
    r1.color = 0x800020;

    // rectangle with gradient
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

    auto& mouse = r2.add<mouse_area>();
    mouse.anchors.fill(r2);

    r1.color =
        BIND((mouse.contains_press, cm), color{cm ? 0x800020u : 0xff0020u});

    // item
    auto& it1 = r1.add<item>();
    it1.anchors.fill(&r1);
    it1.anchors.set_left(&r1, anchors::horizontal_center);
    it1.anchors.margins = 30;
    {
        // column
        auto& col = it1.add<column>();
        {
            for (int i = 0; i < 25; ++i)
            {
                auto& r1 = col.add<rectangle>();
                r1.width = i * 3 + 2;
                r1.height = 10;
                r1.color = color{std::uint8_t(i * 10), 128, 128};
            }
        }
        col.top_padding = 3;
        col.left_padding = 4;
        col.spacing = 1;

        // row
        auto& rr = it1.add<row>();
        {
            for (int i = 0; i < 25; ++i)
            {
                auto& r1 = rr.add<rectangle>();
                r1.width = 3;
                r1.height = i * 10 + 2;
                r1.color = color{std::uint8_t(i * 10), 128, 128};
            }
        }
        rr.top_padding = 3;
        rr.left_padding = 4;
        rr.spacing = 1;
    }

    image& img = window.content_item.add<image>();
    img.source = "test.png";

    text& txt = window.content_item.add<text>();
    txt.font.point_size = 24;
    txt.color = BIND((txt.x, x), color(80, x/2, 80));
    txt.value = "Circle is both a library and a framework";
    txt.anchors.set_top(img, anchors::bottom);
    txt.x = 30;

    timer t;
    t.interval = 8;
    t.triggered.connect([&]{
        static int dx = 2;
        txt.x = txt.x + dx;
        r1.radius = r1.radius + dx;
        if (txt.x == 0 || txt.x == 300)
            dx *= -1;
    });
    t.start();

    window.show();
    app.exec();
    return 0;
}