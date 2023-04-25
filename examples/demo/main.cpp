#include <circle/application.hpp>
#include <circle/rectangle.hpp>
#include <circle/window.hpp>


int main(int argc, char* args[])

{
    using namespace circle;

    application app;
    window window;

    window.show();
    app.exec();
    return 0;
}