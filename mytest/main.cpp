#include <Wt/WApplication>
#include "application.h"

using namespace Wt;

int main(int argc, char **argv)
{
   return WRun(argc, argv, &Application::createApplication);
}

