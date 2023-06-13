#pragma once

#include <type_traits>

namespace circle::detail {

template <typename T, typename = void>
struct fwd_declared : public std::false_type
{
};

template <typename T>
struct fwd_declared<T, std::void_t<typename T::type>> : public std::true_type
{
    using type = typename T::type;
};

template <typename T>
struct base_component
{
    using component_t = T;
    explicit base_component() = default;

    template <typename Tag, typename ComponentT, typename ParentT>
    auto& c_get_declared_or_add(ParentT& parent)
    {
        if constexpr (circle::detail::fwd_declared<Tag>::value)
        {
            auto& res = Tag::getter(static_cast<component_t&>(*this));
            parent.add(&res);
            return res;
        }
        else
        {
            return parent.template add<ComponentT>();
        }
    }
};

} // namespace circle::detail

#define COMPONENT(base, name)                                                  \
    class name : public base, public ::circle::detail::base_component<name>

#define DECLARE(component_type, name)                                          \
private:                                                                       \
    component_type name{};                                                     \
    struct c_##name##_tag                                                      \
    {                                                                          \
        using type = component_type;                                           \
        static type& getter(component_t& comp) { return comp.name; }           \
    }

#define BODY(name)                                                             \
public:                                                                        \
    explicit name()                                                            \
    {                                                                          \
        c_body(*this, *this, this->anchors);                                   \
    }                                                                          \
                                                                               \
private:                                                                       \
    void c_body(component_t& c_working_obj, component_t& parent,               \
                ::circle::anchors& anchors)

#define SET(propname, value) c_working_obj.propname = value

#define SET_BIND(propname, ...) c_working_obj.propname = BIND(__VA_ARGS__)

#define CHILD(type, name)                                                      \
    type* name =                                                               \
        &c_get_declared_or_add<struct c_##name##_tag, type>(c_working_obj);    \
    if (auto& parent = c_working_obj; &parent)                                 \
        if (type& c_working_obj = *name; &c_working_obj)                       \
            if (::circle::anchors& anchors = c_working_obj.anchors; &anchors)

#define END ; // just to make clang-format happy