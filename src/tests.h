#ifndef TESTS_H
#define TESTS_H

// #define TESTS

#include <QVector>
#include <QString>
#include "dtm.h"

#define TEST_DTM_FILE "../data/Ecrins2.xyz"

bool runTests();
void testOnreadDTM(const QString& dtmFileName);

#endif // TESTS_H
