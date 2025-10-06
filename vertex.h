#ifndef __VERTEX__
#define __VERTEX__

#include "buildable.h"

class Vertex : public buildable {
    int stage = 0;
public:
    Vertex(int id);
    ~Vertex();
    std::string toStrForm() const override;

    bool adjValid(int p) const override;
    bool adjValid(int p, bool isFirst) const;
    
    int getStage() const;
    string getStageChar() const;
    void build(int p);
    void improve();
};

#endif
