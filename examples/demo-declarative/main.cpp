#include <circle/application.hpp>
#include <circle/column.hpp>
#include <circle/declarative.hpp>
#include <circle/reactive/bind.hpp>
#include <circle/rectangle.hpp>
#include <circle/row.hpp>
#include <circle/window.hpp>

using namespace circle;

COMPONENT_BEGIN(rectangle, dot)
{
    SET(width, 22);
    SET(height, 22);
    SET(radius, 100);
    SET(color, 0x202020);
}
COMPONENT_END;

const auto dice_gradient =
    gradient{gradient::vertical,
             {gradient_stop{0, 0xe0e0e0}, gradient_stop{0.8, 0xc0c0c0}, gradient_stop{0.95, 0x909090}}};

COMPONENT_BEGIN(rectangle, dice)
{
    SET(width, 114);
    SET(height, 114);
    SET(radius, 10);
    SET(gradient, dice_gradient);
}
COMPONENT_END;


COMPONENT_BEGIN(rectangle, panel)
{
    SET(color, 0x800020);

    CHILD(rectangle, area1)
    {
        anchors.set_left(area1.parent, anchors::left);
        anchors.set_right(area1.parent, anchors::right);
        SET(height, 134);
        SET(color, 0xa00020);

        CHILD(row, dices)
        {
            SET(spacing, 12);
            SET(padding, 10);

            CHILD(dice, dice1)
            {
                CHILD(dot, d1)
                {
                    anchors.center_in(&dice1);
                }
            }

            CHILD(dice, dice2)
            {
                CHILD(dot, d1)
                {
                    anchors.set_left(&dice2, anchors::left);
                    anchors.set_bottom(&dice2, anchors::bottom);
                    anchors.margins = 14;
                }

                CHILD(dot, d2)
                {
                    anchors.set_right(&dice2, anchors::right);
                    anchors.set_top(&dice2, anchors::top);
                    anchors.margins = 14;
                }
            }

            CHILD(dice, dice3)
            {
                CHILD(dot, d1)
                {
                    anchors.set_left(&dice3, anchors::left);
                    anchors.set_bottom(&dice3, anchors::bottom);
                    anchors.margins = 14;
                }

                CHILD(dot, d2)
                {
                    anchors.set_right(&dice3, anchors::right);
                    anchors.set_top(&dice3, anchors::top);
                    anchors.margins = 14;
                }

                CHILD(dot, d3)
                {
                    anchors.center_in(&dice3);
                }
            }

            CHILD(dice, dice4)
            {
                CHILD(dot, d1)
                {
                    anchors.set_left(&dice4, anchors::left);
                    anchors.set_bottom(&dice4, anchors::bottom);
                    anchors.margins = 14;
                }

                CHILD(dot, d2)
                {
                    anchors.set_right(&dice4, anchors::right);
                    anchors.set_top(&dice4, anchors::top);
                    anchors.margins = 14;
                }

                CHILD(dot, d3)
                {
                    anchors.set_left(&dice4, anchors::left);
                    anchors.set_top(&dice4, anchors::top);
                    anchors.margins = 14;
                }

                CHILD(dot, d4)
                {
                    anchors.set_right(&dice4, anchors::right);
                    anchors.set_bottom(&dice4, anchors::bottom);
                    anchors.margins = 14;
                }
            }

            CHILD(dice, dice5)
            {
                CHILD(dot, d1)
                {
                    anchors.set_left(&dice5, anchors::left);
                    anchors.set_bottom(&dice5, anchors::bottom);
                    anchors.margins = 14;
                }

                CHILD(dot, d2)
                {
                    anchors.set_right(&dice5, anchors::right);
                    anchors.set_top(&dice5, anchors::top);
                    anchors.margins = 14;
                }

                CHILD(dot, d3)
                {
                    anchors.set_left(&dice5, anchors::left);
                    anchors.set_top(&dice5, anchors::top);
                    anchors.margins = 14;
                }

                CHILD(dot, d4)
                {
                    anchors.set_right(&dice5, anchors::right);
                    anchors.set_bottom(&dice5, anchors::bottom);
                    anchors.margins = 14;
                }

                CHILD(dot, d5)
                {
                    anchors.center_in(&dice5);
                }
            }

            CHILD(dice, dice6)
            {
                CHILD(dot, d1)
                {
                    anchors.set_left(&dice6, anchors::left);
                    anchors.set_bottom(&dice6, anchors::bottom);
                    anchors.margins = 14;
                }

                CHILD(dot, d2)
                {
                    anchors.set_right(&dice6, anchors::right);
                    anchors.set_top(&dice6, anchors::top);
                    anchors.margins = 14;
                }

                CHILD(dot, d3)
                {
                    anchors.set_left(&dice6, anchors::left);
                    anchors.set_top(&dice6, anchors::top);
                    anchors.margins = 14;
                }

                CHILD(dot, d4)
                {
                    anchors.set_right(&dice6, anchors::right);
                    anchors.set_bottom(&dice6, anchors::bottom);
                    anchors.margins = 14;
                }

                CHILD(dot, d5)
                {
                    anchors.set_left(&dice6, anchors::left);
                    anchors.set_vertical_center(&dice6, anchors::vertical_center);
                    anchors.margins = 14;
                }

                CHILD(dot, d6)
                {
                    anchors.set_right(&dice6, anchors::right);
                    anchors.set_vertical_center(&dice6, anchors::vertical_center);
                    anchors.margins = 14;
                }
            }
        }
    }
}
COMPONENT_END;

int main(int argc, char* args[])

{
    using namespace circle;

    application app;
    window window{800, 400};

           // rectangle
    auto& r1 = window.content_item.add<panel>();
    r1.anchors.fill(r1.parent);

    window.show();
    app.exec();
    return 0;
}