#ifndef __EDGE__
#define __EDGE__

#include "buildable.h"

class Edge : public buildable {
public:
    Edge(int id);
    ~Edge();
    std::string toStrForm() const override;
    void build(int p);
    bool adjValid(int p) const override;
    std::vector<int> getAdjRoads() const;
};

#endif
