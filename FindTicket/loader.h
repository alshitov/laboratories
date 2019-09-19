#ifndef LOADER_H
#define LOADER_H
#include <dataloadersingleton.h>

QT_BEGIN_NAMESPACE
namespace loader { static auto& dls = DataLoaderSingleton::getInstance(); }
QT_END_NAMESPACE

#endif // LOADER_H
