#include <circle/application.hpp>
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
    auto& rh = r2.height;
    r2.radius = BIND(rh, rh / 3);
    r2.gradient = gradient{gradient::vertical,
                           {
                               gradient_stop{0, 0xe6c47e},
                               gradient_stop{0.05, 0x936a29},
                               gradient_stop{0.47, 0xe9be5a},
                               gradient_stop{0.82, 0x7a4b1e},
                               gradient_stop{1, 0x996632},
                           }};

    window.show();
    app.exec();
    return 0;
}