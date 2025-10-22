#include <circle/item.hpp>

namespace circle {

item::item() : anchors{tracking_form_this<item>()} {}

void item::add(item_ptr child)
{
    if (child->parent == this)
        return;
    //    if (child->parent)
    //        child->parent->remove(child);
    child->parent = tracking_form_this<item>();
    children_.emplace_back(std::move(child));
}

void item::render(sdl::context& ctx)
{
    if (visible)
    {
        draw(ctx);

        for (auto& child : children_)
            child->render(ctx);
    }
}

void item::draw(sdl::context ctx)
{
    // item doesn't have visual representation
}

} // namespace circle