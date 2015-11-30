#ifndef _adabase_h_
#define _adabase_h_

void add(int argc, char* argv[], FILE *name);
void read(FILE *name);
int search(char* argv[],FILE *name);
void writeClassifier(struct strongClassifier *strong);
void readClassifier(struct strongClassifier* strong);


#endif
