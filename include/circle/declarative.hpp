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

#define SET(propname, value) c_working_obj.propname = value

#define CHILD(type, name)                                                      \
    if (type& name = c_working_obj.add<type>(); &name)                         \
        if (type& c_working_obj = name; &c_working_obj)                        \
            if (circle::anchors& anchors = name.anchors; &anchors)