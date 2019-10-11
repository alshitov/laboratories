#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <dataloadersingleton.h>
#include <dataprocessorsingleton.h>

QT_BEGIN_NAMESPACE
namespace processor { static auto& dps = DataProcessorSingleton::getInstance(); }
QT_END_NAMESPACE

#endif // PROCESSOR_H
