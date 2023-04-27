#include <circle/item.hpp>

namespace circle {

item::item() : anchors{this} {}

item::~item()
{
    enable_tracking_ptr::call_before_destroyed();
}

item::item(item&& other)
    : object{std::move(other)}, anchors{std::move(other.anchors)}
{
    enable_tracking_ptr::call_moved();
}

item& item::operator=(item&& other)
{
    object::operator=(std::move(other));
    enable_tracking_ptr::call_moved();
    return *this;
}

void item::add(item_ptr child)
{
    if (child->parent == this)
        return;
    //    if (child->parent)
    //        child->parent->remove(child);
    child->parent = this;
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