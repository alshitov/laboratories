#ifndef LINK_H
#define LINK_H
#include <dataloadersingleton.h>
#include <dataprocessorsingleton.h>

QT_BEGIN_NAMESPACE
namespace processor { static auto& dps = DataProcessorSingleton::getInstance(); }
QT_END_NAMESPACE

#endif // LINK_H
