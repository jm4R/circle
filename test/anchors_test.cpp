#include <circle/anchors.hpp>
#include <circle/item.hpp>

#include <catch2/catch.hpp>

#include <string>

using namespace circle;

/*
 * NOTES:
 * - source margins doesn't count: everything is calculated based on its drawing
 *   area;
 * - margins doesn't have any meaning when adjacent anchor is not set;
 * - x property value is modified by anchors
 *
 */

TEST_CASE("anchors")
{
    item it50;
    it50.width = 50;
    it50.height = 50;
    it50.x = 50;
    it50.y = 50;

    item it;

    SECTION("calculate x: as sibling")
    {
        SECTION("no horizontal")
        {
            it.x = 1000;
            REQUIRE(it.x == 1000);

            it.anchors.left_margin = 1000;
            it.anchors.right_margin = 10000;
            REQUIRE(it.x == 1000);
        }

        SECTION("left")
        {
            it.anchors.set_left(&it50, anchors::left);
            REQUIRE(it.x == 50);

            it.anchors.set_left(&it50, anchors::horizontal_center);
            REQUIRE(it.x == 75);

            it.anchors.set_left(&it50, anchors::right);
            REQUIRE(it.x == 100);
        }

        SECTION("left with margin")
        {
            it.anchors.left_margin = 1000;
            it.anchors.right_margin = 10000;

            it.anchors.set_left(&it50, anchors::left);
            REQUIRE(it.x == 1050);

            it.anchors.set_left(&it50, anchors::horizontal_center);
            REQUIRE(it.x == 1075);

            it.anchors.set_left(&it50, anchors::right);
            REQUIRE(it.x == 1100);
        }

        SECTION("horizontal center")
        {
            it.anchors.left_margin = 1000;
            it.anchors.right_margin = 10000;
            it.width = 10;

            it.anchors.set_horizontal_center(&it50, anchors::left);
            REQUIRE(it.x == 45);

            it.anchors.set_horizontal_center(&it50, anchors::horizontal_center);
            REQUIRE(it.x == 70);

            it.anchors.set_horizontal_center(&it50, anchors::right);
            REQUIRE(it.x == 95);
        }

        SECTION("right")
        {
            it.width = 10;

            it.anchors.set_right(&it50, anchors::left);
            REQUIRE(it.x == 40);

            it.anchors.set_right(&it50, anchors::horizontal_center);
            REQUIRE(it.x == 65);

            it.anchors.set_right(&it50, anchors::right);
            REQUIRE(it.x == 90);
        }

        SECTION("right with margin")
        {
            it.anchors.left_margin = 1000;
            it.anchors.right_margin = 10000;
            it.width = 10;

            it.anchors.set_right(&it50, anchors::left);
            REQUIRE(it.x == 40 - 10000);

            it.anchors.set_right(&it50, anchors::horizontal_center);
            REQUIRE(it.x == 65 - 10000);

            it.anchors.set_right(&it50, anchors::right);
            REQUIRE(it.x == 90 - 10000);
        }

        SECTION("dynamic change")
        {
            it.anchors.set_left(&it50, anchors::horizontal_center);
            REQUIRE(it.x == 75);

            it50.width = 1000;
            REQUIRE(it.x == 550);
        }
    }

    SECTION("calculate x: as child")
    {
        it.parent = &it50;

        SECTION("no horizontal")
        {
            it.x = 1000;
            REQUIRE(it.x == 1000);

            it.anchors.left_margin = 1000;
            it.anchors.right_margin = 10000;
            REQUIRE(it.x == 1000);
        }

        SECTION("left")
        {
            it.anchors.set_left(&it50, anchors::left);
            REQUIRE(it.x == 0);

            it.anchors.set_left(&it50, anchors::horizontal_center);
            REQUIRE(it.x == 25);

            it.anchors.set_left(&it50, anchors::right);
            REQUIRE(it.x == 50);
        }

        SECTION("left with margin")
        {
            it.anchors.left_margin = 1000;
            it.anchors.right_margin = 10000;

            it.anchors.set_left(&it50, anchors::left);
            REQUIRE(it.x == 1000);

            it.anchors.set_left(&it50, anchors::horizontal_center);
            REQUIRE(it.x == 1025);

            it.anchors.set_left(&it50, anchors::right);
            REQUIRE(it.x == 1050);
        }

        SECTION("horizontal center")
        {
            it.anchors.left_margin = 1000;
            it.anchors.right_margin = 10000;
            it.width = 10;

            it.anchors.set_horizontal_center(&it50, anchors::left);
            REQUIRE(it.x == -5);

            it.anchors.set_horizontal_center(&it50, anchors::horizontal_center);
            REQUIRE(it.x == 20);

            it.anchors.set_horizontal_center(&it50, anchors::right);
            REQUIRE(it.x == 45);
        }

        SECTION("right")
        {
            it.width = 10;

            it.anchors.set_right(&it50, anchors::left);
            REQUIRE(it.x == -10);

            it.anchors.set_right(&it50, anchors::horizontal_center);
            REQUIRE(it.x == 15);

            it.anchors.set_right(&it50, anchors::right);
            REQUIRE(it.x == 40);
        }

        SECTION("right with margin")
        {
            it.anchors.left_margin = 1000;
            it.anchors.right_margin = 10000;
            it.width = 10;

            it.anchors.set_right(&it50, anchors::left);
            REQUIRE(it.x == -10 - 10000);

            it.anchors.set_right(&it50, anchors::horizontal_center);
            REQUIRE(it.x == 15 - 10000);

            it.anchors.set_right(&it50, anchors::right);
            REQUIRE(it.x == 40 - 10000);
        }

        SECTION("dynamic change")
        {
            it.anchors.set_left(&it50, anchors::horizontal_center);
            REQUIRE(it.x == 25);

            it50.width = 1000;
            REQUIRE(it.x == 500);
        }
    }

    SECTION("calculate width")
    {
        SECTION("no horizontal")
        {
            it.width = 1000;
            REQUIRE(it.width == 1000);
        }

        SECTION("left")
        {
            it.width = 1000;
            it.anchors.set_left(&it50, anchors::left);
            REQUIRE(it.width == 1000);
        }

        SECTION("horizontal center")
        {
            it.width = 1000;
            it.anchors.set_horizontal_center(&it50, anchors::left);
            REQUIRE(it.width == 1000);
        }

        SECTION("right")
        {
            it.width = 1000;
            it.anchors.set_right(&it50, anchors::left);
            REQUIRE(it.width == 1000);
        }

        SECTION("left & right")
        {
            it.width = 1000;
            it.anchors.set_left(&it50, anchors::left);
            it.anchors.set_right(&it50, anchors::left);
            REQUIRE(it.width == 0);

            it.anchors.set_right(&it50, anchors::horizontal_center);
            REQUIRE(it.width == 25);

            it.anchors.set_right(&it50, anchors::right);
            REQUIRE(it.width == 50);

            it.anchors.set_left(&it50, anchors::horizontal_center);
            REQUIRE(it.width == 25);

            it.anchors.set_left(&it50, anchors::right);
            REQUIRE(it.width == 0);
        }

        // other configurations are UB

        SECTION("dynamic change")
        {
            it.anchors.set_left(&it50, anchors::left);
            it.anchors.set_right(&it50, anchors::right);
            REQUIRE(it.width == 50);

            it50.width = 1000;
            REQUIRE(it.width == 1000);
        }
    }

    SECTION("calculate y: as sibling")
    {
        SECTION("no vertical")
        {
            it.y = 1000;
            REQUIRE(it.y == 1000);

            it.anchors.top_margin = 1000;
            it.anchors.bottom_margin = 10000;
            REQUIRE(it.y == 1000);
        }

        SECTION("left")
        {
            it.anchors.set_top(&it50, anchors::top);
            REQUIRE(it.y == 50);

            it.anchors.set_top(&it50, anchors::vertical_center);
            REQUIRE(it.y == 75);

            it.anchors.set_top(&it50, anchors::bottom);
            REQUIRE(it.y == 100);
        }

        SECTION("left with margin")
        {
            it.anchors.top_margin = 1000;
            it.anchors.bottom_margin = 10000;

            it.anchors.set_top(&it50, anchors::top);
            REQUIRE(it.y == 1050);

            it.anchors.set_top(&it50, anchors::vertical_center);
            REQUIRE(it.y == 1075);

            it.anchors.set_top(&it50, anchors::bottom);
            REQUIRE(it.y == 1100);
        }

        SECTION("horizontal center")
        {
            it.anchors.top_margin = 1000;
            it.anchors.bottom_margin = 10000;
            it.height = 10;

            it.anchors.set_vertical_center(&it50, anchors::top);
            REQUIRE(it.y == 45);

            it.anchors.set_vertical_center(&it50, anchors::vertical_center);
            REQUIRE(it.y == 70);

            it.anchors.set_vertical_center(&it50, anchors::bottom);
            REQUIRE(it.y == 95);
        }

        SECTION("right")
        {
            it.height = 10;

            it.anchors.set_bottom(&it50, anchors::top);
            REQUIRE(it.y == 40);

            it.anchors.set_bottom(&it50, anchors::vertical_center);
            REQUIRE(it.y == 65);

            it.anchors.set_bottom(&it50, anchors::bottom);
            REQUIRE(it.y == 90);
        }

        SECTION("right with margin")
        {
            it.anchors.top_margin = 1000;
            it.anchors.bottom_margin = 10000;
            it.height = 10;

            it.anchors.set_bottom(&it50, anchors::top);
            REQUIRE(it.y == 40 - 10000);

            it.anchors.set_bottom(&it50, anchors::vertical_center);
            REQUIRE(it.y == 65 - 10000);

            it.anchors.set_bottom(&it50, anchors::bottom);
            REQUIRE(it.y == 90 - 10000);
        }

        SECTION("dynamic change")
        {
            it.anchors.set_top(&it50, anchors::vertical_center);
            REQUIRE(it.y == 75);

            it50.height = 1000;
            REQUIRE(it.y == 550);
        }
    }

    SECTION("calculate y: as child")
    {
        it.parent = &it50;

        SECTION("no vertical")
        {
            it.y = 1000;
            REQUIRE(it.y == 1000);

            it.anchors.top_margin = 1000;
            it.anchors.bottom_margin = 10000;
            REQUIRE(it.y == 1000);
        }

        SECTION("top")
        {
            it.anchors.set_top(&it50, anchors::top);
            REQUIRE(it.y == 0);

            it.anchors.set_top(&it50, anchors::vertical_center);
            REQUIRE(it.y == 25);

            it.anchors.set_top(&it50, anchors::bottom);
            REQUIRE(it.y == 50);
        }

        SECTION("top with margin")
        {
            it.anchors.top_margin = 1000;
            it.anchors.bottom_margin = 10000;

            it.anchors.set_top(&it50, anchors::top);
            REQUIRE(it.y == 1000);

            it.anchors.set_top(&it50, anchors::vertical_center);
            REQUIRE(it.y == 1025);

            it.anchors.set_top(&it50, anchors::bottom);
            REQUIRE(it.y == 1050);
        }

        SECTION("vertical center")
        {
            it.anchors.top_margin = 1000;
            it.anchors.bottom_margin = 10000;
            it.height = 10;

            it.anchors.set_vertical_center(&it50, anchors::top);
            REQUIRE(it.y == -5);

            it.anchors.set_vertical_center(&it50, anchors::vertical_center);
            REQUIRE(it.y == 20);

            it.anchors.set_vertical_center(&it50, anchors::bottom);
            REQUIRE(it.y == 45);
        }

        SECTION("bottom")
        {
            it.height = 10;

            it.anchors.set_bottom(&it50, anchors::top);
            REQUIRE(it.y == -10);

            it.anchors.set_bottom(&it50, anchors::vertical_center);
            REQUIRE(it.y == 15);

            it.anchors.set_bottom(&it50, anchors::bottom);
            REQUIRE(it.y == 40);
        }

        SECTION("bottom with margin")
        {
            it.anchors.top_margin = 1000;
            it.anchors.bottom_margin = 10000;
            it.height = 10;

            it.anchors.set_bottom(&it50, anchors::top);
            REQUIRE(it.y == -10 - 10000);

            it.anchors.set_bottom(&it50, anchors::vertical_center);
            REQUIRE(it.y == 15 - 10000);

            it.anchors.set_bottom(&it50, anchors::bottom);
            REQUIRE(it.y == 40 - 10000);
        }

        SECTION("dynamic change")
        {
            it.anchors.set_top(&it50, anchors::vertical_center);
            REQUIRE(it.y == 25);

            it50.height = 1000;
            REQUIRE(it.y == 500);
        }
    }

    SECTION("calculate height")
    {
        SECTION("no vertical")
        {
            it.height = 1000;
            REQUIRE(it.height == 1000);
        }

        SECTION("top")
        {
            it.height = 1000;
            it.anchors.set_top(&it50, anchors::top);
            REQUIRE(it.height == 1000);
        }

        SECTION("vertical center")
        {
            it.height = 1000;
            it.anchors.set_vertical_center(&it50, anchors::top);
            REQUIRE(it.height == 1000);
        }

        SECTION("bottom")
        {
            it.height = 1000;
            it.anchors.set_bottom(&it50, anchors::top);
            REQUIRE(it.height == 1000);
        }

        SECTION("top & bottom")
        {
            it.height = 1000;
            it.anchors.set_top(&it50, anchors::top);
            it.anchors.set_bottom(&it50, anchors::top);
            REQUIRE(it.height == 0);

            it.anchors.set_bottom(&it50, anchors::vertical_center);
            REQUIRE(it.height == 25);

            it.anchors.set_bottom(&it50, anchors::bottom);
            REQUIRE(it.height == 50);

            it.anchors.set_top(&it50, anchors::vertical_center);
            REQUIRE(it.height == 25);

            it.anchors.set_top(&it50, anchors::bottom);
            REQUIRE(it.height == 0);
        }

        // other configurations are UB

        SECTION("dynamic change")
        {
            it.anchors.set_top(&it50, anchors::top);
            it.anchors.set_bottom(&it50, anchors::bottom);
            REQUIRE(it.height == 50);

            it50.height = 1000;
            REQUIRE(it.height == 1000);
        }
    }
}
