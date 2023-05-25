#pragma once

namespace circle::detail {

struct declarative_build_context
{
    void postprocess() {}
};

} // namespace circle::detail

#define COMPONENT_BEGIN(base, name)                                            \
    class name : public base                                                   \
    {                                                                          \
    public:                                                                    \
        explicit name()                                                        \
        {                                                                      \
            auto& c_working_obj{*this};                                        \
            circle::anchors& anchors = this->anchors;                          \
            circle::detail::declarative_build_context c_ctx;

#define COMPONENT_END                                                          \
    c_ctx.postprocess();                                                       \
    }                                                                          \
    }                                                                          \
    ;

// TODO: experiment, make context available
#define COMPONENT(base, name)                                                  \
    class name : public base                                                   \
    {                                                                          \
    public:                                                                    \
        explicit name();                                                       \
    };                                                                         \
    inline name::name()

#define SET(propname, value) c_working_obj.propname = value

#define CHILD(type, name)                                                      \
    type* name;                                                                \
    if (auto& parent = c_working_obj; &parent)                                 \
        if (type& c_working_obj = parent.add<type>(); (name = &c_working_obj)) \
            if (circle::anchors& anchors = c_working_obj.anchors; &anchors)