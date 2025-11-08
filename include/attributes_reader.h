#ifndef ATTRIBUTES_READER_H
#define ATTRIBUTES_READER_H

#include <stdio.h>
#include "classfile.h"

stackMapTable_attribute* readStackMapTable(FILE *fp);
bootstrap_methods_attribute* readBootstrapMethods(FILE *fp);

#endif  // ATTRIBUTES_READER_H
